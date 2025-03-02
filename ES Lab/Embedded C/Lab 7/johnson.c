#include <lpc17xx.h>

unsigned int curr, msb, i;

int main (){
	
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= ~(0xFFFF<<8);
	LPC_GPIO0->FIODIR |= (0xFF<<4);
	
	LPC_GPIO0->FIOCLR |= (0xFF<<4);
	LPC_GPIO0->FIOSET |= (0x01<<4);

	while(1) {
		curr = (LPC_GPIO0->FIOPIN & (0xFF<<4)) >> 4;
		msb = curr >> 7;
		curr = ((curr << 1) & 0xFF) | !msb;
		
		LPC_GPIO0->FIOCLR |= (0xFF<<4);
		LPC_GPIO0->FIOSET |= (curr<<4);
		
		for(i = 0; i < 30000; i++);
	}
}
