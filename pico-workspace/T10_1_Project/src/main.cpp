/**
 * @file main.cpp
 * @brief Example top-level application flow for the Module 10 secure BLE
 * project.
 *
 * DISCLAIMER:
 * This file is a task template for ELEC C7222 Module 10 Task 10.1.
 * It contains task-specific TODO notes that indicate the required
 * implementation points.
 *
 * This file documents one possible orchestration design for the project. The
 * main idea is to keep responsibilities separated:
 * - startup and task creation stay in @ref main,
 * - BLE stack bring-up is handled in @ref BleTask,
 * - advertising and connection handling are delegated to the GAP layer and
 *   its event handler,
 * - security setup is performed before the secured Attribute Server is
 *   enabled,
 * - service objects are resolved from the parsed GATT database after
 *   @ref c7222::Ble::EnableAttributeServer returns, and
 * - board events such as button presses are converted into BLE-visible
 *   service behavior.
 *
 * This is the same high-level progression used in the earlier Pico BLE
 * modules:
 * - Module 7 established the GAP startup and event-handler pattern,
 * - Module 8 added GATT database enable, service lookup, and characteristic
 *   event handling, and
 * - Module 9 inserted Security Manager setup before enabling a secured
 *   Attribute Server.
 *
 * The project does not require this exact function decomposition. It is an
 * example application structure that makes the runtime order explicit.
 */

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <memory>

#include "advertisement_data.hpp"
#include "attribute_server.hpp"
#include "ble.hpp"
#include "characteristic.hpp"
#include "c7222_pico_w_board.hpp"
#include "freertos_event_group.hpp"
#include "freertos_task.hpp"
#include "gap.hpp"
#include "platform.hpp"
#include "pwm.hpp"
#include "security_manager.hpp"
#include "service.hpp"

#include "alert_notification_service.hpp"
#include "app_profile.h"
#include "gap_event_handler.hpp"
#include "immediate_alert_service.hpp"
#include "security_event_handler.hpp"

namespace {

/** @brief BLE device name required by the project brief. */
constexpr const char* kDeviceName = "FindMe-11";
/** @brief Event-group bit raised when the user button is pressed. */
constexpr uint32_t kButtonPressedEventMask = (1u << 0);

/**
 * @brief Small container that groups the resolved project services.
 *
 * The parsed GATT database exposes services through the AttributeServer after
 * the profile has been enabled. This helper struct collects the resolved
 * service pointers needed by the example application flow.
 */
struct ProjectGattObjects {
	/** @brief Resolved Immediate Alert Service instance. */
	c7222::Service* immediate_alert_service = nullptr;
	/** @brief Resolved Alert Notification Service instance. */
	c7222::Service* alert_notification_service = nullptr;
};

/** @brief Active AttributeServer pointer after the GATT database is enabled. */
static c7222::AttributeServer* g_att_server = nullptr;
/** @brief PWM object used to visualize the IAS Alert Level. */
static std::unique_ptr<c7222::PwmOut> g_alert_pwm;
/** @brief Event group used to move button IRQ events into the BLE task. */
static c7222::FreeRtosEventGroup g_event_group;
/** @brief FreeRTOS task object that runs the BLE application loop. */
static c7222::FreeRtosTask g_ble_task;

/** @brief GAP callback helper used by the example application flow. */
static GapEventHandler g_gap_event_handler;
/** @brief Security Manager callback helper used by the example flow. */
static SecurityEventHandler g_security_event_handler;


/**
 * @brief Global pointer to the on-board LED controller.
 */
static c7222::OnBoardLED* g_onboard_led = nullptr;

/**
 * @brief Global pointer to the on-board Button.
 */
static c7222::Button* g_button = nullptr;

/**
 * @brief Global pointer to board.
 */
c7222::PicoWBoard* g_board = nullptr;

/**
 * @brief Global pointer to platform.
 */
c7222::Platform* g_platform = nullptr;



/**
 * @brief Configure GAP advertising for the project device.
 *
 * This helper exists to isolate all advertising-related setup in one place.
 * In the example design it performs four jobs:
 * - acquires the GAP interface from the BLE facade,
 * - registers the shared GAP event handler,
 * - configures the advertising payload metadata such as flags and device
 *   name, and
 * - starts connectable advertising.
 *
 * The function is called from @ref OnBleStackOn so GAP operations start only
 * after the BLE stack reports that it is ready.
 */
void ConfigureAdvertisement() {
	// configure advertising.
	auto* ble = c7222::Ble::GetInstance();
	auto* gap = ble->GetGap();

	gap->AddEventHandler(g_gap_event_handler);

	// 3. Configure the advertising flags and device name.

	ble->SetAdvertisementFlags(
        c7222::AdvertisementData::Flags::kLeGeneralDiscoverableMode |
        c7222::AdvertisementData::Flags::kBrEdrNotSupported
	);
    ble->SetDeviceName(kDeviceName);
	
	// 4. Set advertising parameters.
    gap->SetAdvertisingParameters(c7222::Gap::AdvertisementParameters());
	// 5. Start advertising.
    gap->StartAdvertising();

    std::cout << "Advertising'" << kDeviceName << "'\n";
}

/**
 * @brief Enable and configure the Security Manager for the secured server.
 *
 * This helper exists because the project uses authenticated and encrypted
 * attributes. In the example flow it is called before
 * @ref c7222::Ble::EnableAttributeServer, which mirrors the secured-server
 * ordering introduced in Module 9.
 *
 * The function is responsible for:
 * - selecting the desired pairing and authentication policy,
 * - enabling the runtime Security Manager object,
 * - injecting that object into the shared security event handler, and
 * - registering the security event handler with the BLE facade.
 *
 * @param ble BLE facade used to access the Security Manager subsystem.
 */
void ConfigureSecurityManager(c7222::Ble* ble) {
	c7222::SecurityManager::SecurityParameters parameters;
	
	parameters.io_capability = c7222::SecurityManager::IoCapability::kDisplayOnly; // I/O must be picked explicitly - DisplayOnly suggested in course's plus.cs.aalto

	parameters.authentication = c7222::SecurityManager::AuthenticationRequirement::kSecureConnections | // not sure about this? It looked right
	                         c7222::SecurityManager::AuthenticationRequirement::kMitmProtection; // require authenticated pairing with MITM protection

	parameters.gatt_client_required_security_level = c7222::SecurityManager::GattClientSecurityLevel::kLevel4; // not sure about this level

	c7222::SecurityManager *SecurityManager = ble->EnableSecurityManager(parameters); // I think this works ?

	g_security_event_handler.SetSecurityManager(SecurityManager); // could just move ble->EnableSecurityManager(parameters) in here since it's only used once

	ble->AddSecurityEventHandler(&g_security_event_handler);
}

/**
 * @brief Resolve the service objects needed by the project from the parsed
 * GATT database.
 *
 * Once the Attribute Server has been enabled, the compiled profile can be
 * queried through service UUIDs. This helper keeps that lookup separate from
 * the rest of the application task so the initialization flow stays readable.
 *
 * In the reference design, the returned service pointers are then passed to
 * service-specific helper objects such as the IAS and ANS example classes.
 * Other designs may use the same resolved objects differently.
 *
 * @return Struct containing the resolved IAS and ANS service pointers.
 */
ProjectGattObjects ResolveGattObjects() {
	if (!g_att_server) {
		printf("!g_att_server");
		return ProjectGattObjects{};
	}

	auto* ias = g_att_server->FindServiceByUuid(c7222::Uuid(module10_ias_spec::kServiceUuid));
	if (!ias) {
		printf("!ans");
		return ProjectGattObjects{};
	}

	auto* ans = g_att_server->FindServiceByUuid(c7222::Uuid(module10_ans_spec::kServiceUuid)); 
	if (!ans) {
		printf("!ans");
		return ProjectGattObjects{};
	}

	
	return ProjectGattObjects{ias, ans};
}

/**
 * @brief Configure the board-side outputs and inputs used by the project.
 *
 * This helper groups the physical board wiring decisions into one function.
 * In the example flow it performs two jobs:
 * - creates the PWM output used by the Immediate Alert Service, and
 * - converts the Pico button interrupt into an event-group bit that the BLE
 *   task can process outside interrupt context.
 *
 * The intent is to keep hardware event capture separate from BLE protocol
 * handling.
 */
void ConfigureBoardOutputs() {
	g_board = g_platform->GetPicoWBoard();
	if (!g_board) {
		printf("!g_board"); 
		return;
	}

	g_button = &g_board->GetButton(c7222::PicoWBoard::ButtonId::BUTTON_B1);
	if (!g_button) {
		printf("!g_button"); 
		return;
	}

	g_onboard_led = c7222::OnBoardLED::GetInstance(); // good for debugging, but we also need PWM for the actual project
	if (!g_onboard_led->Initialize()) {
		printf("!g_onboard_led->Initialize()"); 
		return;
	}

	g_alert_pwm = g_platform->CreateLedPwm(c7222::PicoWBoard::LedId::LED1_GREEN, 0); // default to off (0-255; min-max)
	if (!g_alert_pwm) {
		printf("!g_alert_pwm"); 
		return;
	}

	g_button->EnableIrq(c7222::GpioInputEvent::BothEdges,
		[](uint32_t) {g_event_group.SetBitsFromISR(kButtonPressedEventMask);} //
	);
	
}

/**
 * @brief Convert a captured button event into ANS behavior.
 *
 * This helper exists to keep the BLE task loop small. In the example design
 * it first checks whether the device is currently connected, because the
 * project counts alerts per active connection. If connected, it forwards the
 * event to the ANS helper object.
 *
 * @param connected Current connection state reported by the GAP handler.
 * @param alert_notification_service Application object that applies ANS logic.
 */
void HandleButtonPress(bool connected, AlertNotificationService& alert_notification_service) {
	if (!connected) {return;} // return if not connected since unconnected presses shouldn't increment
	
	alert_notification_service.AddNewAlert(module10_ans_spec::Category::kSimpleAlert, ""); // add alert when HandleButtonPress is called
}

/**
 * @brief BLE-stack-ready callback used by the example startup flow.
 *
 * The BLE facade invokes this callback after the stack has turned on. The
 * example design uses it as the earliest safe place to configure and start
 * advertising.
 */
void OnBleStackOn() {
	ConfigureAdvertisement();
}

/**
 * @brief Main BLE application task for the reference design.
 *
 * This function demonstrates the full runtime order used by the example
 * application structure:
 * 1. acquire the BLE facade,
 * 2. enable security,
 * 3. enable the Attribute Server with the compiled profile,
 * 4. connect the GAP handler to the Attribute Server,
 * 5. configure board I/O,
 * 6. resolve the project services from the parsed GATT database,
 * 7. construct the application-side service objects,
 * 8. turn on the BLE stack, and
 * 9. run the main event loop.
 *
 * Inside the loop, the task has three responsibilities:
 * - observe connection-state transitions and notify the application objects,
 * - process button events delivered through the event group, and
 * - update the status LED while the device is not connected.
 *
 * @param params Unused FreeRTOS task parameter.
 */
[[noreturn]] void BleTask(void* /*params*/) {
	// 1. Get the BLE instance.
	auto* ble = c7222::Ble::GetInstance();
	if (ble == nullptr) {
		std::cout << "ble_app_task nullptr" << std::endl;
		while (true) {
			c7222::FreeRtosTask::Delay(c7222::FreeRtosTask::MsToTicks(1000)); // need to wait on certain devices tested, else startup fails
		}
	}

	// 2. enable security,
	ConfigureSecurityManager(ble); // CSM  enables the security

	// 3. enable the Attribute Server with the compiled profile
	g_att_server = ble->EnableAttributeServer(profile_data);

	// 4. connect the GAP handler to the Attribute Server,
	g_gap_event_handler.SetAttributeServer(g_att_server);


 	// 5. configure board I/O,
	ConfigureBoardOutputs();
 	
	// 6. resolve the project services from the parsed GATT database,
 	auto gattObj = ResolveGattObjects();
	
	// 7. construct the application-side service objects,
	// construct IAS
	ImmediateAlertService ias(gattObj.immediate_alert_service, g_alert_pwm.get()); // construct IAS
	// construct ANS
	AlertNotificationService ans(*gattObj.alert_notification_service);
	if (!ans.Initialize()) {
		assert("Failed to initialize ANS\n");
	}

	// attach IAS & ANS to g_gap_event_handler
	g_gap_event_handler.SetImmediateAlertService(&ias);
	g_gap_event_handler.SetAlertNotificationService(&ans);


 	// * 8. turn on the BLE stack
	ble->SetOnBleStackOnCallback(OnBleStackOn); // 8_4 & 8_5 call ConfigureAdvertising directly. Not sure what benefit there is to using OnBleStackOn to just call ConfigureAdvertising
	ble->TurnOn();

	auto* gap = ble->GetGap();

	// Inside the loop, the task has three responsibilities:
 	// observe connection-state transitions and notify the application objects,
 	// process button events delivered through the event group, and
 	// - update the status LED while the device is not connected.

	while(true) {
		if (gap->IsAdvertisingEnabled()){
            g_onboard_led->Toggle();
        } else if (g_att_server->IsConnected()) {
            g_onboard_led->On();
        } else {
            g_onboard_led->Off();
        }

		uint32_t button_event = g_event_group.WaitBits(kButtonPressedEventMask, // bits_to_wait_for – Target bits.
									true, // If true, clear requested bits before return.
									false, // I don't think this matters, but I set it to false in case that has better responsiveness. If true, wait for all bits; otherwise any bit.
									100); // I think this is correct. -  ticks_to_wait – Max ticks to wait. 


		if (button_event & kButtonPressedEventMask) {
			HandleButtonPress(g_att_server->IsConnected(), ans);
		}

		c7222::FreeRtosTask::Delay(c7222::FreeRtosTask::MsToTicks(250));
	}
	
}

}  // namespace

/**
 * @brief Program entry point for the example project application.
 *
 * This function is intentionally small. Its purpose is to perform the minimum
 * platform startup needed before the scheduler takes control:
 * - initialize the Pico platform abstraction,
 * - create the BLE application task, and
 * - start the FreeRTOS scheduler.
 *
 * After the scheduler starts, the ongoing BLE behavior is driven by
 * @ref BleTask and the registered callback handlers.
 *
 * @return This function never returns.
 */
[[noreturn]] int main() {
	// initialize the system and start the BLE task.

	g_platform = c7222::Platform::GetInstance();

	if (!g_platform->Initialize()) {
        assert("Failed to initialize platform");
    }

    std::printf("Platform initialized.\n");

	// 5. Create the BLE application task.
    if (!g_ble_task.Initialize(
        "BLE_Task",
        1024,
        c7222::FreeRtosTask::IdlePriority() + 1,
        BleTask,
        nullptr)) {
			assert("Failed to initialize BLE Task");
		}

	// 6. Start the scheduler.
    c7222::FreeRtosTask::StartScheduler();

	
	while(true) {}
	
}
