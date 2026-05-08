/**
 * @file security_event_handler.cpp
 * @brief Implementation file for the Security Manager callback logic used by
 * the project.
 *
 * DISCLAIMER:
 * This file is a task template for ELEC C7222 Module 10 Task 10.1.
 * It contains task-specific TODO notes that indicate the required
 * implementation points.
 *
 * This file is responsible for the application-side reaction to Security
 * Manager events. In the project, that means implementing logic that:
 * - logs or otherwise reports the pairing flow,
 * - confirms the supported pairing procedures when user interaction is
 *   required,
 * - reacts to pairing completion and re-encryption completion, and
 * - grants or denies authorization according to the chosen project policy.
 *
 * The project may organize this logic differently, but the same runtime
 * security behavior is still required.
 */

#include <stdio.h>
#include <string.h>

#include "security_event_handler.hpp"

#define SM_POINTER_CHECK if (security_manager_ == nullptr) {std::printf("[Security] Security Manager is null\n"); return;}

// log the pairing flow and confirm the minimal pairing/authorization steps used by the BLE server.

void SecurityEventHandler::OnJustWorksRequest(c7222::ConnectionHandle connection_handle) const {
    std::printf("[Security] OnJustWorksRequest: handle=0x%04x\n", connection_handle);

    SM_POINTER_CHECK

    security_manager_->ConfirmJustWorks(connection_handle);
}

void SecurityEventHandler::OnNumericComparisonRequest(c7222::ConnectionHandle connection_handle, uint32_t numeric_value) const {
    std::printf("[Security] OnNumericComparisonRequest: handle=0x%04x, numeric_value=%lu\n", connection_handle, numeric_value);
    
    SM_POINTER_CHECK

    security_manager_->ConfirmNumericComparison(connection_handle, true); // true = accept
}

void SecurityEventHandler::OnPasskeyDisplay(c7222::ConnectionHandle connection_handle, uint32_t passkey) const {
	std::printf("[Security] OnPasskeyDisplay: handle=0x%04x, passkey=%06lu\n", connection_handle, passkey);
}

void SecurityEventHandler::OnPasskeyInput(c7222::ConnectionHandle connection_handle) const {
	std::printf("[Security] OnPasskeyInput: handle=0x%04x\n", connection_handle);
    SM_POINTER_CHECK
    uint32_t fixed_passkey = 123456; // example implementation sets fixed passkey of 123456
    security_manager_->ProvidePasskey(connection_handle, fixed_passkey); 
}

void SecurityEventHandler::OnPairingComplete(c7222::ConnectionHandle connection_handle,
                                            c7222::SecurityManager::PairingStatus status,
                                            uint8_t status_code) const {
    std::string status_string = "Unknown"; // default string

    // convert pairing status into something human readable
	switch (status) {
	case c7222::SecurityManager::PairingStatus::kSuccess:
		status_string = "Success";
		break;
	case c7222::SecurityManager::PairingStatus::kFailed:
		status_string = "Failed";
		break;
	case c7222::SecurityManager::PairingStatus::kTimeout:
		status_string = "Timeout";
		break;
	case c7222::SecurityManager::PairingStatus::kUnsupported:
		status_string = "Unsupported";
		break;
	case c7222::SecurityManager::PairingStatus::kUnknown:
		status_string = "Unknown";
		break;
	}

	std::printf("[Security] OnPairingComplete: handle=0x%04x, status=%s, code=0x%02x\n", connection_handle, status_string, status_code);
}

void SecurityEventHandler::OnReencryptionComplete(c7222::ConnectionHandle connection_handle, uint8_t status_code) const {
    std::string status_string = (status_code==0) ? "Success" : "Fail"; // convert into human readable text
	std::printf("[Security] OnReencryptionComplete: handle=0x%04x status=%s\n", connection_handle, status_string);
}

void SecurityEventHandler::OnAuthorizationRequest(c7222::ConnectionHandle connection_handle) const {
	std::printf("[Security] OnAuthorizationRequest: handle=0x%04x\n", connection_handle);
    SM_POINTER_CHECK
    security_manager_->SetAuthorization(connection_handle, c7222::SecurityManager::AuthorizationResult::kGranted);
}

void SecurityEventHandler::OnAuthorizationResult(c7222::ConnectionHandle connection_handle, c7222::SecurityManager::AuthorizationResult result) const {
    std::string result_string = (result == c7222::SecurityManager::AuthorizationResult::kGranted) ? "Granted" : "Denied"; // convert into human readable text
	std::printf("[Security] OnAuthorizationResult: handle=0x%04x, result=%s\n", connection_handle, result_string);
}