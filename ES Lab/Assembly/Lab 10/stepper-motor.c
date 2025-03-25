/////////////////////////////////////////
///// STEPPER MOTOR ROTATE FUNCTION /////
/////////////////////////////////////////

void rotate (DIRECTION dir, int degree) {
	int steps = degree/1.8;
	int motor, i;
	if (dir == CLOCKWISE) {
		motor = 1;
		for (i = 0; i < steps; i++) {
			motor = (motor << 1) & 0xF;
			LPC_GPIO0->FIOCLR = 0xF << 4;
			LPC_GPIO0->FIOSET = motor << 4;
			if (motor == 0) motor = 1;
		}
	} else {
		motor = 1 << 3;
		for (i = 0; i < steps; i++) {
			motor = (motor >> 1);
			LPC_GPIO0->FIOCLR = 0xF << 4;
			LPC_GPIO0->FIOSET = motor << 4;
			if (motor == 0) motor = 1 << 3;
		}
	}
}
