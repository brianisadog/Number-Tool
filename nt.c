#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void to_binary(char *, int);
void print_binary_4_chunk(char *);
void print_binary(char *);

int main(int argc, char **argv) {
    
    int bit_count = 32;
    int range_start = 0;
    int range_end = 31;
    char bits[32];

    to_binary(bits, atoi(argv[1]));

    print_binary_4_chunk(bits);
    print_binary(bits);

    return 0;
}

void to_binary(char *bits, int input) {
    int i = 31;
    while (i >= 0) {
        bits[i] = '0' + input % 2;
        input /= 2;
        i--;
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
