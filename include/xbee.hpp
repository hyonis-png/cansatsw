#pragma once

#include <cstdint>
#include <vector>

struct ReceivePacket
{
    uint64_t source_address;
    std::vector<uint8_t> payload;
};

std::vector<uint8_t> build_transmit_request(
    const std::vector<uint8_t>& payload,
    uint64_t destination_address
);

ReceivePacket parse_receive_packet(
    const std::vector<uint8_t>& frame
);