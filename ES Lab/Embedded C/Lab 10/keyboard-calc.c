#include <lpc17xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "LCDfuncs.c"

char* evaluate(char*, int);
void scanKeyboard();

unsigned int SCAN_CODE[16] = {0x11, 0x21, 0x41, 0x81,
                              0x12, 0x22, 0x42, 0x82,
                              0x14, 0x24, 0x44, 0x84,
                              0x18, 0x28, 0x48, 0x88};

unsigned char ASCII_CODE[16] =  {'1', '2', '3', '+',
                                 '4', '5', '6', '-',
                                 '7', '8', '9', '*',
                                 '<', '0', '=', '/'};

unsigned int currentRow, rowSelect, rowNibble, keyDetected, keyCode;
unsigned char keyChar;
int i;
unsigned int pressedKeys, colNibble;
int exprIndex = 0;
int showingResult = 0;

int main() {
    char message[] = "Basic Calculator";
    char expression[17] = {0};

    SystemInit();
    SystemCoreClockUpdate();

    LPC_PINCON->PINSEL4 &= ~(0xFF << 20);
    LPC_PINCON->PINSEL3 &= ~(0xFF << 14);
    LPC_PINCON->PINSEL1 &= ~(0xFFF << 14);

    LPC_GPIO2->FIODIR |= 0xF << 10;
    LPC_GPIO1->FIODIR &= ~(0xF << 23);
    LPC_GPIO0->FIODIR |= 0xF << 23 | 1 << 27 | 1 << 28;

    clearPorts();
    delayMicroS(1520);

    initLCD();

    comDataLCD(0x80, 0);
    delayMicroS(1520);
    displayLCD(message);

    while(1) {
        while(1) {
            for (currentRow = 0; currentRow < 4; currentRow++) {
                rowSelect = 1 << (10 + currentRow);
                rowNibble = rowSelect;

                LPC_GPIO2->FIOCLR = 0xF << 10;
                LPC_GPIO2->FIOSET = rowSelect;

                keyDetected = 0;
                scanKeyboard();
                if (keyDetected == 1) break;
            }
            if (keyDetected == 1) break;
        }

        for (i = 0; i < 16; i++) {
            if (keyCode == SCAN_CODE[i]) {
                keyChar = ASCII_CODE[i];
                if (keyChar == '<') {
                    if (showingResult) {
                        memset(expression, 0, sizeof(expression));
                        exprIndex = 0;
                        showingResult = 0;
                    } else if (exprIndex > 0) {
                        exprIndex--;
                        expression[exprIndex] = '\0';
                    }
                } else if (keyChar == '=') {
                    char* result = evaluate(expression, 2);
                    if (result) {
                        comDataLCD(0xC0, 0);
                        delayMicroS(1520);
                        displayLCD(result);
                        free(result);
                        showingResult = 1;
                    }
                } else {
                    if (showingResult) {
                        memset(expression, 0, sizeof(expression));
                        exprIndex = 0;
                        showingResult = 0;
                    }
                    if (exprIndex < sizeof(expression) - 1) {
                        expression[exprIndex++] = keyChar;
                        expression[exprIndex] = '\0';
                    }
                }
                break;
            }
        }

        comDataLCD(0xC0, 0);
        delayMicroS(1520);
        displayLCD(expression);
    }
}

char* evaluate(char* expr, int precision) {
    float total = 0.0f, current = 0.0f, number = 0.0f, decimal = 0.1f;
    char op = '+';  
    int is_fraction = 0, last_was_op = 1, first_number = 1;

    char* original_expr = expr;

    while (*expr) {
        if (*expr == '.') {
            if (is_fraction) return strdup("Invalid expression");  
            is_fraction = 1;
        }
        else if (isdigit(*expr)) {
            if (is_fraction) {
                number += (*expr - '0') * decimal;
                decimal *= 0.1f;
            } else {
                number = number * 10.0f + (*expr - '0');
            }
            last_was_op = 0;
        }
        else if (*expr == '+' || *expr == '-' || *expr == '*' || *expr == '/') {
            if (last_was_op) return strdup("Invalid expression");

            if (op == '/' && number == 0.0f) return strdup("Division by zero");

            if (first_number) {
                total = number;  
                first_number = 0;
            } else {
                switch (op) {
                    case '+': total += number; break;
                    case '-': total -= number; break;
                    case '*': total *= number; break;
                    case '/': total /= number; break;
                }
            }
            op = *expr;
            number = 0.0f;
            is_fraction = 0;
            decimal = 0.1f;
            last_was_op = 1;
        }
        else {
            return strdup("Invalid expression");
        }
        expr++;
    }

    if (last_was_op) return strdup("Invalid expression");

    if (op == '/' && number == 0.0f) return strdup("Division by zero");

    switch (op) {
        case '+': total += number; break;
        case '-': total -= number; break;
        case '*': total *= number; break;
        case '/': total /= number; break;
    }

    int bufferSize = strlen(original_expr) + 20;
    char* buffer = (char*)malloc(bufferSize);
    if (!buffer) return NULL;

    snprintf(buffer, bufferSize, "%s = %.*f", original_expr, precision, total);
    return buffer;
}

void scanKeyboard() {
    pressedKeys = (LPC_GPIO1->FIOPIN >> 23) & 0xF;
    if (pressedKeys) {
        keyDetected = 1;
        colNibble = pressedKeys << 4;
        rowNibble >>= 10;
        keyCode = colNibble | rowNibble;
    }
}

/////////////////////////////////////////////
///// EVALUATION WITHOUT ERROR CHECKING /////
/////////////////////////////////////////////

/*
char* evaluate(char* expr, int precision) {
    char* expression = expr;
    float total = 0.0f, current = 0.0f, number = 0.0f, decimal = 0.1f;
    char op = '+';
    int is_fraction = 0;

    while (*expr) {
        if (*expr == '.') is_fraction = 1;
        else if (isdigit(*expr)) {
            if (is_fraction) {
                number += (*expr - '0') * decimal;
                decimal *= 0.1f;
            } else {
                number = number * 10.0f + (*expr - '0');
            }
        }
        
        if ((!isdigit(*expr) && *expr != '.' && *expr != ' ') || !*(expr + 1)) {
            switch (op) {
                case '+': total += current; current = number; break;
                case '-': total += current; current = -number; break;
                case '*': current *= number; break;
                case '/': current /= number; break;
            }
            if (*expr != ' ') op = *expr;
            number = 0.0f;
            is_fraction = 0;
            decimal = 0.1f;
        }
        expr++;
    }
    
    float result = total + current;
    
    int bufferSize = strlen(expr) + 20; // 20 extra chars for " = " and the result
    char* buffer = malloc(bufferSize);
    if (buffer == NULL) return NULL;
    
    snprintf(buffer, bufferSize, "%s = %.*f", expression, precision, result);
    return buffer;
}