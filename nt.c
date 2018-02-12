#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BIT 32

bool parse_argv(int, char **, char *, int *, int *, int *, int*);
void set_start_end(char *, int *, int *);
bool convert_to_binary(char *, char *, int);
bool binary_to_binary(char *, int, char *);
bool hexadecimal_to_binary(char *, int, char *);
bool string_to_binary(char *, int, char *, int);
bool unsigned_decimal_to_binary(unsigned long long, char *);
bool to_negative(char *, int);
void set_output_bits(char *, char *, int, int, int);
void print_all(char *, int);
void print_binary(char *, int);
void print_hexadecimal_and_unsigned(char *, int);
void print_hexadecimal(unsigned int, int);
void print_signed(char *, int);
unsigned int calculate_decimal(char *, int);

int main(int argc, char **argv) {
    
    int bit_width, range_start, range_end, index;
    char bits[BIT], output_bits[BIT + 1];
    bool check;
    
    check = parse_argv(argc, argv, bits
                       , &bit_width, &range_start, &range_end, &index);

    if (!check) {
        if (index == -1) {
            printf("Error: please input a valid argument.\n");
        }
        else if (index == -2) {
            printf("Error: please input a valid bit width in 0-32.\n");
        }
        else if (index == -3) {
            printf("Error: please intpur a valid range in 0-32 and start should not go over end.\n");
        }
        else {
            printf("Error: %s cannot fit into %d bits.\n", argv[index], BIT);
        }
    }
    else {
        set_output_bits(output_bits, bits, bit_width, range_start, range_end);
        print_all(output_bits, bit_width);
    }

    return 0;
}

bool parse_argv(int argc, char **argv, char *bits
                , int *bit_width, int *range_start, int *range_end, int *index) {
    int i;
    bool check = false;

    *bit_width = BIT; // default settings
    *range_start = 0;
    *range_end = 31;
    *index = -1; // if no input argument

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            *bit_width = atoi(argv[i + 1]);
            if (*bit_width <= 0 || *bit_width > 32) {
                *index = -2; // invalid bit width
                return false;
            }
            i++;
        }
        else if (strcmp(argv[i], "-r") == 0) {
            set_start_end(argv[i + 1], range_start, range_end);
            if (*range_start < 0 || *range_start > 32 || *range_end < 0 || *range_end > 32 || *range_start > *range_end) {
                *index = -3; // invalid range
                return false;
            }
            i++;
        }
        else {
            check = convert_to_binary(argv[i], bits, BIT);
            *index = i; // pass the index of the number to the main method
        }
    }
    
    return check;
}

void set_start_end(char *arg, int *range_start, int *range_end) {
    int i, j, len;
    char temp[3]; // the range should be no more than 2 deciamls
    len = strlen(arg);

    for (i = 0, j = 0; i < len; i++) {
        if (arg[i] == ',') { // before the ',' is start
            temp[j] = '\0';
            *range_start = atoi(temp);
            j = 0;
        }
        else {
            temp[j] = arg[i];
            j++;
        }
    }

    temp[j] = '\0';
    *range_end = atoi(temp);
}

bool convert_to_binary(char *arg, char *bits, int bit_width) {
    int len = strlen(arg);
    bool check;
    
    if (len > 2 && arg[1] == 'b') {
        check = binary_to_binary(arg, len, bits);
    }
    else if (len > 2 && arg[1] == 'x') {
        check = hexadecimal_to_binary(arg, len, bits);
    }
    else {
        check = string_to_binary(arg, len, bits, bit_width);
    }

    return check;
}

bool binary_to_binary(char *arg, int len, char *bits) {
    int i, j;
    bool check = true;

    for (i = len - 1, j = 31; j >= 0 || i >= 2; i--, j--) {
        if (i < 2) {
            bits[j] = '0';
        }
        else if (j < 0 && arg[i] == '1') {
            check = false; // if there is an '1' after 32nd bit, overflow
        }
        else if (j >= 0) {
            if (arg[i] != '0' && arg[i] != '1') {
                return false;
            }
            bits[j] = arg[i];
        }
    }

    return check;
}

bool hexadecimal_to_binary(char *arg, int len, char *bits) {
    int i;
    char c;
    unsigned long long unsigned_decimal;

    for (i = 2, unsigned_decimal = 0; i < len; i++) {
        c = arg[i];
        if (!((c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'))) {
            return false; // invalid character
        }
        unsigned_decimal *= 16;
        unsigned_decimal += (c >= 'A' && c <= 'F') ? (10 + c - 'A') : (0 + c - '0');
    }
    
    return unsigned_decimal_to_binary(unsigned_decimal, bits);
}

bool string_to_binary(char *arg, int len, char *bits, int bit_width) {
    int i;
    unsigned int temp;
    char c;
    unsigned long long unsigned_decimal;
    bool check, negative = false;

    if (arg[0] == '-') {
        negative = true;
        i = 1;
    }
    else {
        i = 0;
    }
    
    for (unsigned_decimal = 0; i < len; i++) {
        c = arg[i];
        if (!(c >= '0' && c <= '9')) {
            return false; // invalid character
        }
        unsigned_decimal = unsigned_decimal * 10 + (0 + c - '0');
    }

    if (negative && unsigned_decimal > (unsigned) 2147483648) {
        return false; // signed overflow
    }
    
    check = unsigned_decimal_to_binary(unsigned_decimal, bits);

    if (negative && unsigned_decimal > 0) {
        check = check && to_negative(bits, bit_width);
    }

    return check;
}

bool unsigned_decimal_to_binary(unsigned long long unsigned_decimal, char *bits) {
    int i;

    for (i = BIT - 1; i >= 0; i--) {
        bits[i] = (unsigned_decimal & 0b1) == 0 ? '0' : '1';
        unsigned_decimal >>= 1;
    }

    if (unsigned_decimal > 0) {
        return false; // reminder after 32nd bit consider overflow
    }

    return true;
}

bool to_negative(char *bits, int bit_width) {
    int i;
    char carry = '1';

    for (i = bit_width - 1; i >= 0; i--) {
        bits[i] = bits[i] == '0' ? '1' : '0';
    }

    for (i = bit_width - 1; i >= 0; i--) {
        if (carry == '1' && bits[i] == '1') {
            bits[i] = '0';
        }
        else if (carry == '1' && bits[i] == '0') {
            bits[i] = '1';
            carry = '0';
        }
        else {
            break; // carry == '0'
        }
    }

    if (carry == '1') {
        return false; // the 33rd bit should be '0', otherwise overflow
    }

    return true;
}

void set_output_bits(char *output_bits, char *bits, int bit_width, int range_start, int range_end) {
    int i, j, stop;
    stop = BIT - 1 - range_end;
    
    for (i = bit_width - 1, j = BIT - 1 - range_start; i >= 0; i--, j--) {
        if (j < stop) {
            output_bits[i] = '0'; // bits after the range_end consider '0'
        }
        else {
            output_bits[i] = bits[j];
        }
    }
    output_bits[bit_width] = '\0';
}

void print_all(char *output_bits, int bit_width) {
    print_binary(output_bits, bit_width);
    print_hexadecimal_and_unsigned(output_bits, bit_width);
    print_signed(output_bits, bit_width);
}

void print_binary(char *output_bits, int bit_width) {
    int i, counter = (4 - bit_width % 4 + 1) % 4;

    for (i = 0; i < bit_width; i++, counter++) { // four chunks binary
        printf("%c", output_bits[i]);
        if (counter % 4 == 0) {
            printf(" ");
            counter = 0;
        }
    }
    printf("(base 2)\n");

    printf("0b%s (base 2)\n", output_bits); // normal binary
}

void print_hexadecimal_and_unsigned(char *bits, int bit_width) {
    unsigned int unsigned_decimal;
    unsigned_decimal = calculate_decimal(bits, bit_width);

    print_hexadecimal(unsigned_decimal, bit_width);
    printf("%u (base 10 unsigned)\n", unsigned_decimal);
}

void print_hexadecimal(unsigned int unsigned_decimal, int bit_width) {
    int i, temp, len = (bit_width % 4 == 0) ? (bit_width / 4) : (bit_width / 4 + 1);
    char hex[len + 1];

    for (i = len - 1; i >= 0; i--) {
        temp = unsigned_decimal % 16;
        hex[i] = (temp >= 10) ? ('A' + temp - 10) : ('0' + temp);
        unsigned_decimal /= 16;
    }
    hex[len] = '\0';
    
    printf("0x");
    printf("%s (base 16)\n", hex);
}

void print_signed(char *output_bits, int bit_width) {
    int decimal;
    bool negative = false;

    if (output_bits[0] == '1') {
        to_negative(output_bits, bit_width); // flip back to get the decimal part
        negative = true;
    }

    decimal = (int) calculate_decimal(output_bits, bit_width) * (negative ? -1 : 1);

    printf("%d (base 10 signed)\n", decimal);
}

unsigned int calculate_decimal(char *bits, int bit_width) {
    unsigned int temp = 0;
    int i;
    
    for (i = 0; i < bit_width; i++) {
        temp = temp * 2 + (bits[i] == '1' ? 1 : 0);
    }

    return temp;
}
