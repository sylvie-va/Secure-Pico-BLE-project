/**
 * @file alert_notification_service.cpp
 * @brief Reference implementation file for one possible Alert Notification
 * Service design.
 *
 * DISCLAIMER:
 * This file is a task template for ELEC C7222 Module 10 Task 10.1.
 * It contains task-specific TODO notes that indicate the required
 * implementation points.
 *
 * This file documents one possible design for the Alert Notification Service
 * component of the project. The main idea is to encapsulate all ANS-specific
 * logic into one class that owns the service object and its characteristics.
 *
 * An implementation corresponding to this file is expected to handle the
 * following responsibilities:
 * - resolve the ANS characteristics required by the project from the parsed
 *   service object,
 * - initialize the supported-category values exposed by the server,
 * - track unread counts and client-enabled category masks during a
 *   connection,
 * - react to Control Point writes and CCCD updates,
 * - generate the correct New Alert and Unread Alert Status payloads, and
 * - update or notify the corresponding characteristics when button-generated
 *   alerts occur.
 *
 * The project does not require these responsibilities to be implemented as
 * member functions of an `AlertNotificationService` class. The example class
 * design is only one possible organization of that behavior.
 */

#include "alert_notification_service.hpp"

#include <cassert>
#include <cstdio>

// The example class-based ANS implementation is provided as a reference
// solution only. Students may implement the project using another structure.


