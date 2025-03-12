#include <lpc17xx.h>

unsigned int curr, msb, i, sw_pin;
unsigned int LED = 1;

int main (){
	
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= ~(0xFFFF<<8);
	LPC_PINCON->PINSEL1 &= ~(0x03<<10);

	LPC_GPIO0->FIODIR |= (0xFF<<4);
	LPC_GPIO0->FIODIR &= ~(0x1<<21);
	
	LPC_GPIO0->FIOCLR |= (0xFF<<4);
	LPC_GPIO0->FIOSET |= (0x01<<4);

	while(1) {
		sw_pin = (LPC_GPIO0->FIOPIN & (1 << 21)) >> 21;
		if (sw_pin == 0) {
			curr = (LPC_GPIO0->FIOPIN & (0xFF<<4)) >> 4;
			msb = curr >> 7;
			curr = ((curr << 1) & 0xFF) | msb;
		
			LPC_GPIO0->FIOCLR |= (0xFF<<4);
			LPC_GPIO0->FIOSET |= (curr<<4);

			// Another possible logic
			// LPC_GPIO0->FIOCLR |= (0xFF<<4);
			// LPC_GPIO0->FIOSET |= (LED<<4);

			// LED <<= 1;
			// if (LED >= 1 << 9)	LED = 1;

			for(i = 0; i < 30000; i++);
		}
	}
}
