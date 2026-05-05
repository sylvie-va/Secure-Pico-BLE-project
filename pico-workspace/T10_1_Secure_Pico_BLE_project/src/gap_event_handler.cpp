/**
 * @file gap_event_handler.cpp
 * @brief Implementation file for the GAP callback logic used by the project.
 *
 * DISCLAIMER:
 * This file is a task template for ELEC C7222 Module 10 Task 10.1.
 * It contains task-specific TODO notes that indicate the required
 * implementation points.
 *
 * This file is responsible for the application-side reaction to GAP events.
 * In the project, that means implementing logic that:
 * - stores the AttributeServer pointer once it becomes available,
 * - updates the current connection state when a link is established,
 * - propagates the active connection handle to the AttributeServer, and
 * - restores the advertising state after a disconnect.
 *
 * The project may organize this logic differently, but the same runtime
 * behavior is still required.
 */

#include "gap_event_handler.hpp"

#include <cstdio>

#include "platform.hpp"

GapEventHandler::GapEventHandler(c7222::Gap* gap, c7222::AttributeServer* attribute_server)
	: gap_(gap), attribute_server_(attribute_server) {}

void GapEventHandler::SetAttributeServer(c7222::AttributeServer* attribute_server) {
	// store the AttributeServer pointer for later connection handling.
	attribute_server_ = attribute_server;
}

void GapEventHandler::OnConnectionComplete(uint8_t status,
										   c7222::ConnectionHandle con_handle,
										   const c7222::BleAddress& address,
										   uint16_t conn_interval,
										   uint16_t conn_latency,
										   uint16_t supervision_timeout) const {
	(void)address;
	std::printf(
		"GAP event: ConnectionComplete (status=0x%02X, handle=%u, interval=%u, latency=%u, timeout=%u)\n",
		status,
		con_handle,
		conn_interval,
		conn_latency,
		supervision_timeout); // not used in project, but good for logging

	// if an AttributeServer exists, propagate the active connection handle
	// to it so GATT operations can use the current connection.
	
	if (status == 0 && attribute_server_ != nullptr) {
		attribute_server_ -> SetConnectionHandle(con_handle);
		connected_ = true;
		std::printf("Connected\n"); // not used in project, but good for logging
	} else {
		connected_ = false;
	}
}

void GapEventHandler::OnDisconnectionComplete(uint8_t status,
											  c7222::ConnectionHandle con_handle,
											  uint8_t reason) const {
	std::printf("GAP event: DisconnectionComplete (status=0x%02X, handle=%u, reason=0x%02X)\n",
				status,
				con_handle,
				reason); // not used in project, but good for logging

	// restart advertising through GAP so the device becomes discoverable again.
	connected_ = false;
	
	if (attribute_server_ != nullptr) {
		attribute_server_->SetDisconnected();
	}

	if (gap_ != nullptr) {
        gap_ -> StartAdvertising();
        std::printf("Advertising restart\n"); // not used in project, but good for logging
    }
	
}
