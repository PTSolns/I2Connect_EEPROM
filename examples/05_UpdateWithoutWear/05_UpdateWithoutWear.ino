// Example: Update Without Wear for I2Connect: EEPROM
// Last Update: June 21, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Demonstrates how to update data without performing unnecessary writes.
// This helps maximize EEPROM endurance.
//
// SKILLS LEARNED
// update()
// updateByte()
// updateBytes()
//
// DESCRIPTION
// The I2Connect: EEPROM provides 2 Mbit (256 KByte) of non volatile memory for storing configuration parameters, calibration data, sensor measurements, and application data.
// Internally, the memory is organized into four 64 KByte partitions, each accessed through a different I2C address.
// By default, the module occupies addresses 0x50 through 0x53.
// An address selection jumper on the rear of the board may be changed to shift the address range to 0x54 through 0x57, allowing two modules to coexist on the same I2C bus.
// The PTSolns library automatically manages the partition addressing, allowing the entire 256 KByte memory space to be accessed as a single contiguous address range.
// A Write Control (WC) pin is available on the angled male header and may be driven high to disable all write operations while retaining read access.
// For permanent write protection, the corresponding jumper on the rear of the board may be cut, placing the module into a permanent read only mode.
// The EEPROM is rated for up to 200 years of data retention and 4 million write/erase cycles per memory cell.
//
// HOW MUCH MEMORY
// This EEPROM has 2 Mbit of storage. The following table shows how many values of each data type may be stored.
//
// Data Type | Size    | Max Values
// float     | 4 bytes | 65536
// double    | 4 bytes | 65536
// double    | 8 bytes | 32768
// int16_t   | 2 bytes | 131072
// int32_t   | 4 bytes | 65536
// uint8_t   | 1 byte  | 262144


#include <Wire.h>
#include <PTSolns_I2Connect_EEPROM.h>

// User Settings
const uint32_t SERIAL_BAUD     = 115200;
const uint32_t I2C_CLOCK_HZ    = 100000;
const uint8_t  I2C_ADDRESS     = 0;
const uint32_t COUNTER_ADDRESS = 2000;
const uint32_t COUNTER_VALUE   = 123456;

PTSolns_I2Connect_EEPROM eeprom;

void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) { }

    Wire.begin();

    if (!eeprom.begin(Wire, I2C_ADDRESS, I2C_CLOCK_HZ)) {
        Serial.println("EEPROM not detected.");
        return;
    }

    if (!eeprom.update(COUNTER_ADDRESS, COUNTER_VALUE)) {
        Serial.println("Update failed.");
        return;
    }

    uint32_t counterRead = 0;

    if (!eeprom.get(COUNTER_ADDRESS, counterRead)) {
        Serial.println("Read failed.");
        return;
    }

    Serial.print("Counter value: ");
    Serial.println(counterRead);

    Serial.println("The update function only writes bytes that have changed.");
}

void loop() {
    // Nothing to do here
}
