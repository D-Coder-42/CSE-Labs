#include <stdio.h>
#define MAX 100
#define MIN 10

/* This is a multi-line comment.
   It should be ignored.
   Keywords inside comments like int, main, if should be ignored.
*/

// Single line comment: ignoring char, else

int main() {
    char _var1 = 'c'; 
    int integer = 20;
    int arr[10];
    
    int x = 12345, a;
    int sci1 = 1E10;
    int sci2 = 2e+5;

    printf("Hello World");
    char* str2 = "String with \"escaped quotes\" inside";

    if (a >= 10 && a <= 20) {
        a = a + 1;
        a = a - 1;
    } else {
        a = a * 2;
        a = a / 2;
    }

    int check = (a == b) || (a != c);
    int arr[5];
    arr[0] = { 1 };
    int bitwise = a & b; 
    int bitwise_or = a | b;

    return 0;
}