#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool parse_argv(int, char **, char *, int *, int *, int *);
void set_start_end(char *, int *, int *);
void convert_to_binary(char *, char *);
void binary_to_binary(char *, int, char *);
void hexadecimal_to_binary(char *, int, char *);
void string_to_binary(char *, int, char *);
void unsigned_decimal_to_binary(long, char *);
void to_negative(char *);
void print_binary_4_chunk(char *);
void print_binary(char *);

int main(int argc, char **argv) {
    
    int bit_count, range_start, range_end;
    char bits[33];
    bool check;
    
    check = parse_argv(argc, argv, bits, &bit_count, &range_start, &range_end);
    printf("count = %d, start = %d, end = %d\n", bit_count, range_start, range_end);
    printf("bits = %s\n", bits);
    /*decimal_to_binary(bits, atoi(argv[1]));

    print_binary_4_chunk(bits);
    print_binary(bits);*/

    return 0;
}

bool parse_argv(int argc, char **argv, char *bits, int *bit_count, int *range_start, int *range_end) {
    int i;
    *bit_count = 32;
    *range_start = 0;
    *range_end = 31;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            *bit_count = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-r") == 0) {
            set_start_end(argv[i + 1], range_start, range_end);
            i++;
        }
        else {
            convert_to_binary(argv[i], bits);
        }
    }
    
    return true;
}

void set_start_end(char *arg, int *range_start, int *range_end) {
    int i, j, len;
    char temp[3];
    len = strlen(arg);

    for (i = 0, j = 0; i < len; i++) {
        if (arg[i] == ',') {
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

void convert_to_binary(char *arg, char *bits) {
    int len = strlen(arg);
    if (len > 2 && arg[1] == 'b') {
        binary_to_binary(arg, len, bits);
    }
    else if (len > 2 && arg[1] == 'x') {
        hexadecimal_to_binary(arg, len, bits);
    }
    else {
        string_to_binary(arg, len, bits);
    }

    bits[32] = '\0';
}

void binary_to_binary(char *arg, int len, char *bits) {
    int i, j;

    for (i = len - 1, j = 31; j >= 0; i--, j--) {
        bits[j] = i >= 2 ? arg[i] : '0';
    }
}

void hexadecimal_to_binary(char *arg, int len, char *bits) {
    int i;
    char c;
    long unsigned_decimal;

    for (i = 2, unsigned_decimal = 0; i < len; i++) {
        c = arg[i];
        unsigned_decimal *= 16;
        unsigned_decimal += (c >= 'A' && c <= 'F') ? (10 + c - 'A') : (0 + c - '0');
    }
    
    unsigned_decimal_to_binary(unsigned_decimal, bits);
}

void string_to_binary(char *arg, int len, char *bits) {
    int i;
    unsigned int temp;
    char c;
    long unsigned_decimal;
    bool negative = false;

    if (arg[0] == '-') {
        negative = true;
        i = 1;
    }
    else {
        i = 0;
    }
    
    for (unsigned_decimal = 0; i < len; i++) {
        c = arg[i];
        unsigned_decimal = unsigned_decimal * 10 + (0 + c - '0');
    }

    unsigned_decimal_to_binary(unsigned_decimal, bits);  

    if (negative) {
        to_negative(bits);
    }
}

void unsigned_decimal_to_binary(long unsigned_decimal, char *bits) {
    int i;
    
    for (i = 31; i >= 0; i--) {
        bits[i] = (unsigned_decimal & 0b1) == 0 ? '0' : '1';
        unsigned_decimal >>= 1;
    }
}

void to_negative(char *bits) {
    int i;
    char carry = '1';

    for (i = 31; i >= 0; i--) {
        bits[i] = bits[i] == '0' ? '1' : '0';
    }

    for (i = 31; i >= 0; i--) {
        if (carry == '1' && bits[i] == '1') {
            bits[i] = '0';
        }
        else if (carry == '1' && bits[i] == '0') {
            bits[i] = '1';
            carry = '0';
        }
        else {
            break;
        }
    }
}

void print_binary_4_chunk(char *bits) {
    int i, counter;

    for (i = 0, counter = 1; i < 32; i++, counter++) {
        printf("%c", bits[i]);
        if (counter % 4 == 0) {
            printf(" ");
            counter = 0;
        }
    }

    printf("(base 2)\n");
}

void print_binary(char *bits) {
    int i;

    printf("0b");
    for (i = 0; i < 32; i++) {
        printf("%c", bits[i]);
    }
    printf(" (base 2)\n");
}
