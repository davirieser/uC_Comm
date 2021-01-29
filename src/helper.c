#include "helper.h"

// Helper Functions ------------------------------------------------------------

// Convert Integer to String in Decimal
// Ignores sign
// Supports Base up to 36
// Expects INTEGER_SIZE + 1 sized String
void itoa(uint32_t input, uint32_t base, uint8_t * s) {

    uint32_t i = 0;
    struct div_result_struct div_result;

    div_result.div = input;

    // Generate Digits in reverse Order
    do {
        // Get next Digit
        div_result = divide_with_mod(div_result.div,base);
        // Add Digit converted to right Base (up to Base 36)
        s[i++] = INT_TO_HEX(div_result.mod);
        // write_to_usart((uint8_t *) "\r\n\tDivision :");
        // write_int_hex_to_usart(div_result.div);
        // write_to_usart((uint8_t *) " , Modulo :");
        // write_int_hex_to_usart(div_result.mod);
    }while (div_result.div > 0);

    // Add Null-Terminator
    s[i] = 0;
    // Reverse Array
    reverse_str(s,i);

}

struct div_result_struct divide_with_mod(uint32_t a, uint32_t b) {
    uint32_t x = a,y = b;
    struct div_result_struct ans;
    ans.div = 0;
    // Avoid Division by Zero
    if (y != 0){
        while (x >= y) {
            x -= y;
            ans.div++;
        }
    }
    ans.mod = x;
    // Return Division Result
    return ans;
}

void reverse_arr(uint32_t * arr, uint32_t length){

    uint32_t i = 0, j = length - 1;
    uint32_t temp;
    for (;i < j; i++ , j--){
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}


void reverse_str(uint8_t * arr, uint32_t length){

    uint32_t i = 0, j = length - 1;
    uint8_t temp;
    for (;i < j; i++ , j--){
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
