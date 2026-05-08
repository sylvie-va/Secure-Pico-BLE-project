/**
 * @file immediate_alert_service.cpp
 * @brief Reference implementation file for one possible Immediate Alert
 * Service design.
 *
 * DISCLAIMER:
 * This file is a task template for ELEC C7222 Module 10 Task 10.1.
 * It contains task-specific TODO notes that indicate the required
 * implementation points.
 *
 * This file documents one possible design for the Immediate Alert Service
 * component of the project. The main idea is to encapsulate all IAS-specific
 * logic into one class that owns the service object and its characteristics.
 *
 * An implementation corresponding to this file is expected to handle the
 * following responsibilities:
 * - resolve the Alert Level characteristic from the parsed IAS service,
 * - attach the write callback used for client writes,
 * - validate the written Alert Level value,
 * - map the accepted values to the required PWM duty cycles,
 * - update the stored characteristic value when the alert level changes, and
 * - reset the IAS-visible state on connection and disconnection when the
 *   chosen design requires it.
 *
 * The project does not require these responsibilities to be implemented as
 * member functions of an `ImmediateAlertService` class. The example class
 * design is only one possible organization of that behavior.
 */

#include "immediate_alert_service.hpp"

#include <cassert>
#include <cstdio>

// The example class-based IAS implementation is provided as a reference
// solution only. Students may implement the project using another structure.

ImmediateAlertService::ImmediateAlertService(c7222::Service* service, c7222::PwmOut* pwm) : alert_level_(nullptr), pwm_(pwm){
    // above the constructor initializes the alert_level_ pointer to nullptr and stores the pwm pointer for later use.

    assert(service != nullptr);
    assert(pwm != nullptr);

    /* that service pointer is expected to point to a valid Service object that contains the Alert Level characteristic parsed from the ATT DB.
    The constructor should resolve the Alert Level characteristic and store a pointer to it in alert_level_.
    */

    alert_level_ = service->FindCharacteristicByUuid(c7222::Uuid(module10_ias_spec::kAlertLevelUuid));
    // now the alert level pointer points to the services alert level characteristic, which is where the client writes will be received.

    assert(alert_level_ != nullptr);
    // confirms that the alert level characteristic was found in the service.

    alert_level_->AddEventHandler(*this);
    set_alert_level(module10_ias_spec::AlertLevel::kNoAlert);
    // binds the class to an even handler and then setst the alert to no alert(aka led off).

}

void ImmediateAlertService::reset(){
    set_alert_level(module10_ias_spec::AlertLevel::kNoAlert);
    // used after disconnect to reset alert level.
}

void ImmediateAlertService::OnWrite(const std::vector<uint8_t>& data){
    if (data.empty()){
        return;
    }
    
    const uint8_t rawVal = data[0];
    if (rawVal > static_cast<uint8_t>(module10_ias_spec::AlertLevel::kHighAlert))
    {
        return;
    }
    // if the value given is larger than high alert it is ignored.

    set_alert_level(static_cast<module10_ias_spec::AlertLevel>(rawVal)); 
}

void ImmediateAlertService::set_alert_level(module10_ias_spec::AlertLevel level){
    // checks that the thing wont die to a nullptr
    if(alert_level_ == nullptr || pwm_ == nullptr){
        return;
    }
    
    // makes sure the level is not the same as the level it would be changed to (to avoid redundant work)
    if(current_level_ == level) {
        return;
    }

    // good ole switch case!
    switch (level) {
        case module10_ias_spec::AlertLevel::kNoAlert:
            pwm_->SetDutyCycle(0.0f);
            break;
        case module10_ias_spec::AlertLevel::kMildAlert:
            pwm_->SetDutyCycle(0.25f);
            break;
        case module10_ias_spec::AlertLevel::kHighAlert:
            pwm_->SetDutyCycle(0.90f);
            break;
    }
}