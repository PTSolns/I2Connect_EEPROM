#include "PTSolns_I2Connect_EEPROM.h"

PTSolns_I2Connect_EEPROM::PTSolns_I2Connect_EEPROM() {
    _wire = &Wire;
    _i2cAddress = DEFAULT_I2C_ADDRESS;
    _eepromSize = DEFAULT_EEPROM_SIZE;
    _pageSize = DEFAULT_PAGE_SIZE;
    _i2cClock = DEFAULT_I2C_CLOCK;
    _maxI2CWriteBytes = DEFAULT_MAX_I2C_WRITE_BYTES;
    _maxI2CReadBytes = DEFAULT_MAX_I2C_READ_BYTES;
    _writeTimeoutMs = DEFAULT_WRITE_TIMEOUT_MS;
}

bool PTSolns_I2Connect_EEPROM::begin(TwoWire &wirePort, uint8_t i2cAddress, uint32_t i2cClock) {
    _wire = &wirePort;
    setI2CAddress(i2cAddress); 
    _i2cClock = i2cClock;
    _wire->setClock(_i2cClock);
    return isConnected();
}

void PTSolns_I2Connect_EEPROM::setI2CAddress(uint8_t i2cAddress) {
    
    if (i2cAddress >= 0x54 && i2cAddress <= 0x57) {
        _i2cAddress = 1;
    } 
    
    else if (i2cAddress == 1) {
        _i2cAddress = 1;
    } 
    
    else {
        _i2cAddress = 0;
    }
}



void PTSolns_I2Connect_EEPROM::setI2CClock(uint32_t i2cClock) {
    _i2cClock = i2cClock;
    if (_wire) {
        _wire->setClock(_i2cClock);
    }
}

void PTSolns_I2Connect_EEPROM::setWriteTimeout(uint16_t timeoutMs) {
    _writeTimeoutMs = timeoutMs;
}

void PTSolns_I2Connect_EEPROM::setMaxTransferSize(uint8_t maxWriteBytes, uint8_t maxReadBytes) {
    if (maxWriteBytes > 0) {
        _maxI2CWriteBytes = maxWriteBytes;
    }

    if (maxReadBytes > 0) {
        _maxI2CReadBytes = maxReadBytes;
    }
}

uint32_t PTSolns_I2Connect_EEPROM::size() const {
    return _eepromSize;
}

uint16_t PTSolns_I2Connect_EEPROM::pageSize() const {
    return _pageSize;
}

bool PTSolns_I2Connect_EEPROM::addressIsValid(uint32_t memoryAddress, uint32_t length) const {
    if (length == 0) {
        return true;
    }

    if (memoryAddress >= _eepromSize) {
        return false;
    }

    if (length > (_eepromSize - memoryAddress)) {
        return false;
    }

    return true;
}

uint8_t PTSolns_I2Connect_EEPROM::deviceAddressFor(uint32_t memoryAddress) const {
    uint8_t a17a16 = (memoryAddress >> 16) & 0x03;
    uint8_t address = 0b1010000;
    address |= (_i2cAddress << 2);
    address |= a17a16;
    return address;
}

bool PTSolns_I2Connect_EEPROM::isConnected(uint32_t memoryAddress) {
    if (!addressIsValid(memoryAddress)) {
        return false;
    }

    uint8_t deviceAddress = deviceAddressFor(memoryAddress);
    _wire->beginTransmission(deviceAddress);
    return (_wire->endTransmission() == 0);
}

bool PTSolns_I2Connect_EEPROM::waitForWrite(uint32_t memoryAddress) {
    uint8_t deviceAddress = deviceAddressFor(memoryAddress);
    unsigned long startTime = millis();

    while ((millis() - startTime) < _writeTimeoutMs) {
        _wire->beginTransmission(deviceAddress);

        if (_wire->endTransmission() == 0) {
            return true;
        }
    }

    return false;
}

uint8_t PTSolns_I2Connect_EEPROM::bytesUntilPageEnd(uint32_t memoryAddress) const {
    uint16_t pageOffset = memoryAddress % _pageSize;
    return (uint8_t)min((uint32_t)(_pageSize - pageOffset), (uint32_t)255);
}

uint8_t PTSolns_I2Connect_EEPROM::bytesUntilBlockEnd(uint32_t memoryAddress) const {
    uint16_t blockOffset = memoryAddress & 0xFFFF;
    return (uint8_t)min((uint32_t)(65536UL - blockOffset), (uint32_t)255);
}

bool PTSolns_I2Connect_EEPROM::writeChunk(uint32_t memoryAddress, const uint8_t *data, uint8_t length) {
    uint8_t deviceAddress = deviceAddressFor(memoryAddress);
    uint8_t msb = (memoryAddress >> 8) & 0xFF;
    uint8_t lsb = memoryAddress & 0xFF;

    _wire->beginTransmission(deviceAddress);
    _wire->write(msb);
    _wire->write(lsb);

    for (uint8_t i = 0; i < length; i++) {
        _wire->write(data[i]);
    }

    if (_wire->endTransmission() != 0) {
        return false;
    }

    return waitForWrite(memoryAddress);
}

bool PTSolns_I2Connect_EEPROM::readChunk(uint32_t memoryAddress, uint8_t *data, uint8_t length) {
    uint8_t deviceAddress = deviceAddressFor(memoryAddress);
    uint8_t msb = (memoryAddress >> 8) & 0xFF;
    uint8_t lsb = memoryAddress & 0xFF;

    _wire->beginTransmission(deviceAddress);
    _wire->write(msb);
    _wire->write(lsb);

    if (_wire->endTransmission(false) != 0) {
        return false;
    }

    uint8_t received = _wire->requestFrom(deviceAddress, length);

    if (received != length) {
        return false;
    }

    for (uint8_t i = 0; i < length; i++) {
        data[i] = _wire->read();
    }

    return true;
}

bool PTSolns_I2Connect_EEPROM::writeByte(uint32_t memoryAddress, uint8_t value) {
    return writeBytes(memoryAddress, &value, 1);
}

bool PTSolns_I2Connect_EEPROM::updateByte(uint32_t memoryAddress, uint8_t value) {
    uint8_t existingValue = 0;

    if (!readByte(memoryAddress, existingValue)) {
        return false;
    }

    if (existingValue == value) {
        return true;
    }

    return writeByte(memoryAddress, value);
}

bool PTSolns_I2Connect_EEPROM::readByte(uint32_t memoryAddress, uint8_t &value) {
    return readBytes(memoryAddress, &value, 1);
}

bool PTSolns_I2Connect_EEPROM::writeBytes(uint32_t memoryAddress, const uint8_t *data, uint32_t length) {
    if (length == 0) {
        return true;
    }

    if (data == NULL) {
        return false;
    }

    if (!addressIsValid(memoryAddress, length)) {
        return false;
    }

    uint32_t bytesWritten = 0;

    while (bytesWritten < length) {
        uint32_t currentAddress = memoryAddress + bytesWritten;
        uint32_t remaining = length - bytesWritten;

        uint8_t chunkLength = (uint8_t)min(remaining, (uint32_t)_maxI2CWriteBytes);
        chunkLength = min(chunkLength, bytesUntilPageEnd(currentAddress));
        chunkLength = min(chunkLength, bytesUntilBlockEnd(currentAddress));

        if (!writeChunk(currentAddress, data + bytesWritten, chunkLength)) {
            return false;
        }

        bytesWritten += chunkLength;
    }

    return true;
}

bool PTSolns_I2Connect_EEPROM::updateBytes(uint32_t memoryAddress, const uint8_t *data, uint32_t length) {
    if (length == 0) {
        return true;
    }

    if (data == NULL) {
        return false;
    }

    if (!addressIsValid(memoryAddress, length)) {
        return false;
    }

    for (uint32_t i = 0; i < length; i++) {
        if (!updateByte(memoryAddress + i, data[i])) {
            return false;
        }
    }

    return true;
}

bool PTSolns_I2Connect_EEPROM::readBytes(uint32_t memoryAddress, uint8_t *data, uint32_t length) {
    if (length == 0) {
        return true;
    }

    if (data == NULL) {
        return false;
    }

    if (!addressIsValid(memoryAddress, length)) {
        return false;
    }

    uint32_t bytesRead = 0;

    while (bytesRead < length) {
        uint32_t currentAddress = memoryAddress + bytesRead;
        uint32_t remaining = length - bytesRead;

        uint8_t chunkLength = (uint8_t)min(remaining, (uint32_t)_maxI2CReadBytes);
        chunkLength = min(chunkLength, bytesUntilBlockEnd(currentAddress));

        if (!readChunk(currentAddress, data + bytesRead, chunkLength)) {
            return false;
        }

        bytesRead += chunkLength;
    }

    return true;
}

bool PTSolns_I2Connect_EEPROM::writeString(uint32_t memoryAddress, const char *text) {
    if (text == NULL) {
        return false;
    }

    uint32_t length = strlen(text) + 1;
    return writeBytes(memoryAddress, (const uint8_t *)text, length);
}

bool PTSolns_I2Connect_EEPROM::writeString(uint32_t memoryAddress, const String &text) {
    return writeString(memoryAddress, text.c_str());
}

bool PTSolns_I2Connect_EEPROM::readString(uint32_t memoryAddress, char *buffer, uint32_t bufferLength) {
    if (buffer == NULL) {
        return false;
    }

    if (bufferLength == 0) {
        return false;
    }

    for (uint32_t i = 0; i < bufferLength; i++) {
        uint8_t value = 0;

        if (!readByte(memoryAddress + i, value)) {
            buffer[0] = '\0';
            return false;
        }

        buffer[i] = (char)value;

        if (value == '\0') {
            return true;
        }
    }

    buffer[bufferLength - 1] = '\0';
    return true;
}

bool PTSolns_I2Connect_EEPROM::readString(uint32_t memoryAddress, String &text, uint32_t maxLength) {
    text = "";

    if (maxLength == 0) {
        return false;
    }

    if (!addressIsValid(memoryAddress, maxLength)) {
        return false;
    }

    for (uint32_t i = 0; i < maxLength; i++) {
        uint8_t value = 0;

        if (!readByte(memoryAddress + i, value)) {
            text = "";
            return false;
        }

        if (value == '\0') {
            return true;
        }

        text += (char)value;
    }

    return true;
}

bool PTSolns_I2Connect_EEPROM::fill(uint32_t memoryAddress, uint8_t value, uint32_t length) {
    if (length == 0) {
        return true;
    }

    if (!addressIsValid(memoryAddress, length)) {
        return false;
    }

    uint8_t buffer[DEFAULT_MAX_I2C_WRITE_BYTES];

    for (uint8_t i = 0; i < sizeof(buffer); i++) {
        buffer[i] = value;
    }

    uint32_t bytesFilled = 0;

    while (bytesFilled < length) {
        uint32_t remaining = length - bytesFilled;
        uint8_t chunkLength = (uint8_t)min(remaining, (uint32_t)sizeof(buffer));

        if (!writeBytes(memoryAddress + bytesFilled, buffer, chunkLength)) {
            return false;
        }

        bytesFilled += chunkLength;
    }

    return true;
}

bool PTSolns_I2Connect_EEPROM::clear(uint32_t memoryAddress, uint32_t length) {
    return fill(memoryAddress, 0xFF, length);
}
