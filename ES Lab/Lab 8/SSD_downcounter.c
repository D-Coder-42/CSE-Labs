#include <lpc17xx.h>

unsigned int digits[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
unsigned int i,j,count;

void displaySSD(int);

int main () {
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= ~(0xFFFF << 8);
	LPC_PINCON->PINSEL3 &= ~(0xFF << 14);
	
	LPC_GPIO0->FIODIR |= (0xFF << 4);
	LPC_GPIO1->FIODIR |= (0xF << 23);
	
	count = 9999;
	
	while (1) {
		displaySSD(count--);
		if (count == 0) count = 9999;
		
		for (j = 0; j<40000; j++);
	}
}

void displaySSD (int num) {
	int d[4] = {0};
	d[3] = num % 10;
	d[2] = num/10 % 10;
	d[1] = num/100 % 10;
	d[0] = num/1000 % 10;
	
	for (i = 0; i < 4; i++) {
		LPC_GPIO1->FIOPIN &= ~(0xF << 23);
		LPC_GPIO1->FIOPIN |= (i << 23);
		LPC_GPIO0->FIOPIN &= ~(0xFF << 4);
		LPC_GPIO0->FIOPIN |= digits[3-i] << 4;
			
		for (j = 0; j<10000; j++);
	}
}