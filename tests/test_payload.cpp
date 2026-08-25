#include <gtest/gtest.h>

#include "payload.hpp"

TEST(PayloadTest, PackOutTelemetry)
{
    OutTelemetry telemetry;
    telemetry.seconds_since_epoch = 0x12345678;
    telemetry.mechanisms_deployed_flags = 0xABCD;

    std::vector<uint8_t> payload;

    pack_out_telemetry(telemetry, payload);

    ASSERT_EQ(payload.size(), 7);

    // Message type
    EXPECT_EQ(payload[0], 0x01);

    // seconds_since_epoch, little endian
    EXPECT_EQ(payload[1], 0x78);
    EXPECT_EQ(payload[2], 0x56);
    EXPECT_EQ(payload[3], 0x34);
    EXPECT_EQ(payload[4], 0x12);

    // mechanisms_deployed_flags, little endian
    EXPECT_EQ(payload[5], 0xCD);
    EXPECT_EQ(payload[6], 0xAB);
}


TEST(PayloadTest, PackCommandTelemetryOn)
{
    CommandTelemetry command;
    command.is_on = true;

    std::vector<uint8_t> payload;

    pack_command_telemetry(command, payload);

    ASSERT_EQ(payload.size(), 2);

    EXPECT_EQ(payload[0], 0x03);
    EXPECT_EQ(payload[1], 0x01);
}


TEST(PayloadTest, PackCommandTelemetryOff)
{
    CommandTelemetry command;
    command.is_on = false;

    std::vector<uint8_t> payload;

    pack_command_telemetry(command, payload);

    ASSERT_EQ(payload.size(), 2);

    EXPECT_EQ(payload[0], 0x03);
    EXPECT_EQ(payload[1], 0x00);
}


TEST(PayloadTest, UnpackCommandTelemetry)
{
    std::vector<uint8_t> payload = {
        0x03,
        0x01
    };

    CommandTelemetry command;

    unpack_command_telemetry(payload, command);

    EXPECT_TRUE(command.is_on);
}


TEST(PayloadTest, PackCommandMechanism)
{
    CommandMechanism command;

    command.mechanism_id = 0x05;
    command.value = 0xAA;

    std::vector<uint8_t> payload;

    pack_command_mechanism(command, payload);

    ASSERT_EQ(payload.size(), 3);

    EXPECT_EQ(payload[0], 0x02);
    EXPECT_EQ(payload[1], 0x05);
    EXPECT_EQ(payload[2], 0xAA);
}


TEST(PayloadTest, UnpackCommandMechanism)
{
    std::vector<uint8_t> payload = {
        0x02,
        0x05,
        0xAA
    };

    CommandMechanism command;

    unpack_command_mechanism(payload, command);

    EXPECT_EQ(command.mechanism_id, 0x05);
    EXPECT_EQ(command.value, 0xAA);
}


// -------------------------
// Invalid payload tests
// -------------------------

TEST(PayloadTest, RejectInvalidCommandTelemetryLength)
{
    std::vector<uint8_t> payload = {
        0x03
    };

    CommandTelemetry command;

    EXPECT_THROW(
        unpack_command_telemetry(payload, command),
        std::runtime_error
    );
}


TEST(PayloadTest, RejectInvalidCommandTelemetryType)
{
    std::vector<uint8_t> payload = {
        0x99,
        0x01
    };

    CommandTelemetry command;

    EXPECT_THROW(
        unpack_command_telemetry(payload, command),
        std::runtime_error
    );
}


TEST(PayloadTest, RejectInvalidBoolean)
{
    std::vector<uint8_t> payload = {
        0x03,
        0x55
    };

    CommandTelemetry command;

    EXPECT_THROW(
        unpack_command_telemetry(payload, command),
        std::runtime_error
    );
}


TEST(PayloadTest, RejectInvalidCommandMechanismLength)
{
    std::vector<uint8_t> payload = {
        0x02,
        0x05
    };

    CommandMechanism command;

    EXPECT_THROW(
        unpack_command_mechanism(payload, command),
        std::runtime_error
    );
}


TEST(PayloadTest, RejectInvalidCommandMechanismType)
{
    std::vector<uint8_t> payload = {
        0x99,
        0x05,
        0xAA
    };

    CommandMechanism command;

    EXPECT_THROW(
        unpack_command_mechanism(payload, command),
        std::runtime_error
    );
}