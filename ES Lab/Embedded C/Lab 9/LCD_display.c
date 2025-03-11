#include <lpc17xx.h>

// Function Definitions
void initLCD (void);
void clearPorts (void);
void writeLCD (int, int);
void comDataLCD (int, int);
void displayLCD (char*);
void delayMicroS (int);

// Variable Declarations
int low, high;

int main () {
	char line1[] = "MIT - ";
	char line2[] = "Department of CSE";
	
	SystemInit();
	SystemCoreClockUpdate();
	
	initLCD();
	comDataLCD (0x80,0);
	delayMicroS(1520);
	
	displayLCD(line1);
	displayLCD(line2);
	
	return 0;
}

void initLCD () {
	LPC_PINCON->PINSEL1 &= ~(0xFFF << 14);
	LPC_GPIO0->FIODIR |= (0x3F << 23);
	
	clearPorts();
	delayMicroS(3200);
	
	comDataLCD(0x33,0);
	delayMicroS(1520);
	comDataLCD(0x32,0);
	delayMicroS(1520);
	
	comDataLCD(0x28,0);	// Function Set
	delayMicroS(37);
	comDataLCD(0x0C,0);	// Display ON, Cursor OFF
	delayMicroS(37);
	comDataLCD(0x06,0);	// Entry Mode SET, Increment Cursor Right
	delayMicroS(37);
	comDataLCD(0x01,0);	// Clear Display
	delayMicroS(1520);
	
	return;
}

void comDataLCD (int data, int type) {
	high = data & 0xF0;
	high = (high >> 4) << 23;
	writeLCD(high, type);
	
	low = data & 0x0F;
	low = low << 23;
	writeLCD(low, type);
	
	delayMicroS(37);
	return;
}

void writeLCD (int nibble, int type) {
	clearPorts();
	LPC_GPIO0->FIOPIN |= nibble;
	if (type)	LPC_GPIO0->FIOSET = 1 << 27;
	else			LPC_GPIO0->FIOCLR = 1 << 27;
	
	LPC_GPIO0->FIOSET = 1 << 28;
	delayMicroS(25);
	LPC_GPIO0->FIOCLR = 1 << 28;
	
	return;
}

void clearPorts () {
	LPC_GPIO0->FIOCLR = 0x0F << 23;	// Clearing Data Lines
	LPC_GPIO0->FIOCLR = 1 << 27;		// Clearing RS Line
	LPC_GPIO0->FIOCLR = 1 << 28;		// Clearing Enable Line
	
	return;
}

void delayMicroS (int delay) {
	// implement delay using TIMERS
	int j;
	for (j = 0; j < (100 * delay); j++);
	
	return;
}

void displayLCD (char* message) {
	int i = 0;
	int c;
	
	while (message[i] != '\0') {
		c = message[i];
		comDataLCD(c,1);
		i++;
		if (i==16)	comDataLCD(0xC0,0);
	}
	
	return;
}
