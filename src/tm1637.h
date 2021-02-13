
#pragma once

#include "../main.h"
#include "pins.h"
#include <stdint.h>

struct tm1637{
    uint8_t clk_port;
    uint8_t clk_pin;
    uint8_t data_port;
    uint8_t data_pin;
};

#define CTRL_CODE_1 0x40
#define CTRL_CODE_2 0xC0
#define CTRL_CODE_3 0x80

#define TM_TO_PINS(tm) ( \
    (struct PinDefinition []){ \
        (struct PinDefinition){ \
            .port=tm.clk_port, \
            .pin_number=tm.clk_pin \
        }, \
        (struct PinDefinition){ \
            .port=tm.data_port, \
            .pin_number=tm.data_pin \
        } \
    } \
)

void setup_tm(struct tm1637 tm);

void set_segments(
    struct tm1637 tm,
    uint8_t * segments,
    uint32_t num_segments
);

void write_to_tm(
    struct tm1637 tm,
    uint32_t data,
    uint32_t data_bits
);

void _start_tm(struct tm1637 tm);

void _stop_tm(struct tm1637 tm);
