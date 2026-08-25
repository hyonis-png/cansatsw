# CanSat XBee Communications Library

## Overview

This project implements a modular C++ communications library for transmitting and receiving application data using the XBee API protocol.

The library separates application-level payload serialization from XBee frame construction and parsing. This allows each component to be tested independently and reused without depending on UART hardware or an XBee module.

The project is built using CMake and includes a Google Test test suite.

---

# Project Structure

```
project/
├── CMakeLists.txt
├── include/
│   ├── payload.hpp
│   └── xbee.hpp
├── src/
│   ├── payload.cpp
│   └── xbee.cpp
├── tests/
│   └── test_payload.cpp
└── README.md
```

---

# Payload Format

Each payload begins with a one-byte message type identifier.

## OutTelemetry

| Byte | Description |
|------|-------------|
|0|Message Type = `0x01`|
|1-4|Seconds Since Epoch (32-bit, Little Endian)|
|5-6|Mechanisms Deployed Flags (16-bit, Little Endian)|

Example

```
01 78 56 34 12 CD AB
```

---

## CommandMechanism

| Byte | Description |
|------|-------------|
|0|Message Type = `0x02`|
|1|Mechanism ID|
|2|Value|

Example

```
02 05 FF
```

---

## CommandTelemetry

| Byte | Description |
|------|-------------|
|0|Message Type = `0x03`|
|1|Telemetry Enable (`0x01`) or Disable (`0x00`)|

Example

```
03 01
```

---

# Design Decisions

The project separates two independent responsibilities:

- Payload serialization/deserialization
- XBee API frame construction/parsing

The payload code only converts C++ structures into byte arrays and vice versa.

The XBee code only constructs and parses XBee API frames. It does not know the meaning of the payload data.

This separation makes the code easier to test, reuse, and maintain.

Additional implementation decisions include:

- Fixed-width integer types (`uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`)
- Little-endian encoding for multi-byte payload fields
- Checksum validation for received XBee frames
- Validation of frame length and payload length
- Runtime exceptions for malformed packets

---

# Building

Configure the project:

```bash
cmake -S . -B build
```

Build:

```bash
cmake --build build
```

---

# Running Tests

Run the test executable:

```bash
.\build\Debug\test_payload.exe
```

or, if CTest is configured,

```bash
ctest --test-dir build
```

---

# Features

- Build XBee Transmit Request (`0x10`) frames
- Parse XBee Receive Packet (`0x90`) frames
- Serialize `OutTelemetry`
- Deserialize `CommandTelemetry`
- Deserialize `CommandMechanism`
- Validate XBee checksums
- Validate frame lengths
- Handle malformed packets using exceptions

---

# Test Coverage

The Google Test suite verifies:

- OutTelemetry serialization
- CommandTelemetry serialization
- CommandTelemetry deserialization
- CommandMechanism serialization
- CommandMechanism deserialization
- Invalid payload lengths
- Invalid message types
- Invalid boolean values
- Error handling for malformed packets

A total of **11 Google Test cases** are included and all tests pass successfully.