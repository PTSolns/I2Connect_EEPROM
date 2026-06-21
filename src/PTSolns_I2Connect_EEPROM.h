#ifndef PTSOLNS_I2CONNECT_EEPROM_H
#define PTSOLNS_I2CONNECT_EEPROM_H

#include <Arduino.h>
#include <Wire.h>

class PTSolns_I2Connect_EEPROM {
public:
    static const uint32_t DEFAULT_EEPROM_SIZE = 262144UL;
    static const uint16_t DEFAULT_PAGE_SIZE = 256;
    static const uint8_t DEFAULT_I2C_ADDRESS = 0;
    static const uint32_t DEFAULT_I2C_CLOCK = 100000UL;
    static const uint8_t DEFAULT_MAX_I2C_WRITE_BYTES = 28;
    static const uint8_t DEFAULT_MAX_I2C_READ_BYTES = 32;
    static const uint16_t DEFAULT_WRITE_TIMEOUT_MS = 10;
    static const uint32_t DEFAULT_STRING_MAX_LENGTH = 128;

    PTSolns_I2Connect_EEPROM();

    bool begin(TwoWire &wirePort = Wire, uint8_t i2cAddress = DEFAULT_I2C_ADDRESS, uint32_t i2cClock = DEFAULT_I2C_CLOCK);

    void setI2CAddress(uint8_t i2cAddress);
    void setI2CClock(uint32_t i2cClock);
    void setWriteTimeout(uint16_t timeoutMs);
    void setMaxTransferSize(uint8_t maxWriteBytes, uint8_t maxReadBytes);

    uint32_t size() const;
    uint16_t pageSize() const;

    bool isConnected(uint32_t memoryAddress = 0);

    bool writeByte(uint32_t memoryAddress, uint8_t value);
    bool updateByte(uint32_t memoryAddress, uint8_t value);
    bool readByte(uint32_t memoryAddress, uint8_t &value);

    bool writeBytes(uint32_t memoryAddress, const uint8_t *data, uint32_t length);
    bool updateBytes(uint32_t memoryAddress, const uint8_t *data, uint32_t length);
    bool readBytes(uint32_t memoryAddress, uint8_t *data, uint32_t length);

    bool writeString(uint32_t memoryAddress, const char *text);
    bool writeString(uint32_t memoryAddress, const String &text);
    bool readString(uint32_t memoryAddress, char *buffer, uint32_t bufferLength);
    bool readString(uint32_t memoryAddress, String &text, uint32_t maxLength = DEFAULT_STRING_MAX_LENGTH);

    template <typename T>
    bool put(uint32_t memoryAddress, const T &value) {
        return writeBytes(memoryAddress, (const uint8_t *)&value, sizeof(T));
    }

    template <typename T>
    bool get(uint32_t memoryAddress, T &value) {
        return readBytes(memoryAddress, (uint8_t *)&value, sizeof(T));
    }

    template <typename T>
    bool update(uint32_t memoryAddress, const T &value) {
        return updateBytes(memoryAddress, (const uint8_t *)&value, sizeof(T));
    }

    bool fill(uint32_t memoryAddress, uint8_t value, uint32_t length);
    bool clear(uint32_t memoryAddress = 0, uint32_t length = DEFAULT_EEPROM_SIZE);

    uint8_t deviceAddressFor(uint32_t memoryAddress) const;

private:
    TwoWire *_wire;
    uint8_t _i2cAddress;
    uint32_t _eepromSize;
    uint16_t _pageSize;
    uint32_t _i2cClock;
    uint8_t _maxI2CWriteBytes;
    uint8_t _maxI2CReadBytes;
    uint16_t _writeTimeoutMs;

    bool addressIsValid(uint32_t memoryAddress, uint32_t length = 1) const;
    bool waitForWrite(uint32_t memoryAddress);
    bool writeChunk(uint32_t memoryAddress, const uint8_t *data, uint8_t length);
    bool readChunk(uint32_t memoryAddress, uint8_t *data, uint8_t length);
    uint8_t bytesUntilPageEnd(uint32_t memoryAddress) const;
    uint8_t bytesUntilBlockEnd(uint32_t memoryAddress) const;
};

#endif
