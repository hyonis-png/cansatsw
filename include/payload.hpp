#pragma once

#include <cstdint>
#include <vector>

#include "messages.hpp"
void pack_out_telemetry(
    const OutTelemetry&,
    std::vector<uint8_t>&);

void pack_command_mechanism(
    const CommandMechanism&,
    std::vector<uint8_t>&);

void pack_command_telemetry(
    const CommandTelemetry&,
    std::vector<uint8_t>&);

void unpack_command_mechanism(
    const std::vector<uint8_t>&,
    CommandMechanism&);

void unpack_command_telemetry(
    const std::vector<uint8_t>&,
    CommandTelemetry&);