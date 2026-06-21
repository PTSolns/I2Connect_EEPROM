
// Example: Data Logger for I2Connect: EEPROM
// Last Update: June 21, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Creates a simple data logger that stores temperature and humidity measurements.
// Demonstrates storing many records sequentially in EEPROM memory.
//
// SKILLS LEARNED
// put()
// get()
// Sequential storage
//
#include <Wire.h>
#include <PTSolns_I2Connect_EEPROM.h>

const uint32_t SERIAL_BAUD = 115200;
const uint32_t I2C_CLOCK_HZ = 100000;
const uint8_t I2C_ADDRESS = 0;

struct LogEntry {
  uint32_t sampleNumber;
  float temperature;
  float humidity;
};

PTSolns_I2Connect_EEPROM eeprom;

void setup() {
  Serial.begin(SERIAL_BAUD);
  Wire.begin();
  eeprom.begin(Wire, I2C_ADDRESS, I2C_CLOCK_HZ);

  LogEntry entry = {1, 23.5f, 45.2f};
  eeprom.put(0, entry);

  LogEntry readBack;
  eeprom.get(0, readBack);

  Serial.print("Sample: ");
  Serial.println(readBack.sampleNumber);
}

void loop() {}
