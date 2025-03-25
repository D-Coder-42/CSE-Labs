#include <lpc17xx.h>
#include <stdbool.h>

int sw_pin, led, i;
bool asc;

void delay (int);

int main () {
	LPC_PINCON->PINSEL0 &= ~(0xFFFF << 8);
	LPC_PINCON->PINSEL3 &= ~(0x3 << 20);
	
	LPC_GPIO0->FIODIR |= (0xFF << 4);
	LPC_GPIO1->FIODIR &= ~(0x1 << 26);
	
	led = 0xFF;
	i = 0;
	
	while (1) {
		sw_pin = (LPC_GPIO1->FIOPIN >> 26) & 0x1;
		if (sw_pin == 0) {
			LPC_GPIO0->FIOCLR = 0xFF << 4;
			LPC_GPIO0->FIOSET = led << 4;
			
			if (!asc) led &= (led << 1) & (led >> 1);
			if (asc) led |= (led << 1) | (led >> 1);
			
			if (led == 0x00) {
				led = 0x18;
				asc = true;
			}
			if (led == 0x1FF) {
				led = 0xFF;
				asc = false;
			}
			delay(30000);
		}
	}
}

void delay (int del) {
	int i;
	for (i = 0; i < del; i++);
}
