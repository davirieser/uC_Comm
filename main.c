#include "main.h"

int not_main(void);

uint32_t data_packets_rem = 0;
uint32_t buffer_size = 0;
uint8_t * write_buffer;

int not_main(void) {

    setup_registers();

    unsigned int ra = 0;

    // Wait For Interrupt in an endless Loop
    while(1) {

        // Small Delay
        for(ra=0;ra<400000;ra++) dummy(ra);
        // Toggle User-LED
        write_to_user_led(~read_from_user_led());

    }

    return 0;

}

void setup_registers(void) {

    NO_INTERRUPTS();

    setup_clock();
    setup_pins();
    setup_usart();
    setup_user_led();
    setup_interrupt();

    INTERRUPTS();

    // Starts using Interrupt
    setup_adc();

}

uint32_t setup_pins(void){

    uint32_t outt_register = PORT_REGISTER + GPIO_OTYPER_REGISTER_OFFSET;
    uint32_t pupd_register = PORT_REGISTER + GPIO_PUPDR_REGISTER_OFFSET;
    uint32_t mode_register = PORT_REGISTER + GPIO_MODER_REGISTER_OFFSET;
    uint32_t aflr_register = PORT_REGISTER + GPIO_AFRH_REGISTER_OFFSET;
    uint32_t afhr_register = PORT_REGISTER + GPIO_AFRH_REGISTER_OFFSET;
    uint32_t lock_register = PORT_REGISTER + GPIO_LCKR_REGISTER_OFFSET;

    uint32_t iLauf;
    uint32_t pins[NUMBER_PINS] = PIN_NUMBERS;

    // Lock Registers so they cannot be changed unless there is a Reset
    uint32_t locked_registers = 0;

    // Create Bit-Mask to lock all the supplied Register
    for (iLauf = 0;iLauf < NUMBER_PINS;iLauf ++) {
        locked_registers += (1 << (pins[iLauf]));
    }

    // Check that none of the Registers are locked
    if ((READ_REG(lock_register) & locked_registers) == 0){

        // Run through all the supplied Pins
        for (iLauf = 0;iLauf < NUMBER_PINS;iLauf ++) {

            // Check if the Pin is valid to avoid Overflow-Error / Writing to the other Registers
            if ((pins[iLauf] < 16) & (pins[iLauf] >= 0)) {

                // Change GPIO's Output Type to be Push-Pull
                CLEAR_BIT(outt_register,0x3 << pins[iLauf]);

                // Disable Pull-up and Pull-Down-Resistors
                CLEAR_BIT(pupd_register,0x3 << (2 * pins[iLauf]));

                // Change GPIO's into Alternate Function-Mode
                CLEAR_BIT(mode_register,0x3 << (2 * pins[iLauf]));
                SET_BIT(mode_register,0x2 << (2 * pins[iLauf]));

                // Configure Alternate-Function
                if (pins[iLauf] <= 7){
                    // Write to the Alternate-Funtion-Low-Regiter if the Pin-Number is smaller or equal 7
                    CLEAR_BIT(aflr_register,(0xF << (pins[iLauf]*4)));
                    SET_BIT(aflr_register,(ALTERNATE_FUNCTION << (pins[iLauf]*4)));
                }else if (pins[iLauf] <= 15){
                    // Write to the Alternate-Funtion-High-Regiter if the Pin-Number is smaller or equal 15 ( to avoid Overflows)
                    CLEAR_BIT(afhr_register,(0xF << ((pins[iLauf] - 8)*4)));
                    SET_BIT(afhr_register,(ALTERNATE_FUNCTION << ((pins[iLauf] - 8)*4)));
                }

            }

        }

        // Lock Key Write Sequence according to manual
        SET_BIT(lock_register,1 << 16); // Set the 16th Bit
        CLEAR_BIT(lock_register,1 << 16); // Clear the 16th Bit
        SET_BIT(lock_register,1 << 16); // Set the 16th Bit
        if ((READ_REG(lock_register) >> 16) & 0x1) { // Then check if the 16th Bit is set
            // Successeful
            return 0;
        }else{
            // Lock Key Write Sequence aborted
            return 1;
        }

    }

    return 1;

}

void setup_clock(void) {

    uint32_t rcc_cr = RCC_REGISTER_OFFSET + RCC_CR_REG;
    uint32_t rcc_cfgr = RCC_REGISTER_OFFSET + RCC_CFGR_OFFSET;
    uint32_t rcc_cfgr3 = RCC_REGISTER_OFFSET + RCC_CFGR3_OFFSET;
    uint32_t rcc_apbenr = RCC_REGISTER_OFFSET + RCC_APBENR_OFFSET;
    uint32_t rcc_apb2enr = RCC_REGISTER_OFFSET + RCC_APB2ENR_OFFSET;

    // Enable HSI-Clock
    SET_BIT(rcc_cr,0x01);

    // Configure System-Clock to be HSI = 8MHz
    // Also set AHB and HCLK-Prescaler to not divide Clock
    // => USART-Clock = HSI-Clock = HCLK = PCLK = 8MHz
    CLEAR_BIT(rcc_cfgr,0x000007F3);

    // Enable Clock for GPIO-Ports A and C
    CLEAR_BIT(rcc_apbenr,0x000F0000);
    SET_BIT(rcc_apbenr,0x000A0000);

    // Enable Clock for USART1 and ADC
    SET_BIT(rcc_apb2enr,0x00004200);

    // Select USART Clock-Source as System-Clock
    CLEAR_BIT(rcc_cfgr3,0x03);
    SET_BIT(rcc_cfgr3,0x01);

}

void setup_usart(void){

    uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
    uint32_t usart_cr2 = USART_REGISTER + USART_CR2_OFFSET;
    uint32_t usart_cr3 = USART_REGISTER + USART_CR3_OFFSET;
    uint32_t usart_brr = USART_REGISTER + USART_BRR_OFFSET;

    // Disable USART for Configuration
    DISABLE_USART;


    // Set USART-Baud-Rate Divider
    // Baud = f_clk / USARTDIV
    // Baudrate-Divider = 208 = 0xD0
    // 8 000 000 / 208 = 38 400 (ungefaehr)
    WRITE_REG(usart_brr,0xD0);

    // Create Config using Settings
    // usart_cr2[ADD] = 0; // Bit 31-28 // Address of the USART node
    // usart_cr2[ADD] = 0; // Bit 27-24 // Address of the USART node
    // usart_cr2[RTOEN] = 0; // Bit 23 // Receiver timeout enable
    // usart_cr2[ABRMOD] = 0; // Bit 22-21 // Auto baud rate mode
    // usart_cr2[ABREN] = 0; // Bit 20 // Auto baud rate enable => Disabled
    // usart_cr2[MSBFIRST] = 0; // Bit 19 // LSB is sent first
    // usart_cr2[DATAINV] = 0; // Bit 18 // Disable Data-Inversion
    // usart_cr2[TXINV] = 0; // Bit 17 // TX pin active level inversion
    // usart_cr2[RXINV] = 0; // Bit 16 // RX pin active level inversion
    // usart_cr2[SWAP] = 0; // Bit 15 // Swap TX/RX pins
    // Bit 14 // RESERVED
    // usart_cr2[STOP] = 0; // Bit 13-12 // STOP bits => 1 Stop Bit
    // usart_cr2[CLKEN] = 0; // Bit 11 // Clock enable
    // usart_cr2[CPOL] = 0; // Bit 10 // Clock Polarity
    // usart_cr2[CPHA] = 0; // Bit 9 // Clock Phase
    // usart_cr2[LBCL] = 0; // Bit 8 // Last bit clock pulse
    // Bit 7-5 // RESERVED
    // usart_cr2[ADDM7] = 0; // Bit 4 // 7-bit Address Detection
    // Bit 3-0 // RESERVED
    CLEAR_BIT(usart_cr2,0xFFFFBF10);

    // Create Config using Settings
    // Bit 31-25 // RESERVED
    // Bit 24 // RESERVED
    // Bit 23 // RESERVED
    // Bit 22 // RESERVED
    // Bit 21 // RESERVED
    // Bit 20 // RESERVED
    // Bit 19-17 // RESERVED
    // Bit 16 // RESERVED
    // usart_cr3[DEP] = 0; // Bit 15 // Driver enable polarity selection
    // usart_cr3[DEM] = 0; // Bit 14 // Disable Driver-Enable for RS485
    // usart_cr3[DDRE] = 0; // Bit 13 // DMA Disable on Reception Error
    // usart_cr3[OVRDIS] = 1; // Bit 12 // Overrun Disable
    // usart_cr3[ONEBIT] = 0; // Bit 11 // One sample bit method enable
    // usart_cr3[CTSIE] = 0; // Bit 10 // CTS interrupt enable
    // usart_cr3[CTSE] = 0; // Bit 9 // CTS enable
    // usart_cr3[RTSE] = 0; // Bit 8 // RTS enable
    // usart_cr3[DMAT] = 0; // Bit 7 // DMA enable transmitter
    // usart_cr3[DMAR] = 0; // Bit 6 // DMA enable receiver
    // Bit 5-4 // RESERVED
    // usart_cr3[HDSEL] = 1; // Bit 3 // Half-duplex selection => Half Duplex Mode
    // Bit 2-1 // RESERVED
    // usart_cr3[EIE] = 0; // Bit 0 // Error interrupt enable => Disabled
    CLEAR_BIT(usart_cr3,0x0000FFE9);
    SET_BIT(usart_cr3,0x000001000);

    // Create Config using Settings
    // Bit 31-29 // RESERVED
    // usart_cr1[M1] = 0; // Bit 28 // Word Length Bit => 8 Data-Bits
    // Bit 27 // RESERVED
    // usart_cr1[RTOIE] = 0; // Bit 26 // Receiver timeout interrupt enable
    // usart_cr1[DEAT] = 0; // Bit 25-21 // Driver Enable assertion time
    // usart_cr1[DEDT] = 0; // Bit 20-16 // Driver Enable de-assertion time
    // usart_cr1[OVER8] = 0; // Bit 15 // Oversampling = 16 (lower Frequency but better Tolerance)
    // usart_cr1[CMIE] = 0; // Bit 14 // Character match interrupt enable
    // usart_cr1[MME] = 0; // Bit 13 // Enable USART to go into Sleep-Mode
    // usart_cr1[M0] = 0; // Bit 12 // Word Length Bit => 8 Data-Bits
    // usart_cr1[WAKE] = 0; // Bit 11 // Tell USART to wake from Sleep on IDLE-Line
    // usart_cr1[PCE] = 0; // Bit 10 // Disable Parity-Checking
    // usart_cr1[PS] = 0; // Bit 9 // Set Parity to Even
    // usart_cr1[PEIE] = 0; // Bit 8 // Parity Error Interrupt Enable
    // usart_cr1[TXIEIE] = 0; // Bit 7 // Transmit Buffer Empty Interrupt Enable
    // usart_cr1[TCIE] = 0; // Bit 6 // Transmission Complete Interrupt Enable = TC
    // usart_cr1[RXNEIE] = 0; // Bit 5 // Enable Interrupt on Receive-Data available = RXNE
    // usart_cr1[IDLEIE] = 0; // Bit 4 // IDLE Interrupt Enable
    // usart_cr1[TE] = 1; // Bit 3 // Enable Transmitter
    // usart_cr1[RE] = 1; // Bit 2 // Enable Receiver
    // Bit 1 // RESERVED
    // usart_cr1[UE] = 0; // Bit 0 // USART enable => Disabled
    CLEAR_BIT(usart_cr1,
        (0x1 << 28)|(0x7 << 24)|(0xF << 20)|(0xF << 16)|
        (0xF << 12)|(0xF << 8)|(0xF << 4)|(0xC << 0)
    );
    SET_BIT(usart_cr1,0xC);

    ENABLE_USART;

    ENABLE_RXNE_INT;

}

void setup_adc() {

    // Port PA0 Configuration
    uint32_t mode_register = PORT_REGISTER + GPIO_MODER_REGISTER_OFFSET;

    SET_BIT(mode_register,0x3);

    // ADC-Configuration
    uint32_t adc_chselr = ADC_BASE_REG + ADC_CHSELR;
    uint32_t adc_cr = ADC_BASE_REG + ADC_CR;
    uint32_t adc_ier = ADC_BASE_REG + ADC_IER;
    uint32_t adc_isr = ADC_BASE_REG + ADC_ISR;
    uint32_t adc_cfgr1 = ADC_BASE_REG + ADC_CFGR1;
    uint32_t adc_cfgr2 = ADC_BASE_REG + ADC_CFGR2;

    // Check that no Conversion is currently going on
    while((READ_REG(adc_cr) & ADC_CR_ADCAL) != 0x0);

    // Check if ADC is already started (by checking if ADEN = 1)
    if(READ_REG(adc_cr) & 0x1){
        // Disable ADC by setting ADDIS => Clears ADEN-Bit
        // Also clears ADSTART
        SET_BIT(adc_cr,0x2);
    }

    // Check that all Bits in ADC_CR are 0
    // Otherwise the remaining Configuration won't work
    while ((READ_REG(adc_cr) & 0x80000017) != 0x0);

    // Clear ADRDY-Bit in ADC-ISR (optional)
    SET_BIT(adc_isr,0x01);

    // Set ADC clock to PCLK/2 = Sysclock/2 = HCLK/2 = 4MHz
    CLEAR_BIT(adc_cfgr2,0xC0000000);
    SET_BIT(adc_cfgr2,0x40000000);

    // Calibrate ADC
    SET_BIT(adc_cr,ADC_CR_ADCAL);

    // Wait for Calibration to finish => ADSTART-Bit = 0
    while((READ_REG(adc_cr) & ADC_CR_ADCAL) != 0x0);
    // ADSTART has to be cleared here

    // Put ADC in Continous Mode and set Overrun Mangement to overwrite Data
    CLEAR_BIT(adc_cfgr1,
        (0x7 << 28)|(0xC << 24)|(0xC << 20)|(1<<16)|
        (0xF << 12)|(0xD << 8) |(0xF << 4) |(0xF)
    );
    SET_BIT(adc_cfgr1,(3<<12));

    // Enable ADC-Ready-Interrupt of ADC
    CLEAR_BIT(adc_ier,0x9F);
    SET_BIT(adc_ier,0x01);

    //Set ADC Channel to channel 0 because PA0 is ADC_IN0
    CLEAR_BIT(adc_chselr,0x0001FFFF);
    SET_BIT(adc_chselr,0x01);

    // Enable ADC
    SET_BIT(adc_cr,0x01);

    // Wait for ADRDY
    while (READ_BIT(adc_isr,0x01));

    // Set ADSTART-Bit
    SET_BIT(adc_cr,0x04);

    return;

}

uint32_t setup_user_led(void) {

    // LED is connected to PIN PA4 and to 3V3 via a Resistor

    uint32_t led_mask = 1 << USER_LED_PIN_PORT;
    uint32_t lock_register = REGISTER_GPIO_A_START + GPIO_LCKR_REGISTER_OFFSET;

    // Check if the Port is already locked
    if ((READ_REG(lock_register) & led_mask) == 0){

        uint32_t pupd_register = REGISTER_GPIO_A_START + GPIO_PUPDR_REGISTER_OFFSET;
        uint32_t mode_register = REGISTER_GPIO_A_START + GPIO_MODER_REGISTER_OFFSET;

        // Configure Pin to be in General Purpose Output Mode
        CLEAR_BIT(mode_register,0x00000300);
        SET_BIT(mode_register,0x00000100);

        // Configure Pin to be Pull-Up (Led is connected towards Ground)
        CLEAR_BIT(pupd_register,0x00000300);
        SET_BIT(pupd_register,0x00000100);

        // Lock Key Write Sequence according to manual
        SET_BIT(lock_register,1 << 16); // Set the 16th Bit
        CLEAR_BIT(lock_register,1 << 16); // Clear the 16th Bit
        SET_BIT(lock_register,1 << 16); // Set the 16th Bit
        // Then check if the 16th Bit is set
        if ((READ_REG(lock_register) >> 16) & 0x1) {
            // Successeful
            return 0;
        }else{
            // Lock Key Write Sequence aborted
            return 1;
        }

    }else{
        return 1;
    }

}

void setup_interrupt(void) {

    uint32_t nvic_iser = NVIC_BASE + NVIC_ISER;
    // uint32_t nvic_ipr6 = NVIC_BASE + NVIC_IPR + 4;

    // Enable Interrupts
    WRITE_REG(nvic_iser,0x08001000);

    // Set Interrupt Priorities
    // WRITE_REG(nvic_ipr6,0x40000000);

}

void write_to_user_led(uint32_t mode) {

    // uint8_t * LED_ON = (uint8_t * )"Turning LED on\r\n";
    // uint8_t * LED_OFF = (uint8_t *)"Turning LED off\r\n";

    uint32_t led_register = REGISTER_GPIO_A_START + GPIO_ODR_REGISTER_OFFSET;

    // Write the mode negated because the LED is on when the Pin is low and off when the pin is low
    if (CHECK_BIT(mode,0) == 0x01){
        // write_to_usart(LED_OFF);
        CLEAR_BIT(led_register,1 << USER_LED_PIN_NUMBER);
    }else{
        // write_to_usart(LED_ON);
        SET_BIT(led_register,1 << USER_LED_PIN_NUMBER);
    }

}

uint32_t read_from_user_led(void) {

    uint32_t led_register = REGISTER_GPIO_A_START + GPIO_ODR_REGISTER_OFFSET;

    // Return bool is negated because the LED is on when the Pin is low and off when the pin is low
    return (~((READ_REG(led_register) >> USER_LED_PIN_NUMBER) & 0x01));

}

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

            // Write first Byte
            WRITE_REG(usart_tdr,input[ra] & 0x000000FF);

            // Loop over all characters until Zero is encountered
            while (input[++ra] != 0x0) {
                // Check if TXE-Bit is 1
                // Otherwise the Write-Buffer is not ready to be written to
                if (CHECK_BIT_MASK(READ_REG(usart_isr), USART_TRANS_REG_EMPTY)){
                    WRITE_REG(usart_tdr,input[ra] & 0x000000FF);
                }
            }

        }

    }

    wait_for_trans_complete(0x1);

}

// TODO Not Tested
// void write_int_to_usart(int base,int reg){
//
//     // Check that Base is bigger than 2 and smaller than the Integer-Size
//     if(!((base > 2) | (base < INTEGER_SIZE - 1))){
//         return;
//     }
//
//     uint32_t usart_cr1 = USART_REGISTER + USART_CR1_OFFSET;
//     uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;
//     uint32_t usart_tdr = USART_REGISTER + USART_TDR_OFFSET;
//
//     // Convert Number to String ------------------------------------------------
//
//     // TODO Nummer zu String konvertieren
//
//     // Send String via USART ---------------------------------------------------
//
//     ra = number_start;
//
//     // Check if the USART is enabled
//     if (READ_REG(usart_cr1) & 0x1){
//
//         // Make sure that the first Byte isn't Zero
//         if (x[ra] != 0x0){
//
//             // Write first Byte
//             WRITE_REG(usart_tdr,x[ra] & 0x000000FF);
//
//             // Loop over all characters until Zero is encountered
//             while (x[++ra] != 0x0) {
//                 // Check if TXE-Bit is 1
//                 // Otherwise the Write-Buffer is not ready to be written to
//                 if (CHECK_BIT_MASK(READ_REG(usart_isr), USART_TRANS_REG_EMPTY)){
//                     WRITE_REG(usart_tdr,x[ra] & 0x000000FF);
//                 }
//             }
//
//         }
//
//     }
//
//     wait_for_trans_complete(0x1);
//
// }

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

void USART1_IRQHandler(void) {

    // Disable Interrupts
    NO_INTERRUPTS();

    // Identify Interrupt in USART-Terminal
    uint8_t * int_id = (uint8_t * ) "USART-INT\r\n";

    write_to_usart(int_id);

    // Actual Interrupt Service Routine

    uint32_t usart_isr = USART_REGISTER + USART_ISR_OFFSET;

    uint32_t int_register = READ_REG(usart_isr);

    // Find out which Interrupt was triggered
    if ((int_register & USART_READ_DATA_NE) != 0){

        // Clear RXNE-Bit
        int_register = read_from_usart();

        // Increment Data-Packets to be sent and enable Interrupt
        data_packets_rem = DATA_PACKETS_PER_REQUEST;
        ENABLE_ADC_EOC_INT;

        // Could be included for Safety Purposes
        // Discards Read-Buffer and clears Interrupt-Flag
        // USART_FLUSH_READ_REG;
        // CLEAR_USART_RXNE_FLAG;

    }else if((int_register & USART_TRANS_REG_EMPTY) != 0){

        if (buffer_size > 1){
            write_bit_to_usart(write_buffer[buffer_size--]);
        }else if(buffer_size == 0){
            write_bit_to_usart(write_buffer[buffer_size--]);
            ENABLE_TC_INT;
            DISABLE_TXE_INT;
            CLEAR_USART_TXE_FLAG;
        }else{
            DISABLE_TXE_INT;
            ENABLE_TC_INT;
            CLEAR_USART_TXE_FLAG;
        }

    }else if((int_register & USART_TRANS_COMP) != 0) {

        CLEAR_USART_TC_FLAG;
        DISABLE_TXE_INT;
        DISABLE_TC_INT;

    }else {

        // Clear all Interrupts
        CLEAR_USART_FLAGS;

    }

    // Wait For Interrupt
    INTERRUPTS();
    // __WFI;

}

void ADC1_IRQHandler(){

    NO_INTERRUPTS();

    uint8_t * int_id = (uint8_t * )"ADC-INT\r\n";

    write_to_usart(int_id);

    uint32_t adc_ier = ADC_BASE_REG + ADC_IER;
    uint32_t adc_isr = ADC_BASE_REG + ADC_ISR;
    uint32_t adc_cr = ADC_BASE_REG + ADC_CR;
    uint32_t adc_dr = ADC_BASE_REG + ADC_DR;

    uint32_t REG_CONTENT = READ_REG(adc_isr);

    // ADC is ready
    if(((REG_CONTENT & ADC_ISR_ADRDY) == ADC_ISR_ADRDY) & (READ_BIT(adc_ier,0x01))){
        write_to_usart((uint8_t * ) "ADC-Initialization Complete\r\n");
        // Start Conversion
        SET_BIT(adc_cr,ADC_CR_ADSTART);
        // Disable ADC-Ready-Interrupt
        CLEAR_BIT(adc_ier,0x01);
    }
    // End of Conversion
    else if((REG_CONTENT & ADC_ISR_EOC) == ADC_ISR_EOC){
        if (data_packets_rem > 0){
            // Send Message at the Start
            write_to_usart((uint8_t * ) "ADC-Value : ");
            // Read ADC-Value and send it in Hex
            REG_CONTENT = (READ_REG(adc_dr) & 0xFFFF);
            write_int_hex_to_usart(REG_CONTENT);
            // Send Newline and Carriage-Return
            write_bit_to_usart(NEWLINE);
            write_bit_to_usart(CARRIAGE_RT);
            // Decrement Data Packets remaining
            data_packets_rem --;
            write_int_hex_to_usart(data_packets_rem);
            // Send Newline and Carriage-Return
            write_bit_to_usart(NEWLINE);
            write_bit_to_usart(CARRIAGE_RT);
        }else{
            DISABLE_ADC_EOC_INT;
            write_bit_to_usart(NEWLINE);
            write_bit_to_usart(CARRIAGE_RT);
        }
    }else{
        DISABLE_ADC_EOC_INT;
    }

    INTERRUPTS();

}
