// Example: Address Blocks for I2Connect: EEPROM
// Last Update: June 21, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Demonstrates the EEPROM's four internal 64 KByte address partitions.
// Shows how the library automatically handles partition transitions.
//
// SKILLS LEARNED
// deviceAddressFor()
// Automatic partition handling
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
const uint32_t SERIAL_BAUD  = 115200;
const uint32_t I2C_CLOCK_HZ = 100000;
const uint8_t  I2C_ADDRESS  = 0;

const uint32_t BLOCK_0_START = 0x00000;
const uint32_t BLOCK_1_START = 0x10000;
const uint32_t BLOCK_2_START = 0x20000;
const uint32_t BLOCK_3_START = 0x30000;

PTSolns_I2Connect_EEPROM eeprom;

void printBlockInfo(uint32_t memoryAddress) {
    Serial.print("Memory address 0x");
    Serial.print(memoryAddress, HEX);
    Serial.print(" uses I2C address 0x");
    Serial.println(eeprom.deviceAddressFor(memoryAddress), HEX);
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) { }

    Wire.begin();

    if (!eeprom.begin(Wire, I2C_ADDRESS, I2C_CLOCK_HZ)) {
        Serial.println("EEPROM not detected.");
        return;
    }

    Serial.println("EEPROM address block demonstration:");
    printBlockInfo(BLOCK_0_START);
    printBlockInfo(BLOCK_1_START);
    printBlockInfo(BLOCK_2_START);
    printBlockInfo(BLOCK_3_START);

    const String text = "This write crosses an internal address block boundary.";
    const uint32_t CROSS_BOUNDARY_ADDRESS = 0x0FFF0;

    if (!eeprom.writeString(CROSS_BOUNDARY_ADDRESS, text)) {
        Serial.println("Cross boundary write failed.");
        return;
    }

    String textRead;

    if (!eeprom.readString(CROSS_BOUNDARY_ADDRESS, textRead)) {
        Serial.println("Cross boundary read failed.");
        return;
    }

    Serial.print("Read back: ");
    Serial.println(textRead);
}

void loop() {
    // Nothing to do here
}
