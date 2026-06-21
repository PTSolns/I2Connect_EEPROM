# PTSolns *I2Connect: EEPROM* Library

![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Storage](https://img.shields.io/badge/Storage-256KB-orange)

Library for the PTSolns *I2Connect: EEPROM* module. Available through both the Arduino Library Manager and the PTSolns IDE Library Manager, this library provides simple access to the module's full 2 Mbit (256 KByte) non volatile memory space while automatically managing page boundaries, internal address partitions, and I2C communication details.

---

## Overview

The *I2Connect: EEPROM* is a high capacity non volatile memory module designed for the PTSolns I2Connect ecosystem. It provides 256 KByte of storage for configuration parameters, calibration constants, measurement logs, event records, and other application data.

Internally, the EEPROM is organized into four 64 KByte memory partitions. The library automatically manages these partitions, allowing the entire memory space to be accessed as a single contiguous address range from 0 to 262143.

Unlike many generic EEPROM libraries, the PTSolns library automatically manages page boundaries, partition transitions, write cycle polling, and I2C transfer limitations, allowing the user to focus on storing and retrieving data rather than low level EEPROM details.

The module connects using the standard I2Connect interface, making it easy to add large scale data storage to compatible microcontroller development boards without soldering.

---

## *I2Connect* Series

The PTSolns *I2Connect* Series is a family of plug and play expansion modules designed to simplify the integration of sensors, displays, user interfaces, and other peripherals into embedded systems. Built around the industry standard I2C communication protocol and featuring convenient Qwiic® compatible connectors (4 pin, 1 mm JST SH), *I2Connect* modules can be quickly connected without soldering or complex wiring.

Each module is fully assembled and includes a preinstalled male header that is compatible with standard solderless breadboards, enabling immediate use and rapid prototyping. Designed to work seamlessly with a wide range of microcontroller platforms, I2Connect modules combine compact hardware, comprehensive documentation, and easy to use software libraries. The _I2Connect_ Series enables makers, students, educators, and professionals to rapidly prototype, evaluate, and develop reliable electronic systems.

The following modules are part of the *I2Connect* Series:

* [**AHT20**](https://ptsolns.com/products/i2connect-aht20) — Temperature & Humidity
* **VEML7700** — Light & Lux
* [**EEPROM**](https://ptsolns.com/products/i2connect-eeprom) — 2 Mbit Memory
* **Buttons** — User Input via 4×4 Matrix of Buttons
* **Digit Display** — Dual 7 Segment Display
* **ICP-20100** — Pressure & Temperature
* **KXTJ3-1057** — 3 Axis Accelerometer
* **MUX** — 4 Channel I2C Multiplexer
* **OLED** — 0.96" 128×64 OLED Display
* **I2C Doctor** — I2C Bus Recovery and Diagnostics
* **...and more!**

---

## Key Features

* Full 2 Mbit (256 KByte) EEPROM support
* Automatic management of all four internal 64 KByte memory partitions
* Automatic page boundary handling
* Automatic write cycle completion polling
* Automatic handling of I2C transfer limits
* Single byte read and write functions
* Array read and write functions
* String storage and retrieval
* Structure storage and retrieval
* Wear reducing update functions
* EEPROM fill and clear functions
* Compatible with AVR, ESP32, RP2040, STM32, and other Arduino supported platforms
* Supports both default and alternate EEPROM address ranges

---

## I2Connect Ecosystem

The *I2Connect: EEPROM* is part of the PTSolns I2Connect ecosystem.

All *I2Connect* modules utilize a standardized 4 pin JST SH 1.0 mm connector carrying:

| Pin | Function |
| --- | -------- |
| 1   | GND      |
| 2   | VCC      |
| 3   | SDA      |
| 4   | SCL      |

This connector follows the widely adopted Qwiic® standard, allowing modules and compatible boards to be connected without soldering.

For maximum compatibility, the module also includes a preinstalled angled male header, allowing use with virtually any 3.3 V or 5 V microcontroller platform even when a Qwiic compatible connector is unavailable.

---

## EEPROM Specifications

| Parameter               | Value                        |
| ----------------------- | ---------------------------- |
| Memory Capacity         | 2 Mbit (256 KByte)           |
| Internal Partitions     | 4 × 64 KByte                 |
| Data Retention          | Up to 200 Years              |
| Endurance               | 4 Million Write/Erase Cycles |
| Interface               | I2C                          |
| Default Address Range   | 0x50 to 0x53                 |
| Alternate Address Range | 0x54 to 0x57                 |
| Supply Voltage          | 1.8 V to 5.5 V               |
| Maximum I2C Clock       | 1 MHz                        |

---

## Memory Map

The EEPROM contains 2 Mbit (256 KByte) of storage organized into four internal 64 KByte partitions.

| Partition | Memory Range       | I2C Address |
| --------- | ------------------ | ----------- |
| 0         | 0 to 65,535        | 0x50        |
| 1         | 65,536 to 131,071  | 0x51        |
| 2         | 131,072 to 196,607 | 0x52        |
| 3         | 196,608 to 262,143 | 0x53        |

When the alternate address jumper is selected, the partitions occupy addresses 0x54 through 0x57.

The library automatically manages these partitions and presents the EEPROM as a single contiguous memory space.

---

## *PTSolns IDE*

The [*PTSolns IDE*](https://ptsolns.com/ide) is a free desktop application for programming PTSolns microcontroller development boards and compatible Arduino platforms.

This library is available directly through both the PTSolns IDE Library Manager and the Arduino Library Manager.

To install:

1. Open the PTSolns IDE (or Arduino IDE).
2. Open the Library Manager.
3. Search for:

```text
I2Connect_EEPROM
```

4. Click Install.

Features of the [*PTSolns IDE*](https://ptsolns.com/ide) include:

* Integrated board management
* Built in serial monitor
* Library management
* Example project browser
* Support for over 8,500 third party libraries
* Built in CH340 USB driver support
* Open source and community driven

The [*PTSolns IDE*](https://ptsolns.com/ide) can be used alongside the Arduino IDE and supports many PTSolns development boards out of the box.

---

## Quick Start

```cpp
#include <Wire.h>
#include <PTSolns_I2Connect_EEPROM.h>

PTSolns_I2Connect_EEPROM eeprom;

void setup()
{
    Serial.begin(115200);

    Wire.begin();

    eeprom.begin();

    eeprom.writeString(100, "Hello EEPROM");

    String text;

    eeprom.readString(100, text);

    Serial.println(text);
}

void loop()
{
}
```

---

## Address Selection

Internally, the EEPROM is divided into four 64 KByte memory partitions. The library automatically manages these partitions and presents the EEPROM as a single memory space.

By default, the module occupies:

```text
0x50
0x51
0x52
0x53
```

If the address jumper on the rear of the board is changed, the module occupies:

```text
0x54
0x55
0x56
0x57
```

During initialization:

```cpp
eeprom.begin(Wire, 0, 100000);
```

or

```cpp
eeprom.begin(Wire, 1, 100000);
```

| Setting | Address Range |
| ------- | ------------- |
| 0       | 0x50 to 0x53  |
| 1       | 0x54 to 0x57  |

---

## Library Functions

| Function      | Description              |
| ------------- | ------------------------ |
| writeByte()   | Store a single byte      |
| readByte()    | Read a single byte       |
| writeBytes()  | Store an array of bytes  |
| readBytes()   | Read an array of bytes   |
| writeString() | Store a String           |
| readString()  | Read a String            |
| put()         | Store a structure        |
| get()         | Read a structure         |
| update()      | Write only changed bytes |
| clear()       | Erase EEPROM contents    |
| fill()        | Fill memory with a value |

---

## Basic Functions

### Single Byte Storage

```cpp
eeprom.writeByte(0, 123);

uint8_t value;

eeprom.readByte(0, value);
```

### String Storage

```cpp
eeprom.writeString(100, "Hello World");

String text;

eeprom.readString(100, text);
```

### Array Storage

```cpp
uint8_t data[] = {1, 2, 3, 4};

eeprom.writeBytes(500, data, sizeof(data));

uint8_t buffer[4];

eeprom.readBytes(500, buffer, sizeof(buffer));
```

### Structure Storage

```cpp
struct Settings
{
    float temperatureOffset;
    uint16_t sampleInterval;
};

Settings settings;

eeprom.put(1000, settings);

eeprom.get(1000, settings);
```

### Wear Reducing Updates

```cpp
eeprom.updateByte(0, 123);

eeprom.update(1000, settings);
```

Only bytes that have changed will be written, helping maximize EEPROM endurance.

### Clear Memory

```cpp
eeprom.clear();
```

Clear a specific region:

```cpp
eeprom.clear(0, 256);
```

---

## Write Protection

A Write Control (WC) pin is available on the angled male header.

| WC State | Function                     |
| -------- | ---------------------------- |
| Low      | Read and Write Enabled       |
| High     | Read Enabled, Write Disabled |

For permanent write protection, the corresponding jumper on the rear of the module may be cut. This permanently places the EEPROM into a read only mode regardless of the external WC signal.

---

## Included Examples

| Example              | Description                          |
| -------------------- | ------------------------------------ |
| 01_BasicReadWrite    | Read and write a single byte         |
| 02_StringStorage     | Store and retrieve strings           |
| 03_ArrayStorage      | Store and retrieve arrays            |
| 04_StructStorage     | Store and retrieve structures        |
| 05_UpdateWithoutWear | Minimize EEPROM wear                 |
| 06_ClearAndVerify    | Clear memory regions                 |
| 07_AddressBlocks     | Demonstrate internal partitions      |
| 08_DataLogger        | Store sequential measurement records |

---

## Documentation & Resources

Additional resources are available through the PTSolns Documentation Repository, including:

* Datasheets

* Schematics

* Pinout diagrams

* 3D models

* Fritzing files

* Tutorials

* Example projects

* [PTSolns Documentation Repository](https://docs.ptsolns.com)

* [PTSolns Website](https://ptsolns.com)

* [Contact Support](https://ptsolns.com/contact-us)

* [PTSolns IDE](https://ptsolns.com/ide)
