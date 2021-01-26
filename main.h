
#pragma once

#include <stdint.h>

void dummy ( unsigned int );
void INTERRUPTS ( void );
void NO_INTERRUPTS ( void );

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Custom Defines --------------------------------------------------------------

#define STATE_IDLE 0
#define STATE_SENDING 1

#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_F 4

#define NUMBER_PINS 2
#define PIN_NUMBERS {9,10}
#define ALTERNATE_FUNCTION 0x1
#define DATA_PACKETS_PER_REQUEST 10
#define USART_NUMBER 1
#define PORT_IDENTIFIER GPIO_A

#define USER_LED_PIN_PORT GPIO_A
#define USER_LED_PIN_NUMBER 4

#define ASCII_START_NUMBERS 0x30
#define ASCII_START_UPPERCASE 0x41
#define ASCII_START_LOWERCASE 0x61

#define NEWLINE 0x0A
#define CARRIAGE_RT 0x0D

#define INTEGER_SIZE 32
#define BYTE_SIZE 8
#define NIBBLE_SIZE 4

// -----------------------------------------------------------------------------
// Macros ----------------------------------------------------------------------

#define SET_BIT(REG, BIT)     ((*(volatile uint32_t *)(REG)) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((*(volatile uint32_t *)(REG)) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((*(volatile uint32_t *)(REG)) & (BIT))
#define WRITE_BIT(REG, BIT_POS, BIT_VAL) (BIT_VAL ? SET_BIT(REG, BIT_POS) : CLEAR_BIT(REG, BIT_POS))

#define CLEAR_REG(REG)        ((*(volatile uint32_t *)(REG)) = (0x0))
#define WRITE_REG(REG, VAL)   ((*(volatile uint32_t *)(REG)) = (VAL))
#define READ_REG(REG)         ((*(volatile uint32_t *)(REG)))

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define CHECK_BIT_MASK(var,comp) ((var & comp) == comp)

#define INT_TO_HEX(int) (( \
    (int & 0x0F) <= 9) ? \
    ((int & 0x0F) + ASCII_START_NUMBERS) : \
    ((int & 0x0F) + ASCII_START_UPPERCASE - 10) \
)

#ifndef HIGH
    #define HIGH 0x1
#endif
#ifndef LOW
    #define LOW  0x0
#endif

#ifndef _NOP
    #define _NOP() asm volatile ("nop");
#endif

#define __WFI asm volatile ("wfi");

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Clock Register Addresses

#define RCC_REGISTER_OFFSET 0x40021000
#define RCC_REGISTER_END 0x400213FF

#define RCC_CR_REG  0x0
#define RCC_CFGR_OFFSET 0x00000004
#define RCC_PERIPH_RESET 0x0000000C
#define RCC_APBENR_OFFSET 0x00000014
#define RCC_APB2ENR_OFFSET 0x00000018
#define RCC_CFGR3_OFFSET 0x00000030

// -----------------------------------------------------------------------------
// NVIC Control Register

#define NVIC_BASE 0xE000E100
#define NVIC_ISER 0x0 // Interrupt set-enable register
#define NVIC_ICER 0x080 // Interrupt clear-enable register
#define NVIC_ISPR 0x0100 // Interrupt set-pending register
#define NVIC_ICPR 0x0180 // Interrupt clear-pending register
#define NVIC_IPR 0x0300 // Interrupt priority register

// -----------------------------------------------------------------------------
// ADC-Addresses

//boundary address at page 40
#define ADC_BASE_REG 0x40012400
//Page 220
#define ADC_ISR         0x00      //ADC interrupt and status register -- at Page 207
#define ADC_IER         0x04      //ADC interrupt enable register -- at Page 208
#define ADC_CR          0x08      //ADC control register -- at Page 210
#define ADC_CFGR1       0x0C      //ADC configuration register 1 -- at Page 212
#define ADC_CFGR2       0x10      //ADC configuration register 2 -- at Page 216
#define ADC_SMPR        0x14      //ADC sampling time register -- at Page 216
#define ADC_TR          0x20      //ADC watchdog threshold register -- at Page 217
#define ADC_CHSELR      0x28      //ADC channel selection register -- at Page 218
#define ADC_DR          0x40      //ADC data register -- at Page 218
#define ADC_CCR         0x308     //ADC common configuration register -- at Page 219

#define ADC_CR_ADCAL    0x80000000  //ADC Calibration
#define ADC_CR_ADSTP    0x00000010  //ADC stop of conversion command
#define ADC_CR_ADSTART  0x00000004  //ADC start of conversion
#define ADC_CR_ADDIS    0x00000002  //ADC disable command
#define ADC_CR_ADEN      0x00000001 //ADC enable control

#define ADC_ISR_AWD     0x00000080        //Analog watchdog flag
#define ADC_ISR_OVR     0x00000010        //Overrun flag
#define ADC_ISR_EOSEQ   0x00000008        //End of Sequence flag
#define ADC_ISR_EOC     0x00000004        //End of Conversion
#define ADC_ISR_EOSMP   0x00000002        //End of sampling flag
#define ADC_ISR_ADRDY   0x00000001        //ADC Ready

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
// USART Register Definitions

#if USART_NUMBER == 1
    #define USART_REGISTER REGISTER_USART1_START
    #define INTERRUPT_OFFSET USART_1_INTERRUPT_OFFSET
#elif USART_NUMBER == 2
    #define USART_REGISTER REGISTER_USART2_START
    #define INTERRUPT_OFFSET USART_2_INTERRUPT_OFFSET
#elif USART_NUMBER == 3
    #define USART_REGISTER REGISTER_USART3_START
    #define INTERRUPT_OFFSET USART_3_INTERRUPT_OFFSET
#elif USART_NUMBER == 4
    #define USART_REGISTER REGISTER_USART4_START
    #define INTERRUPT_OFFSET USART_4_INTERRUPT_OFFSET
#elif USART_NUMBER == 5
    #define USART_REGISTER REGISTER_USART5_START
    #define INTERRUPT_OFFSET USART_5_INTERRUPT_OFFSET
#elif USART_NUMBER == 6
    #define USART_REGISTER REGISTER_USART6_START
    #define INTERRUPT_OFFSET USART_6_INTERRUPT_OFFSET
#else
    // Use USART1 as default
    #define USART_REGISTER REGISTER_USART1_START
    #define INTERRUPT_OFFSET USART_1_INTERRUPT_OFFSET
#endif

#define USART_CR1_OFFSET 0x00000000 // Control Register 1
#define USART_CR2_OFFSET 0x00000004 // Control Register 2
#define USART_CR3_OFFSET 0x00000008 // Control Register 3
#define USART_BRR_OFFSET 0x0000000C // Baud rate register
#define USART_RTOR_OFFSET 0x00000014 // Receiver timeout register
#define USART_RQR_OFFSET 0x00000018 // Request register
#define USART_ISR_OFFSET 0x0000001C // Interrupt and status register
#define USART_ICR_OFFSET 0x00000020 // Interrupt flag clear register
#define USART_RDR_OFFSET 0x00000024 // Receive data register
#define USART_TDR_OFFSET 0x00000028 // Transmit data register

// -----------------------------------------------------------------------------
// Vector Table Addresses

#define VECTOR_TABLE_OFFSET_REGISTER 0xE000ED08

#define USART_1_INTERRUPT_OFFSET 0x000000AC
#define USART_2_INTERRUPT_OFFSET 0x000000B0
#define USART_3_INTERRUPT_OFFSET 0x000000B4
#define USART_4_INTERRUPT_OFFSET 0x000000B4
#define USART_5_INTERRUPT_OFFSET 0x000000B4
#define USART_6_INTERRUPT_OFFSET 0x000000B4

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

// Interrupt-Bitmasks ----------------------------------------------------------

#define USART_PARITY_ERROR 0x00000001       // Parity Error
#define USART_FRAMING_ERROR 0x00000002      // Framing Error
#define USART_START_BIT_NOISE 0x00000004    // Start Bit Noise Detection
#define USART_OVERRUN_ERROR 0x00000008      // Overrun Error
#define USART_IDLE_DETECTED 0x00000010      // Idle Line Detected
#define USART_READ_DATA_NE 0x00000020       // Read Data Register Not Empty
#define USART_TRANS_COMP 0x00000040         // Transmission Complete => Send new Data-Packet
#define USART_TRANS_REG_EMPTY 0x00000080    // Transmit data register empty
#define USART_CTS_INT 0x00000200            // CTS interrupt flag
#define USART_CTS_FLAG 0x00000400           // CTS flag
#define USART_RECVER_TIMEOUT 0x00000800     // Receiver timeout
#define USART_AUTO_BRR_ERR 0x00004000       // Auto baud rate error
#define USART_AUTO_BRR_FLAG 0x00008000      // Auto baud rate flag
#define USART_BUSY_FLAG 0x00010000          // Busy flag
#define USART_CHAR_MATCH_FLAG 0x00020000    // Character match flag
#define USART_BREAK_FLAG 0x00040000         // Send break flag
#define USART_RECVER_WKUP 0x00080000        // Receiver wakeup from Mute mode

#define USART_ALL_FLAGS { \
    USART_PARITY_ERROR|USART_FRAMING_ERROR| \
    USART_START_BIT_NOISE|USART_OVERRUN_ERROR|USART_IDLE_DETECTED| \
    USART_READ_DATA_NE|USART_TRANS_COMP|USART_TRANS_REG_EMPTY|USART_CTS_INT| \
    USART_CTS_FLAG|USART_RECVER_TIMEOUT|USART_AUTO_BRR_ERR|USART_AUTO_BRR_FLAG| \
    USART_BUSY_FLAG|USART_CHAR_MATCH_FLAG|USART_BREAK_FLAG|USART_RECVER_WKUP;\
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Miscellaneous Addresses

#define CODE_MEMORY_START 0x00000000
#define CODE_MEMORY_END 0x20000000

#define PERIPHERAL_MEMORY_START 0x40000000
#define PERIPHERAL_MEMORY_END 0x0480017FF

#define SYSTEM_MEMORY_START 0x1FFFEC00
#define SYSTEM_MEMORY_END 0x1FFFF800

#define APB1_MEMORY_START 0x40000000
#define APB1_MEMORY_END 0x40008000
#define APB2_MEMORY_START 0x40010000
#define APB2_MEMORY_END 0x40018000

// Port-Register ---------------------------------------------------------------

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

// USART-Register --------------------------------------------------------------

#define REGISTER_USART1_START 0x40013800
#define REGISTER_USART1_END 0x40013BFF

#define REGISTER_USART2_START 0x40004400
#define REGISTER_USART2_END 0x400047FF

#define REGISTER_USART3_START 0x40004800
#define REGISTER_USART3_END 0x40004BFF

#define REGISTER_USART4_START 0x40004C00
#define REGISTER_USART4_END 0x40004FFF

#define REGISTER_USART5_START 0x40005000
#define REGISTER_USART5_END 0x400053FF

#define REGISTER_USART6_START 0x40011400
#define REGISTER_USART6_END 0x400117FF

// -----------------------------------------------------------------------------

#define MEMORY_DMA_START 0x40020000
#define MEMORY_DMA_END 0x400203FF

// -----------------------------------------------------------------------------

void setup_registers(void);
uint32_t setup_pins(void);
void setup_clock(void);
void setup_usart(void);
void setup_adc();
uint32_t setup_user_led(void);
void setup_interrupt(void);
void write_to_user_led(uint32_t mode);
uint32_t read_from_user_led(void);
uint32_t read_from_usart(void);
void write_to_usart(uint8_t * input);
void write_arr_to_usart(uint32_t * input);
void write_int_to_usart(int base, int reg);
void write_int_bin_to_usart(uint32_t reg);
void write_int_hex_to_usart(uint32_t reg);
void write_bit_to_usart(uint32_t input);
void write_string_to_usart_int(uint32_t size,uint8_t * buffer);
void write_idle_frame(void);
void wait_for_trans_complete(uint8_t clear_flag);
void USART1_IRQHandler(void);
void ADC1_IRQHandler();

// -----------------------------------------------------------------------------

#define ENABLE_USART SET_BIT(USART_REGISTER + USART_CR1_OFFSET,0x01);
#define DISABLE_USART CLEAR_BIT(USART_REGISTER + USART_CR1_OFFSET,0x01);

#define ENABLE_USART_TRANS SET_BIT(USART_REGISTER + USART_CR1_OFFSET,0x08)
#define DISABLE_USART_TRANS CLEAR_BIT(USART_REGISTER + USART_CR1_OFFSET,0x08)
#define ENABLE_USART_RECV SET_BIT(USART_REGISTER + USART_CR1_OFFSET,0x04)
#define DISABLE_USART_RECV CLEAR_BIT(USART_REGISTER + USART_CR1_OFFSET,0x04)

#define CLEAR_USART_TC_FLAG SET_BIT(USART_REGISTER + USART_ICR_OFFSET,USART_TRANS_COMP);
#define CLEAR_USART_TXE_FLAG SET_BIT(USART_REGISTER + USART_ICR_OFFSET,USART_TRANS_REG_EMPTY);
#define CLEAR_USART_RXNE_FLAG SET_BIT(USART_REGISTER + USART_ICR_OFFSET,USART_READ_DATA_NE);
#define CLEAR_USART_FLAGS SET_BIT(USART_REGISTER + USART_ICR_OFFSET,USART_ALL_FLAGS);

#define ENABLE_TC_INT SET_BIT(USART_REGISTER + USART_CR1_OFFSET,USART_TRANS_COMP);
#define DISABLE_TC_INT CLEAR_BIT(USART_REGISTER + USART_CR1_OFFSET,USART_TRANS_COMP);
#define ENABLE_RXNE_INT SET_BIT(USART_REGISTER + USART_CR1_OFFSET,USART_READ_DATA_NE);
#define DISABLE_RXNE_INT CLEAR_BIT(USART_REGISTER + USART_CR1_OFFSET,USART_READ_DATA_NE);
#define ENABLE_TXE_INT SET_BIT(USART_REGISTER + USART_CR1_OFFSET,USART_TRANS_REG_EMPTY);
#define DISABLE_TXE_INT CLEAR_BIT(USART_REGISTER + USART_CR1_OFFSET,USART_TRANS_REG_EMPTY);

#define ENABLE_ADC_READY_INT SET_BIT(ADC_BASE_REG + ADC_IER,0x01)
#define DISABLE_ADC_READY_INT CLEAR_BIT(ADC_BASE_REG + ADC_IER,0x01)
#define ENABLE_ADC_EOC_INT SET_BIT(ADC_BASE_REG + ADC_IER,0x04)
#define DISABLE_ADC_EOC_INT CLEAR_BIT(ADC_BASE_REG + ADC_IER,0x04)

#define USART_FLUSH_READ_REG SET_BIT(USART_REGISTER + USART_RQR_OFFSET,0x08)

#define RESET_USART() { \
    SET_BIT(RCC_REGISTER_OFFSET + RCC_PERIPH_RESET,1 << 14); \
    READ_REG(RCC_REGISTER_OFFSET + RCC_PERIPH_RESET); \
    CLEAR_BIT(RCC_REGISTER_OFFSET + RCC_PERIPH_RESET,1 << 14); \
}
