#include "payload.hpp"

#include <cstdint>
#include <vector>
#include <stdexcept>


constexpr uint8_t OUT_TELEMETRY_TYPE     = 0x01;
constexpr uint8_t COMMAND_MECHANISM_TYPE = 0x02;
constexpr uint8_t COMMAND_TELEMETRY_TYPE = 0x03;




void extract_bytes(
    uint32_t value,
    uint8_t num_bytes,
    uint8_t* bytes)
{
    for (uint8_t i = 0; i < num_bytes; ++i)
    {
        bytes[i] = (value >> (i * 8)) & 0xFF;
    }
}


void pack_out_telemetry(
    const OutTelemetry& out_telemetry,
    std::vector<uint8_t>& packed_data)
{
    packed_data.resize(7);

    packed_data[0] = OUT_TELEMETRY_TYPE;

    extract_bytes(
        out_telemetry.seconds_since_epoch,
        4,
        packed_data.data() + 1
    );

    extract_bytes(
        out_telemetry.mechanisms_deployed_flags,
        2,
        packed_data.data() + 5
    );
}


void pack_command_telemetry(
    const CommandTelemetry& command_telemetry,
    std::vector<uint8_t>& packed_data)
{
    packed_data.resize(2);

    packed_data[0] = COMMAND_TELEMETRY_TYPE;
    packed_data[1] = command_telemetry.is_on ? 0x01 : 0x00;
}


void unpack_command_telemetry(
    const std::vector<uint8_t>& packed_data,
    CommandTelemetry& command_telemetry)
{
    if (packed_data.size() != 2)
    {
        throw std::runtime_error(
            "Invalid CommandTelemetry payload length."
        );
    }

    if (packed_data[0] != COMMAND_TELEMETRY_TYPE)
    {
        throw std::runtime_error(
            "Payload is not CommandTelemetry."
        );
    }

    if (packed_data[1] != 0x00 &&
        packed_data[1] != 0x01)
    {
        throw std::runtime_error(
            "Invalid CommandTelemetry boolean value."
        );
    }

    command_telemetry.is_on = (packed_data[1] == 0x01);
}


void pack_command_mechanism(
    const CommandMechanism& command_mechanism,
    std::vector<uint8_t>& packed_data)
{
    packed_data.resize(3);

    packed_data[0] = COMMAND_MECHANISM_TYPE;
    packed_data[1] = command_mechanism.mechanism_id;
    packed_data[2] = command_mechanism.value;
}


void unpack_command_mechanism(
    const std::vector<uint8_t>& packed_data,
    CommandMechanism& command_mechanism)
{
    if (packed_data.size() != 3)
    {
        throw std::runtime_error(
            "Invalid CommandMechanism payload length."
        );
    }

    if (packed_data[0] != COMMAND_MECHANISM_TYPE)
    {
        throw std::runtime_error(
            "Payload is not CommandMechanism."
        );
    }

    command_mechanism.mechanism_id = packed_data[1];
    command_mechanism.value = packed_data[2];
}