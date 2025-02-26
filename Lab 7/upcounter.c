#include <lpc17XX.h>

unsigned int i,j;

int main (){
	
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON->PINSEL0 &= ~(0x3<<29);
	LPC_PINCON->PINSEL1 &= ~(0x3FFF);
    
	LPC_GPIO0->FIODIR |= (0xFF<<15);
	LPC_GPIO0->FIOCLR |= (0xFF<<15);

	while(1) {
		for(i = 0; i < 1 << 8; i++) {
			LPC_GPIO0->FIOPIN = (i << 15);
			for(j = 0; j < 30000; j++);
		}
	}
}
