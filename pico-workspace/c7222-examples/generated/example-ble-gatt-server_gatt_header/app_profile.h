
// clang-format off
// /home/sylvie/Documents/University/embedded-cpp-2026/pico-workspace/pico-workspace/c7222-development/build/generated/example-ble-gatt-server_gatt_header/app_profile.h generated from /home/sylvie/Documents/University/embedded-cpp-2026/pico-workspace/pico-workspace/c7222-development/libs/elec_c7222/examples/ble/gatt-server/app_profile.gatt for BTstack
// it needs to be regenerated when the .gatt file is updated. 

// To generate /home/sylvie/Documents/University/embedded-cpp-2026/pico-workspace/pico-workspace/c7222-development/build/generated/example-ble-gatt-server_gatt_header/app_profile.h:
// /home/sylvie/.pico-sdk/sdk/2.2.0/lib/btstack/tool/compile_gatt.py /home/sylvie/Documents/University/embedded-cpp-2026/pico-workspace/pico-workspace/c7222-development/libs/elec_c7222/examples/ble/gatt-server/app_profile.gatt /home/sylvie/Documents/University/embedded-cpp-2026/pico-workspace/pico-workspace/c7222-development/build/generated/example-ble-gatt-server_gatt_header/app_profile.h

// att db format version 1

// binary attribute representation:
// - size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

#include <stdint.h>

// Reference: https://en.cppreference.com/w/cpp/feature_test
#if __cplusplus >= 200704L
constexpr
#endif
const uint8_t profile_data[] =
{
    // ATT DB Version
    1,

    // 0x0001 PRIMARY_SERVICE-GAP_SERVICE
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18, 
    // 0x0002 CHARACTERISTIC-GAP_DEVICE_NAME - READ
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x02, 0x03, 0x00, 0x00, 0x2a, 
    // 0x0003 VALUE CHARACTERISTIC-GAP_DEVICE_NAME - READ -'picow_temp'
    // READ_ANYBODY
    0x12, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x2a, 0x70, 0x69, 0x63, 0x6f, 0x77, 0x5f, 0x74, 0x65, 0x6d, 0x70, 
    // 0x0004 PRIMARY_SERVICE-GATT_SERVICE
    0x0a, 0x00, 0x02, 0x00, 0x04, 0x00, 0x00, 0x28, 0x01, 0x18, 
    // 0x0005 CHARACTERISTIC-GATT_DATABASE_HASH - READ
    0x0d, 0x00, 0x02, 0x00, 0x05, 0x00, 0x03, 0x28, 0x02, 0x06, 0x00, 0x2a, 0x2b, 
    // 0x0006 VALUE CHARACTERISTIC-GATT_DATABASE_HASH - READ -''
    // READ_ANYBODY
    0x18, 0x00, 0x02, 0x00, 0x06, 0x00, 0x2a, 0x2b, 0x97, 0xa3, 0x34, 0x81, 0xdc, 0x06, 0x25, 0x6e, 0x4f, 0xcf, 0x30, 0x11, 0x96, 0xd4, 0x37, 0x21, 
    // 0x0007 PRIMARY_SERVICE-ORG_BLUETOOTH_SERVICE_ENVIRONMENTAL_SENSING
    0x0a, 0x00, 0x02, 0x00, 0x07, 0x00, 0x00, 0x28, 0x1a, 0x18, 
    // 0x0008 CHARACTERISTIC-fc930f88-1a30-45d7-8c17-604c1a036b9f - DYNAMIC | READ | WRITE | READ_AUTHENTICATED | READ_ENCRYPTED | WRITE_AUTHENTICATED | WRITE_ENCRYPTED
    0x1b, 0x00, 0x02, 0x00, 0x08, 0x00, 0x03, 0x28, 0x0a, 0x09, 0x00, 0x9f, 0x6b, 0x03, 0x1a, 0x4c, 0x60, 0x17, 0x8c, 0xd7, 0x45, 0x30, 0x1a, 0x88, 0x0f, 0x93, 0xfc, 
    // 0x0009 VALUE CHARACTERISTIC-fc930f88-1a30-45d7-8c17-604c1a036b9f - DYNAMIC | READ | WRITE | READ_AUTHENTICATED | READ_ENCRYPTED | WRITE_AUTHENTICATED | WRITE_ENCRYPTED
    // READ_AUTHENTICATED, WRITE_AUTHENTICATED
    0x16, 0x00, 0x1a, 0x0b, 0x09, 0x00, 0x9f, 0x6b, 0x03, 0x1a, 0x4c, 0x60, 0x17, 0x8c, 0xd7, 0x45, 0x30, 0x1a, 0x88, 0x0f, 0x93, 0xfc, 
    // 0x000a USER_DESCRIPTION-READ
    // READ_ANYBODY, WRITE_ANYBODY
    0x08, 0x00, 0x0a, 0x01, 0x0a, 0x00, 0x01, 0x29, 
    // 0x000b CHARACTERISTIC-ORG_BLUETOOTH_CHARACTERISTIC_TEMPERATURE - READ | NOTIFY | INDICATE | DYNAMIC | READ_AUTHENTICATED | READ_ENCRYPTED
    0x0d, 0x00, 0x02, 0x00, 0x0b, 0x00, 0x03, 0x28, 0x32, 0x0c, 0x00, 0x6e, 0x2a, 
    // 0x000c VALUE CHARACTERISTIC-ORG_BLUETOOTH_CHARACTERISTIC_TEMPERATURE - READ | NOTIFY | INDICATE | DYNAMIC | READ_AUTHENTICATED | READ_ENCRYPTED
    // READ_AUTHENTICATED
    0x08, 0x00, 0x02, 0x09, 0x0c, 0x00, 0x6e, 0x2a, 
    // 0x000d CLIENT_CHARACTERISTIC_CONFIGURATION
    // READ_ANYBODY, WRITE_ANYBODY
    0x0a, 0x00, 0x0e, 0x01, 0x0d, 0x00, 0x02, 0x29, 0x00, 0x00, 
    // 0x000e USER_DESCRIPTION-READ
    // READ_ANYBODY, WRITE_ANYBODY
    0x08, 0x00, 0x0a, 0x01, 0x0e, 0x00, 0x01, 0x29, 
    // END
    0x00, 0x00, 
}; // total size 98 bytes 


//
// list service handle ranges
//
#define ATT_SERVICE_GAP_SERVICE_START_HANDLE 0x0001
#define ATT_SERVICE_GAP_SERVICE_END_HANDLE 0x0003
#define ATT_SERVICE_GAP_SERVICE_01_START_HANDLE 0x0001
#define ATT_SERVICE_GAP_SERVICE_01_END_HANDLE 0x0003
#define ATT_SERVICE_GATT_SERVICE_START_HANDLE 0x0004
#define ATT_SERVICE_GATT_SERVICE_END_HANDLE 0x0006
#define ATT_SERVICE_GATT_SERVICE_01_START_HANDLE 0x0004
#define ATT_SERVICE_GATT_SERVICE_01_END_HANDLE 0x0006
#define ATT_SERVICE_ORG_BLUETOOTH_SERVICE_ENVIRONMENTAL_SENSING_START_HANDLE 0x0007
#define ATT_SERVICE_ORG_BLUETOOTH_SERVICE_ENVIRONMENTAL_SENSING_END_HANDLE 0x000e
#define ATT_SERVICE_ORG_BLUETOOTH_SERVICE_ENVIRONMENTAL_SENSING_01_START_HANDLE 0x0007
#define ATT_SERVICE_ORG_BLUETOOTH_SERVICE_ENVIRONMENTAL_SENSING_01_END_HANDLE 0x000e

//
// list mapping between characteristics and handles
//
#define ATT_CHARACTERISTIC_GAP_DEVICE_NAME_01_VALUE_HANDLE 0x0003
#define ATT_CHARACTERISTIC_GATT_DATABASE_HASH_01_VALUE_HANDLE 0x0006
#define ATT_CHARACTERISTIC_fc930f88_1a30_45d7_8c17_604c1a036b9f_01_VALUE_HANDLE 0x0009
#define ATT_CHARACTERISTIC_fc930f88_1a30_45d7_8c17_604c1a036b9f_01_USER_DESCRIPTION_HANDLE 0x000a
#define ATT_CHARACTERISTIC_ORG_BLUETOOTH_CHARACTERISTIC_TEMPERATURE_01_VALUE_HANDLE 0x000c
#define ATT_CHARACTERISTIC_ORG_BLUETOOTH_CHARACTERISTIC_TEMPERATURE_01_CLIENT_CONFIGURATION_HANDLE 0x000d
#define ATT_CHARACTERISTIC_ORG_BLUETOOTH_CHARACTERISTIC_TEMPERATURE_01_USER_DESCRIPTION_HANDLE 0x000e
