#include <stdio.h>
// Example C code to test the lexer and parser
main() {
    int a, b, arr[50], result;
    char flag, symbol;

    a = 10;
    b = 20 + 5 * 2;
    
    if (a <= b) {
        result = a * b / 2;
        arr[1] = result % 3;
    } else {
        result = 0;
    }
}