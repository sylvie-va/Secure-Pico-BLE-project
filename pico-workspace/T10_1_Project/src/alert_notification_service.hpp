/**
 * @file alert_notification_service.hpp
 * @brief Shared Alert Notification Service values and one reference class
 * design.
 *
 * DISCLAIMER:
 * This file is a task template for ELEC C7222 Module 10 Task 10.1.
 * The example class-based solution is provided only as a reference design.
 */
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "characteristic.hpp"
#include "service.hpp"

/**
 * @brief Shared Alert Notification Service values available to all solutions.
 *
 * These constants and enums are kept visible even when the reference
 * class-based design is hidden. They allow implementations to use the correct
 * Bluetooth-assigned values for UUIDs, category identifiers, and Control
 * Point commands without copying raw values from the specification.
 */
namespace module10_ans_spec {

/** @brief Number of standard ANS alert categories. */
inline constexpr std::size_t kAlertCategoryCount = 10;
/** @brief Alert Notification Service UUID. */
inline constexpr uint16_t kServiceUuid = 0x1811;
/** @brief UUID of the Supported New Alert Category characteristic. */
inline constexpr uint16_t kSupportedNewAlertCategoryUuid = 0x2A47;
/** @brief UUID of the New Alert characteristic. */
inline constexpr uint16_t kNewAlertUuid = 0x2A46;
/** @brief UUID of the Supported Unread Alert Category characteristic. */
inline constexpr uint16_t kSupportedUnreadAlertCategoryUuid = 0x2A48;
/** @brief UUID of the Unread Alert Status characteristic. */
inline constexpr uint16_t kUnreadAlertStatusUuid = 0x2A45;
/** @brief UUID of the Alert Notification Control Point characteristic. */
inline constexpr uint16_t kAlertNotificationControlPointUuid = 0x2A44;
/** @brief Bit mask for the Simple Alert category. */
inline constexpr uint16_t kSimpleAlertMask = (1u << 0);
/** @brief Bit mask that selects all ten standard ANS categories. */
inline constexpr uint16_t kAllAlertMask = 0x03FF;

/**
 * @brief ANS category identifiers.
 *
 * The values match the standard ANS category numbering so they can be used
 * directly in characteristic payloads and local bit masks.
 */
enum class Category : uint8_t {
	kSimpleAlert = 0,  /**< General alert category. */
	kEmail = 1,  /**< Email alert category. */
	kNews = 2,  /**< News alert category. */
	kCall = 3,  /**< Incoming call alert category. */
	kMissedCall = 4,  /**< Missed call alert category. */
	kSmsMms = 5,  /**< SMS or MMS alert category. */
	kVoiceMail = 6,  /**< Voice mail alert category. */
	kSchedule = 7,  /**< Calendar or schedule alert category. */
	kHighPriorityAlert = 8,  /**< High-priority alert category. */
	kInstantMessage = 9,  /**< Instant message alert category. */
	kAllAlerts = 0xFF,  /**< Special selector for all categories. */
};

/**
 * @brief ANS Control Point commands.
 *
 * The values match the standard Control Point opcodes and can be used when
 * parsing client writes to the Alert Notification Control Point.
 */
enum class Command : uint8_t {
	kEnableNewIncomingAlertNotification = 0,
	kEnableUnreadCategoryStatusNotification = 1,
	kDisableNewIncomingAlertNotification = 2,
	kDisableUnreadCategoryStatusNotification = 3,
	kNotifyNewIncomingAlertImmediately = 4,
	kNotifyUnreadCategoryStatusImmediately = 5,
};

}  // namespace module10_ans_spec

/**
 * @brief Forward declaration for the reference ANS helper design.
 *
 * The full class declaration is kept in the solution section because this
 * object-oriented decomposition is an example, not a required project
 * architecture.
 */
class AlertNotificationService
    : public c7222::Characteristic::EventHandler {

public:
    explicit AlertNotificationService(c7222::Service& service);

    bool Initialize();

    void AddNewAlert(module10_ans_spec::Category category,
                     const std::string& message);

    void SetUnreadCount(module10_ans_spec::Category category,
                        uint8_t count);

    void ResetClientConfiguration();

    void OnWrite(const std::vector<uint8_t>& data) override;

private:
    c7222::Service& service_;

    c7222::Characteristic* supported_new_alert_category_ = nullptr;
    c7222::Characteristic* new_alert_ = nullptr;
    c7222::Characteristic* supported_unread_alert_category_ = nullptr;
    c7222::Characteristic* unread_alert_status_ = nullptr;
    c7222::Characteristic* control_point_ = nullptr;

    uint16_t enabled_new_alert_categories_ = 0;
    uint16_t enabled_unread_categories_ = 0;

    std::array<uint8_t, module10_ans_spec::kAlertCategoryCount>
        unread_counts_{};

private:
    bool ResolveCharacteristics();

    void InitializeSupportedCategories();

    void NotifyNewAlert(module10_ans_spec::Category category,
                        const std::string& message);

    void NotifyUnreadStatus(module10_ans_spec::Category category);

    void HandleControlPointCommand(const std::vector<uint8_t>& data);

    void EnableCategory(uint16_t& mask,
                        module10_ans_spec::Category category);

    void DisableCategory(uint16_t& mask,
                         module10_ans_spec::Category category);

    bool IsCategoryEnabled(uint16_t mask,
                           module10_ans_spec::Category category) const;

    uint16_t CategoryToMask(module10_ans_spec::Category category) const;

    std::vector<uint8_t> BuildNewAlertPayload(
        module10_ans_spec::Category category,
        const std::string& message) const;

    std::vector<uint8_t> BuildUnreadStatusPayload(
        module10_ans_spec::Category category) const;

    void SendNotification(c7222::Characteristic& characteristic,
                          const std::vector<uint8_t>& payload);
};


