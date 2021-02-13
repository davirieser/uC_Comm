
#pragma once

// See Explanation on  https://en.wikipedia.org/wiki/ANSI_escape_code
// See Colors on https://github.com/linrock/256-colors

#define RESET_TERMINAL "\033[0m"
#define BOLD "\033[1m"
#define FAINT "\033[2m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define SLOW_BLINK "\033[5m"
#define FAST_BLINK "\033[6m"
#define CROSS_OUT "\033[9m"
#define PROP_SPACING "\033[26m"  // Proportional Spacing
// #define FORE_COLOR "\033[38m" // Foreground Color (Next Arg is Color in RGB)
// #define BACK_COLOR "\033[48m" // Background Color (Next Arg is Color in RGB)

#define RESET_FORE_COLOR "\033[39m"
#define RESET_BACK_COLOR "\033[49m"

// Use Colors after Index 16 because the first 16 colors can
// apparently be modified in the Terminal Color Scheme
#define BLACK "\033[38;5;232m"
#define WHITE "\033[38;5;231m"
#define RED "\033[38;5;196m"
#define GREEN "\033[38;5;113m"
#define YELLOW "\033[38;5;227m"
#define BLUE "\033[38;5;69m"
#define LIGHT_GREY "\033[38;5;240m"
#define MEDIUM_GREY "\033[38;5;238m"
#define DARK_GREY "\033[38;5;236m"
