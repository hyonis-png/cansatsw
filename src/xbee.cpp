#include "xbee.hpp"

#include <stdexcept>

ReceivePacket parse_receive_packet(const std::vector<uint8_t>& frame)
{
    if (frame.size() < 16)
    {
        throw std::runtime_error("Frame too short.");
    }

    // Start delimiter
    if (frame[0] != 0x7E)
    {
        throw std::runtime_error("Invalid start delimiter.");
    }

    // Read 16-bit length (MSB first)
    uint16_t length =
        (static_cast<uint16_t>(frame[1]) << 8) |
        static_cast<uint16_t>(frame[2]);

    // Total frame = delimiter + 2 length bytes + length + checksum
    if (frame.size() != static_cast<size_t>(length) + 4)
    {
        throw std::runtime_error("Invalid frame length.");
    }

    // Must be Receive Packet 0x90
    if (frame[3] != 0x90)
    {
        throw std::runtime_error("Not a Receive Packet (0x90).");
    }

    // Verify checksum
    uint8_t sum = 0;

    for (size_t i = 3; i < frame.size() - 1; ++i)
    {
        sum += frame[i];
    }

    uint8_t expected_checksum =
        static_cast<uint8_t>(0xFF - sum);

    if (frame.back() != expected_checksum)
    {
        throw std::runtime_error("Invalid checksum.");
    }

    ReceivePacket packet;

    // Reconstruct 64-bit source address
    packet.source_address = 0;

    for (int i = 0; i < 8; ++i)
    {
        packet.source_address <<= 8;
        packet.source_address |= frame[4 + i];
    }

    // Extract received RF payload.
    // NOTE: index 15 assumes bytes 12-14 are the remaining
    // 0x90 metadata fields for the XBee frame format you're using.
    packet.payload.assign(
        frame.begin() + 15,
        frame.end() - 1
    );

    return packet;
}


std::vector<uint8_t> build_transmit_request(
    const std::vector<uint8_t>& payload,
    uint64_t destination_address)
{
    std::vector<uint8_t> frame;

    // Start delimiter
    frame.push_back(0x7E);

    // Placeholder for 16-bit length
    frame.push_back(0x00);
    frame.push_back(0x00);

    // Frame type: Transmit Request
    frame.push_back(0x10);

    // Frame ID
    frame.push_back(0x01);

    // 64-bit destination address (MSB first)
    for (int i = 7; i >= 0; --i)
    {
        frame.push_back(
            static_cast<uint8_t>(
                (destination_address >> (8 * i)) & 0xFF
            )
        );
    }

    // Reserved 16-bit address
    frame.push_back(0xFF);
    frame.push_back(0xFE);

    // Broadcast radius
    frame.push_back(0x00);

    // Transmit options
    frame.push_back(0x00);

    // Append entire payload
    frame.insert(
        frame.end(),
        payload.begin(),
        payload.end()
    );

    // Length = everything between length field and checksum
    uint16_t length =
        static_cast<uint16_t>(frame.size() - 3);

    frame[1] =
        static_cast<uint8_t>((length >> 8) & 0xFF);

    frame[2] =
        static_cast<uint8_t>(length & 0xFF);

    // Calculate checksum
    uint8_t sum = 0;

    for (size_t i = 3; i < frame.size(); ++i)
    {
        sum += frame[i];
    }

    uint8_t checksum =
        static_cast<uint8_t>(0xFF - sum);

    frame.push_back(checksum);

    return frame;
}