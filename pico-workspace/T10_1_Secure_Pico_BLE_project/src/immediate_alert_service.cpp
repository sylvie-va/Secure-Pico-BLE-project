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

ImmediateAlertService::ImmediateAlertService(c7222::Service* service) : 
    alert_level_(c7222::Uuid(module10_ias_spec::kAlertLevelUuid), static_cast<uint8_t>(c7222::Characteristic::Properties::kWriteWithoutResponse),0x0002,0x0003){
        /* 
        the above should create the service with the characteristic alert_level which has the Uuid of kAlertLevelUuid, 
        the property of kWriteWithoutResponse as dictated by the IAS HTML at https://www.bluetooth.com/specifications/specs/immediate-alert-service-1-0/,
        and the ATT handles 2 and 3.
        */

        // this here sets the actual value of alert_level_ to no alert
        alert_level_.SetValue(module10_ias_spec::AlertLevel::kNoAlert);
        service->AddCharacteristic(alert_level_);


    
    



}
