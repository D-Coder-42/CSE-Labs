#include <stdio.h>

int main() {
    int x = 42;
    double f = 314.15296;
    char ch = 'A';
    char str[] = "Hello";

    printf("Decimal: %d\n", x);
    printf("Hexadecimal: %x\n", x);
    printf("Octal: %o\n", x);
    printf("Character: %c\n", ch);
    printf("String: %s\n", str);
    printf("Floating point: %f\n", f);
    printf("Exponential: %e\n", f);
    printf("Pointer: %p\n", &x);

    return 0;
}

