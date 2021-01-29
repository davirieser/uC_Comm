#include "usart.h"

uint32_t buffer_size = 0;
uint8_t * write_buffer;

uint32_t read_from_usart(void) {

    uint32_t ra = 0;

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;
    uint32_t usart_rdr = USART_REGISTER + USART_RDR_OFFSET;

    // Check if the USART is enabled
    if (READ_REG(usart_cr1) & 0x1){
        // Check if RXNE-Bit is 1
        // Otherwise there is nothing in the Read-Buffer to read
        if ((READ_REG(usart_isr) & (1 << 5)) == (1 << 5)) {
            // Return Data from the Buffer
            return READ_BIT(usart_rdr,0x000000FF);
        }else{
            dummy(ra);
        }
    }

    return 0;

}

void write_to_usart(uint8_t * input) {

    unsigned int ra = 0;

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;
    uint32_t usart_tdr = USART_REGISTER + USART_TDR_OFFSET;

    // Check if the USART is enabled
    if (READ_REG(usart_cr1) & 0x1){

        // Loop over all characters until Zero is encountered
        while (input[ra] != 0x0) {
            // Check if TXE-Bit is 1
            // Otherwise the Write-Buffer is not ready to be written to
            if (CHECK_BIT_MASK(READ_REG(usart_isr), USART_TRANS_REG_EMPTY)){
                // Append the Character to the Output-Buffer
                WRITE_REG(usart_tdr,input[ra]);
                ra ++;
            }
        }

    }

    wait_for_trans_complete(0x1);

}

void write_arr_to_usart(uint32_t * input) {

    unsigned int ra = 0;

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;
    uint32_t usart_tdr = USART_REGISTER + USART_TDR_OFFSET;

    // Check if the USART is enabled
    if (READ_REG(usart_cr1) & 0x1){

        // Make sure that the first Byte isn't Zero
        if (input[ra] != 0x0){

            // Loop over all characters until Zero is encountered
            while (input[ra] != 0x0) {
                // Check if TXE-Bit is 1
                // Otherwise the Write-Buffer is not ready to be written to
                if (CHECK_BIT_MASK(READ_REG(usart_isr), USART_TRANS_REG_EMPTY)){
                    WRITE_REG(usart_tdr,input[ra] & 0x000000FF);
                    ra++;
                }
            }

        }

    }

    wait_for_trans_complete(0x1);

}

void write_int_to_usart(uint32_t reg,uint32_t base){

    // Check that Base is bigger than 2 and smaller than the Integer-Size
    if(!((base > 2) & (base < INTEGER_SIZE - 1))){
        return;
    }

    // Initialize String with Zeroes and enough Space
    // INTEGER_SIZE + 1
    // Number in Binary + Null-Termination
    uint8_t str[INTEGER_SIZE + 1] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0
    };

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;
    uint32_t usart_tdr = USART_REGISTER + USART_TDR_OFFSET;

    // Convert Number to String ------------------------------------------------

    itoa(reg,base,str);

    // Send String via USART ---------------------------------------------------

    uint32_t ra = 0;

    // Check if the USART is enabled
    if (READ_REG(usart_cr1) & 0x1){

        // Loop over all characters until Zero is encountered
        while (str[ra] != 0x0) {
            // Check if TXE-Bit is 1
            // Otherwise the Write-Buffer is not ready to be written to
            if (CHECK_BIT_MASK(READ_REG(usart_isr), USART_TRANS_REG_EMPTY)){
                WRITE_REG(usart_tdr,str[ra] & 0x000000FF);
                ra ++;
            }

        }

    }

    wait_for_trans_complete(0x1);

}

void write_int_bin_to_usart(uint32_t reg) {

    unsigned int ra = 0;

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;
    uint32_t usart_tdr = USART_REGISTER + USART_TDR_OFFSET;

    // Check if the USART is enabled
    if (READ_REG(usart_cr1) & 0x1){

        // Loop through all Bits one by one
        while (ra <= 31) {

            // Check if TXE-Bit is 1
            // Otherwise the Write-Buffer is not ready to be written to
            if (CHECK_BIT_MASK(READ_REG(usart_isr), USART_TRANS_REG_EMPTY)){
                // Put Bit into Write-Register
                WRITE_REG(usart_tdr,ASCII_START_NUMBERS + ((reg >> (31 - (ra++))) & 0x01));
                // Add Whitespace between every Nibble
                if ((ra & 0x03) == 0x00) {
                    write_bit_to_usart(0x20);
                }
            }

        }

    }

    wait_for_trans_complete(0x1);

}

void write_int_hex_to_usart(uint32_t reg) {

    unsigned int ra = 0;

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;
    uint32_t usart_tdr = USART_REGISTER + USART_TDR_OFFSET;

    // Check if the USART is enabled
    if (READ_REG(usart_cr1) & 0x1){

        // Write "0x" over USART to indicate Hex-Number
        write_bit_to_usart(ASCII_START_NUMBERS);
        write_bit_to_usart(ASCII_START_LOWERCASE + 23);

        // Loop through all Bytes one by one
        while (ra < 8) {

            // Check if TXE-Bit is 1
            // Otherwise the Write-Buffer is not ready to be written to
            if (CHECK_BIT_MASK(READ_REG(usart_isr), USART_TRANS_REG_EMPTY)){
                // Put Byte into Write-Register as Hex
                WRITE_REG(usart_tdr,INT_TO_HEX((reg >> (4 * (7 - ra))) & 0x0F));
                ra ++;
            }

        }

    }

    wait_for_trans_complete(0x1);

}

void write_bit_to_usart(uint32_t input) {

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;
    uint32_t usart_tdr = USART_REGISTER + USART_TDR_OFFSET;

    // Check if the USART is enabled
    if (READ_REG(usart_cr1) & 0x1){
        while(1){
            // Check if TXE-Bit is 1
            // Otherwise the Write-Buffer is not ready to be written to
            if (CHECK_BIT_MASK(READ_REG(usart_isr), USART_TRANS_REG_EMPTY)) {
                // Append the Data the Write-Buffer
                WRITE_REG(usart_tdr,input & 0xFF);
                return;
            }
        }
    }

    wait_for_trans_complete(0x1);

}

void write_string_to_usart_int(uint32_t size,uint8_t * buffer){

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;
    uint32_t usart_tdr = USART_REGISTER + USART_TDR_OFFSET;

    buffer_size = size;
    write_buffer = buffer;

    // Check if the USART is enabled
    if (READ_REG(usart_cr1) & 0x1){
        while(1){
            // Check if TXE-Bit is 1
            // Otherwise the Write-Buffer is not ready to be written to
            if (CHECK_BIT_MASK(READ_REG(usart_isr), USART_TRANS_REG_EMPTY)) {
                // Append the Data the Write-Buffer
                WRITE_REG(usart_tdr,write_buffer[buffer_size--] & 0xFF);
                return;
            }
        }
    }

    ENABLE_TXE_INT;

}

void write_idle_frame(void) {

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;

    // Check if the USART is enabled
    if (CHECK_BIT_MASK(READ_REG(usart_cr1), 0x01)) {
        // Pulse TE-Register => Sends IDLE-Frame for some reason
        CLEAR_BIT(usart_cr1,0x08);
        SET_BIT(usart_cr1,0x08);
    }

}

void wait_for_trans_complete(uint8_t clear_flag) {

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;

    // Check if the USART is enabled
    if (CHECK_BIT_MASK(READ_REG(usart_cr1), 0x01)) {

        // Wait until Transmission-Complete Flag is set
        while(!(CHECK_BIT_MASK(READ_REG(usart_isr),USART_TRANS_COMP)));

        // Clear Transmission if specified
        if (clear_flag) CLEAR_USART_TC_FLAG;

    }

    // Return from Subroutine
    return;

}
