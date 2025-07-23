#include <lpc17xx.h>

unsigned int digits[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
unsigned int i,j,count,sw_pin;

void displaySSD (int);
void initTimer (void);
void delayMilliS (int);

int main () {
	SystemInit();
	SystemCoreClockUpdate();
	initTimer();
	
	LPC_PINCON->PINSEL0 &= ~(0xFFFF << 8);
	LPC_PINCON->PINSEL3 &= ~(0xFF << 14);
	LPC_PINCON->PINSEL1 &= ~(0x03<<10);
	
	LPC_GPIO0->FIODIR |= (0xFF << 4);
	LPC_GPIO1->FIODIR |= (0xF << 23);
	LPC_GPIO0->FIODIR &= ~(0x1<<21);
	
	count = 0;
	
	while (1) {
		sw_pin = (LPC_GPIO0->FIOPIN & (1 << 21)) >> 21;
		count += sw_pin == 1? 1 : -1;
		
		displaySSD(count);
		
		if (count == 9999 && sw_pin == 1) count = 0;
		if (count == 0 && sw_pin == 0) count = 9999;
		
		// delayMilliS(1000);
	}
}

void displaySSD (int num) {
	int d[4] = {0};
	d[3] = num % 10;
	d[2] = num/10 % 10;
	d[1] = num/100 % 10;
	d[0] = num/1000 % 10;
	
	for (i = 0; i < 4; i++) {
		LPC_GPIO1->FIOCLR = (0xF << 23);
		LPC_GPIO1->FIOSET = (i << 23);
		LPC_GPIO0->FIOCLR = (0xFF << 4);
		LPC_GPIO0->FIOSET = digits[d[3-i]] << 4;
		
		delayMilliS(5);
	}
}

void initTimer() {
    LPC_TIM0->CTCR = 0x00;
    LPC_TIM0->TCR = 0x02;
    LPC_TIM0->PR = 2999;
    LPC_TIM0->TCR = 0x01;
}

void delayMilliS(int delay) {
    LPC_TIM0->TC = 0;
    while (LPC_TIM0->TC < delay);
}
