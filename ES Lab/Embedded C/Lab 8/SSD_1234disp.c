#include <lpc17xx.h>

unsigned int digits[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
unsigned int i;

int main () {
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= ~(0xFFFF << 8);
	LPC_PINCON->PINSEL3 &= ~(0xFF << 14);
	
	LPC_GPIO0->FIODIR |= (0xFF << 4);
	LPC_GPIO1->FIODIR |= (0xF << 23);
	
	while (1) {
		for (i = 0; i < 4; i++) {
			LPC_GPIO1->FIOCLR = (0xF << 23);
			LPC_GPIO1->FIOSET = (i << 23);
			LPC_GPIO0->FIOCLR = (0xFF << 4);
			LPC_GPIO0->FIOSET = digits[3-i] << 4;
		}
	}
}
