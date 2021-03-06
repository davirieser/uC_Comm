#include "main.h"

uint8_t * write_buffer;
uint32_t buffer_size;

uint32_t data_packets_rem = 0;
const struct PinDefinition led_pin = {.port=GPIO_A,.pin_number=4};
const struct shift_reg sr = {
    .rclk_port=GPIO_B,
    .rclk_pin=1,
    .data_port=GPIO_A,
    .data_pin=5,
    .sclk_port=GPIO_A,
    .sclk_pin=6
};
// const struct tm1637 display = {
//     .clk_port=GPIO_A,
//     .clk_pin=3,
//     .data_port=GPIO_A,
//     .data_pin=2
// };

uint32_t not_main(void);

uint32_t not_main(void) {

    setup_registers();

    uint32_t ra = 0;
    uint32_t rb = 0;

    struct PinConfig config = {
        .otyper=GPIO_OTYPE_PUSH_PULL,
        .moder=GPIO_MODE_GPIO,
        .ospeedr=GPIO_SPEED_LOW,
        .pupd=GPIO_PUPD_PULL_DOWN,
        .alt_fun=0x00,
        .lock=0x00
    };

    // Wait For Interrupt in an endless Loop
    while(1) {

        for (rb = 1; rb < 4; rb++) {

            // Small Delay
            for(ra=0;ra<42734;ra++) dummy(ra);
            // Toggle User-LED
            // write_to_user_led(~read_from_user_led());

        }

        // set_segments(display,segments,2);

        // shift_out_clk(
        //     (struct PinDefinition){
        //         .port=display.data_port,
        //         .pin_number=display.data_pin
        //     },
        //     (struct PinDefinition){
        //         .port=display.clk_port,
        //         .pin_number=display.clk_pin
        //     },
        //     0x88,
        //     8,
        //     _bitdelay
        // );

        // shift_out_clk(
        //     (struct PinDefinition){
        //         .port=display.data_port,
        //         .pin_number=display.data_pin
        //     },
        //     (struct PinDefinition){
        //         .port=display.clk_port,
        //         .pin_number=display.clk_pin
        //     },
        //     rb,
        //     0x08,
        //     _bitdelay
        // );
        //
        // rb++;

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

    // Setup USART-Pins
    struct PinDefinition pins[] = {
        {.port=GPIO_A,.pin_number=9},
        {.port=GPIO_A,.pin_number=10},
    };
    struct PinConfig config = {
        .otyper=GPIO_OTYPE_PUSH_PULL,
        .moder=GPIO_MODE_ALT_FUN,
        .ospeedr=GPIO_SPEED_LOW,
        .pupd=GPIO_PUPD_NO_PUPD,
        .alt_fun=GPIO_ALTERNATE_FUNCTION1,
        .lock=0x00
    };
    setupPins(pins,config,2);

    // setup_tm(display);
    setup_shift_reg(sr);

    // Setup PA0 to be Analog-Pin for ADC
    struct PinDefinition pin = {.port=GPIO_A,.pin_number=0};
    config = (struct PinConfig){
        .otyper=0x00,
        .moder=GPIO_MODE_ANALOG,
        .ospeedr=0x00,
        .pupd=0x00,
        .alt_fun=0x00,
        .lock=0x00
    };
    setupPin(pin,config);

    // LED is connected to PIN PA4 and to 3V3 via a Resistor
    config = (struct PinConfig){
        .otyper=GPIO_OTYPE_PUSH_PULL,
        .moder=GPIO_MODE_GPIO,
        .ospeedr=GPIO_SPEED_LOW,
        .pupd=GPIO_PUPD_PULL_UP,
        .alt_fun=0x00,
        .lock=0x00
    };
    setupPin(led_pin,config);

    return 0;

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

    return 0;

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

    // Write the Value negated because the LED is on when the Pin is low
    // and off when the pin is high
    write_to_pin(led_pin,(~mode) & 0x01);

}

uint32_t read_from_user_led(void) {

    // Return bool is negated because the LED is on when the Pin is low
    // and off when the pin is high
    return (~read_from_pin(led_pin)) & 0x01;

}

void _bitdelay() {

    uint32_t iLauf;

    for(iLauf = 0; iLauf < 20000; iLauf ++) dummy(iLauf);

}

void USART1_IRQHandler(void) {

    // Disable Interrupts
    NO_INTERRUPTS();

    set_terminal_color(BLUE);

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

        write_to_shift_reg(sr,int_register,8);

        write_int_bin_to_usart(int_register);
        write_bit_to_usart(NEWLINE);
        write_bit_to_usart(CARRIAGE_RT);

        // Increment Data-Packets to be sent and enable Interrupt
        // data_packets_rem = DATA_PACKETS_PER_REQUEST;
        // ENABLE_ADC_EOC_INT;

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

    set_terminal_color(RED);

    uint8_t * int_id = (uint8_t * )"ADC-INT\r\n";
    // Indicate that the ADC triggered an Interrupt on the Terminal
    write_to_usart(int_id);

    uint32_t adc_ier = ADC_BASE_REG + ADC_IER;
    uint32_t adc_isr = ADC_BASE_REG + ADC_ISR;
    uint32_t adc_cr = ADC_BASE_REG + ADC_CR;
    uint32_t adc_dr = ADC_BASE_REG + ADC_DR;

    uint32_t REG_CONTENT = READ_REG(adc_isr);

    // ADC-Ready Interrupt ( cannot be cleared so check INT-Enable)
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
