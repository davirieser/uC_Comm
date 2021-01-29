
#pragma once

#include <stdint.h>

#include "../main.h"

uint32_t read_from_usart(void);
void write_to_usart(uint8_t * input);
void write_arr_to_usart(uint32_t * input);
void write_int_to_usart(uint32_t base, uint32_t reg);
void write_int_bin_to_usart(uint32_t reg);
void write_int_hex_to_usart(uint32_t reg);
void write_bit_to_usart(uint32_t input);
void write_string_to_usart_int(uint32_t size,uint8_t * buffer);
void write_idle_frame(void);
void wait_for_trans_complete(uint8_t clear_flag);
