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


