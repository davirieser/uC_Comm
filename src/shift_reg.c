
#include "shift_reg.h"

void setup_shift_reg(struct shift_reg sr) {

    struct PinConfig config = {
        .otyper=GPIO_OTYPE_PUSH_PULL,
        .moder=GPIO_MODE_GPIO,
        .ospeedr=GPIO_SPEED_LOW,
        .pupd=GPIO_PUPD_PULL_DOWN,
        .alt_fun=0x00,
        .lock=0x00
    };

    // TODO Warum kann i des nit in einem SetupPins()-Call machen?

    setupPin(
        (struct PinDefinition) {
            .port=sr.rclk_port,
            .pin_number=sr.rclk_pin
        },
        config
    );
    setupPin(
        (struct PinDefinition) {
            .port=sr.data_port,
            .pin_number=sr.data_pin
        },
        config
    );
    setupPin(
        (struct PinDefinition) {
            .port=sr.sclk_port,
            .pin_number=sr.sclk_pin
        },
        config
    );

}

void write_to_shift_reg(
    struct shift_reg sr,
    uint32_t data,
    uint32_t data_bits
) {

    shift_out_clk(
        (struct PinDefinition){
            .port=sr.data_port,
            .pin_number=sr.data_pin
        },
        (struct PinDefinition){
            .port=sr.sclk_port,
            .pin_number=sr.sclk_pin
        },
        data,
        data_bits,
        _bitdelay
    );

    _bitdelay();

    write_to_pin(
        (struct PinDefinition){
            .port=sr.rclk_port,
            .pin_number=sr.rclk_pin
        },
        0x01
    );

    _bitdelay();

    write_to_pin(
        (struct PinDefinition){
            .port=sr.rclk_port,
            .pin_number=sr.rclk_pin
        },
        0x00
    );

}
