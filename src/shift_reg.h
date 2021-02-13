
#pragma once

#include "../main.h"
#include "pins.h"
#include <stdint.h>

struct shift_reg{
    uint8_t rclk_port;
    uint8_t rclk_pin;
    uint8_t data_port;
    uint8_t data_pin;
    uint8_t sclk_port;
    uint8_t sclk_pin;
};

#define SR_TO_PINS(sr) ( \
    (struct PinDefinition []){ \
        (struct PinDefinition){ \
            .port=sr.rclk_port, \
            .pin_number=sr.rclk_port \
        }, \
        (struct PinDefinition){ \
            .port=sr.sclk_port, \
            .pin_number=sr.sclk_port \
        }, \
        (struct PinDefinition){ \
            .port=sr.data_port, \
            .pin_number=sr.data_pin \
        } \
    } \
)

void setup_shift_reg(struct shift_reg tm);

void write_to_shift_reg(
    struct shift_reg sr,
    uint32_t data,
    uint32_t data_bits
);
