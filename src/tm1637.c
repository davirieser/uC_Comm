
#include "tm1637.h"

void setup_tm(struct tm1637 tm) {

    struct PinConfig config = {
        .otyper=GPIO_OTYPE_PUSH_PULL,
        .moder=GPIO_MODE_GPIO,
        .ospeedr=GPIO_SPEED_LOW,
        .pupd=GPIO_PUPD_PULL_UP,
        .alt_fun=0x00,
        .lock=0x00
    };

    setupPins(
        TM_TO_PINS(tm),
        config,
        2
    );

    _stop_tm(tm);

}

void set_segments(
    struct tm1637 tm,
    uint8_t * segments,
    uint32_t num_segments
) {

    uint32_t iLauf;

    write_to_tm(tm,CTRL_CODE_1,8);

    // Write COMM2 + first digit address
    write_to_tm(tm,CTRL_CODE_2,8);

    // Write the data bytes
    for (iLauf = 0; iLauf < num_segments; iLauf ++)
      write_to_tm(tm,segments[iLauf],8);

    // Write COMM3 + brightness
    write_to_tm(tm,CTRL_CODE_3 + 0x00,8);

}

void clear_display() {

}

void write_to_tm(
    struct tm1637 tm,
    uint32_t data,
    uint32_t data_bits
) {

    _start_tm(tm);

    shift_out_clk(
        (struct PinDefinition){
            .port=tm.data_port,
            .pin_number=tm.data_pin
        },
        (struct PinDefinition){
            .port=tm.clk_port,
            .pin_number=tm.clk_pin
        },
        data,
        data_bits,
        _bitdelay
    );

    _stop_tm(tm);

}

void _start_tm(struct tm1637 tm) {

    _bitdelay();

    // Change Data-Pin to be an Output
    change_pin_mode(
        (struct PinDefinition){
            .port=tm.data_port,
            .pin_number=tm.data_pin
        },
        GPIO_MODE_GPIO
    );

    _bitdelay();

    // Change Clk-Pin to be an Output
    change_pin_mode(
        (struct PinDefinition){
            .port=tm.clk_port,
            .pin_number=tm.clk_pin
        },
        GPIO_MODE_GPIO
    );

    _bitdelay();

}

void _stop_tm(struct tm1637 tm) {

    _bitdelay();

    // Clear Clk-Pin
    write_to_pin(
        (struct PinDefinition){
            .port=tm.clk_port,
            .pin_number=tm.clk_pin
        },
        0x00
    );

    // Clear Data-Pin
    write_to_pin(
        (struct PinDefinition){
            .port=tm.data_port,
            .pin_number=tm.data_pin
        },
        0x00
    );

    _bitdelay();

    // Change Data-Pin to be an Input
    change_pin_mode(
        (struct PinDefinition){
            .port=tm.clk_port,
            .pin_number=tm.clk_pin
        },
        GPIO_MODE_INPUT
    );

    _bitdelay();

    // Change Data-Pin to be an Input
    change_pin_mode(
        (struct PinDefinition){
            .port=tm.data_port,
            .pin_number=tm.data_pin
        },
        GPIO_MODE_INPUT
    );

    _bitdelay();

}
