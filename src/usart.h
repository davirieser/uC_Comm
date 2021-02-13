
#pragma once

#include <stdint.h>

#include "../main.h"

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

// -----------------------------------------------------------------------------

uint32_t read_from_usart(void);
void write_to_usart(uint8_t * input);
void set_terminal_color(const char * color);
void write_to_usart_colored(const char * color, uint8_t * input);
void write_arr_to_usart(uint32_t * input);
void write_int_to_usart(uint32_t base, uint32_t reg);
void write_int_bin_to_usart(uint32_t reg);
void write_int_hex_to_usart(uint32_t reg);
void write_bit_to_usart(uint32_t input);
void write_string_to_usart_int(uint32_t size,uint8_t * buffer);
void write_idle_frame(void);
void wait_for_trans_complete(uint8_t clear_flag);
// void reset_terminal(void);
