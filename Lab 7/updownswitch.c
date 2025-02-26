#include <lpc17xx.h>

int i = 0, j, sw_pin;

int main (){
	
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= ~(0xFFFF<<8);
	LPC_PINCON->PINSEL1 &= ~(0x03<<10); 
	
	LPC_GPIO0->FIODIR |= (0xFF<<4);
	LPC_GPIO0->FIODIR &= ~(0x1<<21);

	while(1) {
		sw_pin = (LPC_GPIO0->FIOPIN & (1 << 21)) >> 21;
		if (sw_pin == 1) i = i+1;
		else i = i-1;
		
		LPC_GPIO0->FIOPIN = i << 4;
		for(j = 0; j < 30000; j++);
		
		if (i == 255 && sw_pin==1) i = 0;
		if (i == 0 && sw_pin==0) i = 255;
	}
}
