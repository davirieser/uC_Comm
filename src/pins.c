
#include "pins.h"

inline uint32_t return_gpio_port_reg(uint32_t port) {

    if (port == GPIO_A) {
        return REGISTER_GPIO_A_START;
    }
    else if(port == GPIO_B) {
        return REGISTER_GPIO_B_START;
    }
    else if(port == GPIO_C) {
        return REGISTER_GPIO_C_START;
    }
    else if(port == GPIO_D) {
        return REGISTER_GPIO_D_START;
    }
    else if(port == GPIO_F) {
        return REGISTER_GPIO_F_START;
    }
    else {
        return REGISTER_GPIO_A_START;
    }

}

void setupPin(
    struct PinDefinition pin,
    struct PinConfig config
){

    uint32_t port_reg = return_gpio_port_reg(pin.port);

    uint32_t outt_register = port_reg + GPIO_OTYPER_REGISTER_OFFSET;
    uint32_t pupd_register = port_reg + GPIO_PUPDR_REGISTER_OFFSET;
    uint32_t mode_register = port_reg + GPIO_MODER_REGISTER_OFFSET;
    uint32_t aflr_register = port_reg + GPIO_AFRH_REGISTER_OFFSET;
    uint32_t afhr_register = port_reg + GPIO_AFRH_REGISTER_OFFSET;
    uint32_t lock_register = port_reg + GPIO_LCKR_REGISTER_OFFSET;

    // Lock Registers so they cannot be changed unless there is a Reset
    uint32_t locked_registers = 1 << pin.pin_number;

    // Check that the Registers is not locked
    if ((READ_REG(lock_register) & locked_registers) == 0){

        // Check if the Pin is valid to avoid Overflow-Error / Writing to the other Registers
        if ((pin.pin_number < 16) & (pin.pin_number >= 0)) {

            // Change GPIO's Output Type
            CLEAR_BIT(outt_register,GPIO_TYPE_MASK << (GPIO_TYPE_BITS * pin.pin_number));
            SET_BIT(outt_register,config.otyper << (GPIO_TYPE_BITS * pin.pin_number));

            // Change GPIO's Pull-Up-Pull-Down Type
            CLEAR_BIT(pupd_register,GPIO_PUPD_MASK << (GPIO_PUPD_BITS * pin.pin_number));
            SET_BIT(pupd_register,config.pupd << (GPIO_PUPD_BITS * pin.pin_number));

            // Change GPIO's Mode Type
            CLEAR_BIT(mode_register,GPIO_MODE_MASK << (GPIO_MODE_BITS * pin.pin_number));
            SET_BIT(mode_register,config.moder << (GPIO_MODE_BITS * pin.pin_number));

            // Check that alternate Function is selected for the Pin
            if (config.moder == GPIO_MODE_ALT_FUN){

                // Configure Alternate-Function
                if (pin.pin_number <= 7){
                    // Write to the Alternate-Funtion-Low-Regiter if the Pin-Number is smaller or equal 7
                    CLEAR_BIT(aflr_register,(ALTERNATE_FUNCTION_MASK << (ALTERNATE_FUNCTION_BITS * pin.pin_number)));
                    SET_BIT(aflr_register,(ALTERNATE_FUNCTION << (ALTERNATE_FUNCTION_BITS * pin.pin_number)));
                }else if (pin.pin_number <= 15){
                    // Write to the Alternate-Funtion-High-Regiter if the Pin-Number is smaller or equal 15 ( to avoid Overflows)
                    CLEAR_BIT(afhr_register,(ALTERNATE_FUNCTION_MASK << (ALTERNATE_FUNCTION_BITS * (pin.pin_number - 8))));
                    SET_BIT(afhr_register,(ALTERNATE_FUNCTION << (ALTERNATE_FUNCTION_BITS * (pin.pin_number - 8))));
                }
            }

        }

        if (config.lock & 0x01) {

            // Lock Key Write Sequence according to manual
            SET_BIT(lock_register,1 << 16); // Set the 16th Bit
            CLEAR_BIT(lock_register,1 << 16); // Clear the 16th Bit
            SET_BIT(lock_register,1 << 16); // Set the 16th Bit
            if ((READ_REG(lock_register) >> 16) & 0x1) { // Then check if the 16th Bit is set
                // Successeful
            }else{
                // Lock Key Write Sequence aborted
            }

        }

    }

    return;

}

void setupPins(
    struct PinDefinition * pins,
    struct PinConfig config,
    uint32_t num_pins
){

    uint32_t iLauf;
    uint32_t port_reg;
    struct PinDefinition curr_pin;

    // Lock Registers so they cannot be changed unless there is a Reset
    uint32_t locked_registers;


    uint32_t outt_register;
    uint32_t pupd_register;
    uint32_t mode_register;
    uint32_t aflr_register;
    uint32_t afhr_register;
    uint32_t lock_register;

    for(iLauf = 0; iLauf < num_pins; iLauf++){

        curr_pin = pins[iLauf];
        port_reg = return_gpio_port_reg(curr_pin.port);
        outt_register = port_reg + GPIO_OTYPER_REGISTER_OFFSET;
        pupd_register = port_reg + GPIO_PUPDR_REGISTER_OFFSET;
        mode_register = port_reg + GPIO_MODER_REGISTER_OFFSET;
        aflr_register = port_reg + GPIO_AFRH_REGISTER_OFFSET;
        afhr_register = port_reg + GPIO_AFRH_REGISTER_OFFSET;
        lock_register = port_reg + GPIO_LCKR_REGISTER_OFFSET;
        locked_registers = 1 << curr_pin.pin_number;

        // Check that the Registers is not locked
        if ((READ_REG(lock_register) & locked_registers) == 0){

            // Check if the Pin is valid to avoid Overflow-Error / Writing to the other Registers
            if ((curr_pin.pin_number < 16) & (curr_pin.pin_number >= 0)) {

                // Change GPIO's Output Type
                CLEAR_BIT(outt_register,GPIO_TYPE_MASK << (GPIO_TYPE_BITS * curr_pin.pin_number));
                SET_BIT(outt_register,config.otyper << (GPIO_TYPE_BITS * curr_pin.pin_number));

                // Change GPIO's Pull-Up-Pull-Down Type
                CLEAR_BIT(pupd_register,GPIO_PUPD_MASK << (GPIO_PUPD_BITS * curr_pin.pin_number));
                SET_BIT(pupd_register,config.pupd << (GPIO_PUPD_BITS * curr_pin.pin_number));

                // Change GPIO's Mode Type
                CLEAR_BIT(mode_register,GPIO_MODE_MASK << (GPIO_MODE_BITS * curr_pin.pin_number));
                SET_BIT(mode_register,config.moder << (GPIO_MODE_BITS * curr_pin.pin_number));

                // Check that alternate Function is selected for the Pin
                if (config.moder == GPIO_MODE_ALT_FUN){

                    // Configure Alternate-Function
                    if (curr_pin.pin_number <= 7){
                        // Write to the Alternate-Funtion-Low-Regiter if the Pin-Number is smaller or equal 7
                        CLEAR_BIT(aflr_register,(ALTERNATE_FUNCTION_MASK << (ALTERNATE_FUNCTION_BITS * curr_pin.pin_number)));
                        SET_BIT(aflr_register,(ALTERNATE_FUNCTION << (ALTERNATE_FUNCTION_BITS * curr_pin.pin_number)));
                    }else if (curr_pin.pin_number <= 15){
                        // Write to the Alternate-Funtion-High-Regiter if the Pin-Number is smaller or equal 15 ( to avoid Overflows)
                        CLEAR_BIT(afhr_register,(ALTERNATE_FUNCTION_MASK << (ALTERNATE_FUNCTION_BITS * (curr_pin.pin_number - 8))));
                        SET_BIT(afhr_register,(ALTERNATE_FUNCTION << (ALTERNATE_FUNCTION_BITS * (curr_pin.pin_number - 8))));
                    }
                }

            }

            if (config.lock & 0x01) {

                // Lock Key Write Sequence according to manual
                SET_BIT(lock_register,1 << 16); // Set the 16th Bit
                CLEAR_BIT(lock_register,1 << 16); // Clear the 16th Bit
                SET_BIT(lock_register,1 << 16); // Set the 16th Bit
                if ((READ_REG(lock_register) >> 16) & 0x1) { // Then check if the 16th Bit is set
                    // Successeful
                }else{
                    // Lock Key Write Sequence aborted
                }

            }

        }

    }

    return;

}

void change_pin_outt(
    struct PinDefinition pin,
    uint32_t outt
) {

    uint32_t outt_register = return_gpio_port_reg(pin.port) + GPIO_OTYPER_REGISTER_OFFSET;

    // Change GPIO's Output Type
    CLEAR_BIT(outt_register,GPIO_TYPE_MASK << (GPIO_TYPE_BITS * pin.pin_number));
    SET_BIT(outt_register,outt << (GPIO_TYPE_BITS * pin.pin_number));

}

void change_pin_mode(
    struct PinDefinition pin,
    uint32_t mode
) {

    uint32_t mode_register = return_gpio_port_reg(pin.port) + GPIO_MODER_REGISTER_OFFSET;

    // Change GPIO's Mode Type
    CLEAR_BIT(mode_register,GPIO_MODE_MASK << (GPIO_MODE_BITS * pin.pin_number));
    SET_BIT(mode_register,mode << (GPIO_MODE_BITS * pin.pin_number));



}

void change_pin_pupd(
    struct PinDefinition pin,
    uint32_t pupd
) {

    uint32_t pupd_register = return_gpio_port_reg(pin.port) + GPIO_PUPDR_REGISTER_OFFSET;

    // Change GPIO's Pull-Up-Pull-Down Type
    CLEAR_BIT(pupd_register,GPIO_PUPD_MASK << (GPIO_PUPD_BITS * pin.pin_number));
    SET_BIT(pupd_register,pupd << (GPIO_PUPD_BITS * pin.pin_number));

}

void lock_pin_config(
    struct PinDefinition pin
) {

    uint32_t lock_register = return_gpio_port_reg(pin.port) + GPIO_LCKR_REGISTER_OFFSET;

    // Lock Key Write Sequence according to manual
    SET_BIT(lock_register,1 << 16); // Set the 16th Bit
    CLEAR_BIT(lock_register,1 << 16); // Clear the 16th Bit
    SET_BIT(lock_register,1 << 16); // Set the 16th Bit
    if ((READ_REG(lock_register) >> 16) & 0x1) { // Then check if the 16th Bit is set
        // Successeful
    }else{
        // Lock Key Write Sequence aborted
    }

}

void write_to_pin(
    struct PinDefinition pin,
    uint32_t value
){

    uint32_t output_reg = return_gpio_port_reg(pin.port) + GPIO_ODR_REGISTER_OFFSET;

    if ((value & 0x01) == 0x01){
        SET_BIT(output_reg,(1 << pin.pin_number));
    }else{
        CLEAR_BIT(output_reg,(1 << pin.pin_number));
    }

}

void toggle_pin(
    struct PinDefinition pin
) {

    uint32_t output_reg = return_gpio_port_reg(pin.port) + GPIO_ODR_REGISTER_OFFSET;

    XOR_BIT(output_reg,(1 << pin.pin_number));

}

uint32_t read_from_pin(
    struct PinDefinition pin
){

    uint32_t output_reg = return_gpio_port_reg(pin.port) + GPIO_ODR_REGISTER_OFFSET;

    return (((READ_REG(output_reg) >> pin.pin_number) & 0x01));

}

void shift_out(
    struct PinDefinition data_pin,
    uint32_t bits,
    uint32_t num_bits,
    void (*f)(void)
){

    uint32_t iLauf, iDelay;

    for(iLauf = num_bits; iLauf > 0;){

        iLauf --;

        // Shift out next bit
        write_to_pin(data_pin,((bits >> iLauf) & 0x01));
        // Execute Function at the End of the Shift
        f();

    }

}

void shift_out_clk(
    struct PinDefinition data_pin,
    struct PinDefinition clk_pin,
    uint32_t bits,
    uint32_t num_bits,
    void (*f)(void)
){

    uint32_t iLauf;

    write_to_pin(clk_pin,0x00);

    for(iLauf = num_bits; iLauf > 0;){

        iLauf --;

        // Shift out next bit
        write_to_pin(data_pin,((bits >> iLauf) & 0x01));
        // Toggle Clk-Pin
        write_to_pin(clk_pin,0x01);
        // Execute Function at the End of the Shift
        f();
        // Toggle Clk-Pin
        write_to_pin(clk_pin,0x00);
        // Execute Function at the End of the Shift
        f();

    }

}
