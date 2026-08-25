#pragma once

#include <cstdint>

struct OutTelemetry
{
    uint32_t seconds_since_epoch; //bit 0 for type, byte  1-5 for seconds since, byte 5-7 for mechanism flags 
    uint16_t mechanisms_deployed_flags;
};

struct CommandTelemetry
{
    bool is_on ; //byte  //byte 0 for type, byte 1 for is_on
};

struct CommandMechanism
{
    uint8_t mechanism_id; //byte 0 for type, byte 1 for mechanism id, byte 2 for value
    uint8_t value;
};