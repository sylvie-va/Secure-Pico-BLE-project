/**
 * @file immediate_alert_service.hpp
 * @brief Shared Immediate Alert Service values and one reference class design.
 *
 * DISCLAIMER:
 * This file is a task template for ELEC C7222 Module 10 Task 10.1.
 * The example class-based solution is provided only as a reference design.
 */
#pragma once

#include <cstdint>
#include <vector>

#include "characteristic.hpp"
#include "pwm.hpp"
#include "service.hpp"

/**
 * @brief Shared Immediate Alert Service values available to all solutions.
 *
 * These constants and enum values are kept visible even when the reference
 * class-based design is hidden. They allow implementations to use the correct
 * Bluetooth-assigned values without copying raw numbers from the
 * specification.
 */
namespace module10_ias_spec {

/** @brief Immediate Alert Service UUID. */
inline constexpr uint16_t kServiceUuid = 0x1802;
/** @brief Alert Level characteristic UUID. */
inline constexpr uint16_t kAlertLevelUuid = 0x2A06;

/**
 * @brief Standard IAS Alert Level values.
 *
 * The numeric values match the Bluetooth specification, so they can be used
 * directly in characteristic payload handling.
 */
enum class AlertLevel : uint8_t {
	kNoAlert = 0,  /**< Disable the alert output. */
	kMildAlert = 1,  /**< Apply a low-intensity alert output. */
	kHighAlert = 2,  /**< Apply a high-intensity alert output. */
};

}  // namespace module10_ias_spec

/**
 * @brief Forward declaration for the reference IAS helper design.
 *
 * The full class declaration is kept in the solution section because this
 * object-oriented decomposition is an example, not a required project
 * architecture.
 */
class ImmediateAlertService {
	public:
		/** @brief IAS Builder. */
		explicit ImmediateAlertService(c7222::Service* service);

		/** @brief reset alert level to NoAlert when called */
		void reset();

	private:
		// set IAS alert level
		void set_alert_level(module10_ias_spec::AlertLevel level);

		// IAS alert level
		c7222::Characteristic* alert_level_;

		// current IAS alert level
		module10_ias_spec::AlertLevel current_level_ = module10_ias_spec::AlertLevel::kNoAlert;
};