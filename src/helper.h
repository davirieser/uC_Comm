
#pragma once

#include <stdint.h>

#include "../main.h"

struct div_result_struct{
    uint32_t div;
    uint32_t mod;
};

void itoa(uint32_t input, uint32_t base, uint8_t * s);
struct div_result_struct divide_with_mod(uint32_t a, uint32_t b);
void reverse_arr(uint32_t * arr, uint32_t length);
void reverse_str(uint8_t * arr, uint32_t length);
