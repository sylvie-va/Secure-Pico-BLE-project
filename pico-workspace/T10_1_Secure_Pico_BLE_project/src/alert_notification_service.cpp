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

using namespace module10_ans_spec;

AlertNotificationService::AlertNotificationService(
    c7222::Service& service)
    : service_(service) {
}

bool AlertNotificationService::Initialize(){ 
    if (!ResolveCharacteristics()){
        return false;
    }
    InitializeSupportedCategories();
    unread_counts_.fill(0);
    enabled_new_alert_categories_ = 0;
    enabled_unread_categories_ = 0;
    if (control_point_){
        control_point_->AddEventHandler(*this);
    }

    return true;
}

bool AlertNotificationService::ResolveCharacteristics() {
    supported_new_alert_category_ =
        service_.FindCharacteristicByUuid(
            c7222::Uuid(kSupportedNewAlertCategoryUuid));
    new_alert_ =
        service_.FindCharacteristicByUuid(
            c7222::Uuid(kNewAlertUuid));
    supported_unread_alert_category_ =
        service_.FindCharacteristicByUuid(
            c7222::Uuid(kSupportedUnreadAlertCategoryUuid));

    unread_alert_status_ =
        service_.FindCharacteristicByUuid(
            c7222::Uuid(kUnreadAlertStatusUuid));

    control_point_ =
        service_.FindCharacteristicByUuid(
            c7222::Uuid(kAlertNotificationControlPointUuid));

    return supported_new_alert_category_ &&
           new_alert_ &&
           supported_unread_alert_category_ &&
           unread_alert_status_ &&
           control_point_;
}
void AlertNotificationService::InitializeSupportedCategories() {
    uint16_t mask = kAllAlertMask;

    std::vector<uint8_t> value = {
        static_cast<uint8_t>(mask & 0xFF),
        static_cast<uint8_t>(mask >> 8)
    };
    supported_new_alert_category_->SetValue(value);
    supported_unread_alert_category_->SetValue(value);
}
void AlertNotificationService::AddNewAlert(
    Category category,
    const std::string& message) {

    size_t i = static_cast<size_t>(category);
    if (i >= unread_counts_.size()){
        return;
    }

    if (unread_counts_[i] < 255){
        unread_counts_[i]++;
    }

    NotifyNewAlert(category, message);
    NotifyUnreadStatus(category);
}

void AlertNotificationService::SetUnreadCount(
    Category category,
    uint8_t count) {

    size_t i = static_cast<size_t>(category);
    if (i >= unread_counts_.size()){
        return;
    }
    unread_counts_[i] = count;
}

void AlertNotificationService::OnWrite(
    const std::vector<uint8_t>& data) {
    HandleControlPointCommand(data);
}

void AlertNotificationService::HandleControlPointCommand(
    const std::vector<uint8_t>& data){
    if (data.size() < 2){
        return;
    }
    Command cmd = static_cast<Command>(data[0]);
    Category cat = static_cast<Category>(data[1]);

    if (static_cast<size_t>(cat) >= module10_ans_spec::kAlertCategoryCount &&
        cat != Category::kAllAlerts) {
        return;
    }
    uint16_t bit = CategoryToMask(cat);
    switch (cmd) {
        case Command::kEnableNewIncomingAlertNotification:
            enabled_new_alert_categories_ |= bit;
            break;

        case Command::kDisableNewIncomingAlertNotification:
            enabled_new_alert_categories_ &= ~bit;
            break;

        case Command::kEnableUnreadCategoryStatusNotification:
            enabled_unread_categories_ |= bit;
            break;

        case Command::kDisableUnreadCategoryStatusNotification:
            enabled_unread_categories_ &= ~bit;
            break;

        case Command::kNotifyNewIncomingAlertImmediately:
            NotifyNewAlert(cat, "");
            break;

        case Command::kNotifyUnreadCategoryStatusImmediately:
            NotifyUnreadStatus(cat);
            break;

        default:
            break;
    }
}

uint16_t AlertNotificationService::CategoryToMask(
    Category category) const {

    if (category == Category::kAllAlerts) return kAllAlertMask;
    return 1u << static_cast<uint8_t>(category);
}

std::vector<uint8_t>
AlertNotificationService::BuildNewAlertPayload(
    Category category,
    const std::string& message) const {

    size_t i = static_cast<size_t>(category);

    std::vector<uint8_t> p;
    p.push_back(static_cast<uint8_t>(category));
    p.push_back(unread_counts_[i]);
    p.insert(p.end(), message.begin(), message.end());

    return p;
}

std::vector<uint8_t>
AlertNotificationService::BuildUnreadStatusPayload(
    Category category) const {

    size_t i = static_cast<size_t>(category);

    return {
        static_cast<uint8_t>(category),
        unread_counts_[i]
    };
}
void AlertNotificationService::SendNotification(
    c7222::Characteristic& ch,
    const std::vector<uint8_t>& payload) {

    ch.SetValue(payload);
}

void AlertNotificationService::ResetClientConfiguration() {
    enabled_new_alert_categories_ = 0;
    enabled_unread_categories_ = 0;
    unread_counts_.fill(0);
}

void AlertNotificationService::NotifyNewAlert(module10_ans_spec::Category category, const std::string& message) {
    if (IsCategoryEnabled(enabled_new_alert_categories_, category) && new_alert_->IsNotificationsEnabled()) {
        auto payload = BuildNewAlertPayload(category, message);
        SendNotification(*new_alert_, payload);
    }
}

void AlertNotificationService::NotifyUnreadStatus(module10_ans_spec::Category category) {
    if (IsCategoryEnabled(enabled_unread_categories_, category) &&
        unread_alert_status_->IsNotificationsEnabled()) {
        auto payload = BuildUnreadStatusPayload(category);
        SendNotification(*unread_alert_status_, payload);
    }
}

void AlertNotificationService::EnableCategory(uint16_t& mask,
                                             module10_ans_spec::Category category) {
    mask |= CategoryToMask(category); // enable bits
}

void AlertNotificationService::DisableCategory(uint16_t& mask,
                                              module10_ans_spec::Category category) {
    mask &= ~CategoryToMask(category); // disable bits
}

bool AlertNotificationService::IsCategoryEnabled(uint16_t mask,
                                                module10_ans_spec::Category category) const {
    return (mask & CategoryToMask(category)); // check if bits enabled
}