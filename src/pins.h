
#pragma once

#include "../main.h"
#include <stdint.h>

// GPIO Ports ------------------------------------------------------------------

#define NUMBER_GPIO_PORTS 5

#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_F 5

// Port-Registers --------------------------------------------------------------

#define REGISTER_GPIO_A_START 0x48000000
#define REGISTER_GPIO_A_END 0x4800003F

#define REGISTER_GPIO_B_START 0x48000400
#define REGISTER_GPIO_B_END 0x4800007F

#define REGISTER_GPIO_C_START 0x48000800
#define REGISTER_GPIO_C_END 0x48000BFF

#define REGISTER_GPIO_D_START 0x48000C00
#define REGISTER_GPIO_D_END 0x480000FF

#define REGISTER_GPIO_F_START 0x48001400
#define REGISTER_GPIO_F_END 0x4800017F

// -----------------------------------------------------------------------------
// Port Register Definitions

#if PORT_IDENTIFIER == GPIO_A
    #define PORT_REGISTER REGISTER_GPIO_A_START
#elif PORT_IDENTIFIER == GPIO_B
    #define PORT_REGISTER REGISTER_GPIO_B_START
#elif PORT_IDENTIFIER == GPIO_C
    #define PORT_REGISTER REGISTER_GPIO_C_START
#elif PORT_IDENTIFIER == GPIO_D
    #define PORT_REGISTER REGISTER_GPIO_D_START
#elif PORT_IDENTIFIER == GPIO_F
    #define PORT_REGISTER REGISTER_GPIO_F_START
#else
    // Use Ports A as default
    #define PORT_REGISTER REGISTER_GPIO_A_START
#endif

// -----------------------------------------------------------------------------
// GPIO Register Addresses

#define GPIO_MODER_REGISTER_OFFSET 0x00000000 // GPIO port mode register
#define GPIO_OTYPER_REGISTER_OFFSET 0x00000004 // GPIO port output type register
#define GPIO_OSPEEDR_REGISTER_OFFSET 0x00000008 // GPIO port output speed register
#define GPIO_PUPDR_REGISTER_OFFSET 0x0000000C // GPIO port pull-up/pull-down register
#define GPIO_IDR_REGISTER_OFFSET 0x00000010 // GPIO port input data register
#define GPIO_ODR_REGISTER_OFFSET 0x00000014 // GPIO port output data register
#define GPIO_BSRR_REGISTER_OFFSET 0x00000018 // GPIO port bit set/reset register
#define GPIO_LCKR_REGISTER_OFFSET 0x0000001C // GPIO port configuration lock register
#define GPIO_AFLR_REGISTER_OFFSET 0x00000020 // GPIO alternate function low register
#define GPIO_AFRH_REGISTER_OFFSET 0x00000024 // GPIO alternate function high register
#define GPIO_BRR_REGISTER_OFFSET 0x00000028 // GPIO port bit reset register

// GPIO MODE-Register

#define GPIO_MODE_BITS 0x02
#define GPIO_MODE_MASK 0x03

#define GPIO_MODE_INPUT 0x00
#define GPIO_MODE_GPIO 0x01
#define GPIO_MODE_ALT_FUN 0x02
#define GPIO_MODE_ANALOG 0x03

// GPIO OTYPE-Register

#define GPIO_TYPE_BITS 0x01
#define GPIO_TYPE_MASK 0x01

#define GPIO_OTYPE_PUSH_PULL 0x00
#define GPIO_OTYPE_OPEN_DRAIN 0x01

// GPIO SPEED-Register

#define GPIO_SPEED_BITS 0x02
#define GPIO_SPEED_MASK 0x03

#define GPIO_SPEED_LOW 0x00
#define GPIO_SPEED_MEDIUM 0x01
#define GPIO_SPEED_FAST 0x11

// GPIO Pull-Up-Pull-Down-Register

#define GPIO_PUPD_BITS 0x02
#define GPIO_PUPD_MASK 0x03

#define GPIO_PUPD_NO_PUPD 0x00
#define GPIO_PUPD_PULL_UP 0x01
#define GPIO_PUPD_PULL_DOWN 0x10

// GPIO Alternate-Functions-Register

#define ALTERNATE_FUNCTION_BITS 0x04
#define ALTERNATE_FUNCTION_MASK 0x0F

#define GPIO_ALTERNATE_FUNCTION0 0x00
#define GPIO_ALTERNATE_FUNCTION1 0x01
#define GPIO_ALTERNATE_FUNCTION2 0x02
#define GPIO_ALTERNATE_FUNCTION3 0x03
#define GPIO_ALTERNATE_FUNCTION4 0x04
#define GPIO_ALTERNATE_FUNCTION5 0x05
#define GPIO_ALTERNATE_FUNCTION6 0x06
#define GPIO_ALTERNATE_FUNCTION7 0x07

// Struct Definitions ----------------------------------------------------------

struct PinDefinition{
    uint8_t port;
    uint8_t pin_number;
};

struct PinConfig{
    uint8_t otyper;
    uint8_t moder;
    uint8_t ospeedr;
    uint8_t pupd;
    uint8_t alt_fun;
    uint8_t lock;
};

// Function Prototypes ---------------------------------------------------------

uint32_t return_gpio_port_reg(uint32_t port);

void setupPin(
    struct PinDefinition pin,
    struct PinConfig config
);

void setupPins(
    struct PinDefinition * pins,
    struct PinConfig config,
    uint32_t num_pins
);

void change_pin_outt(
    struct PinDefinition pin,
    uint32_t outt
);

void change_pin_pupd(
    struct PinDefinition pin,
    uint32_t pupd
);

void lock_pin_config(struct PinDefinition pin);

void change_pin_mode(
    struct PinDefinition pin,
    uint32_t mode
);

void write_to_pin(
    struct PinDefinition pin,
    uint32_t value
);

void toggle_pin(
    struct PinDefinition pin
);

uint32_t read_from_pin(
    struct PinDefinition pin
);

void shift_out(
    struct PinDefinition data_pin,
    uint32_t bits,
    uint32_t num_bits,
    void (*f)(void)
);

void shift_out_clk(
    struct PinDefinition data_pin,
    struct PinDefinition clk_pin,
    uint32_t bits,
    uint32_t num_bits,
    void (*f)(void)
);
