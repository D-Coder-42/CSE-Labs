	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD Reset_Handler
	
	AREA	gcd_lcm, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR		R0, =NUM1
	LDR		R1, =NUM2
	LDR		R2, =GCD
	LDR		R3, =LCM
	LDR		R4, [R0]
	LDR		R5, [R1]
	
LOOP
	CMP		R4, R5
	BEQ		DONE
	SUBHI	R4, R4, R5
	SUBLO	R5, R5, R4
	B		LOOP
DONE
	STR		R4, [R2]
	
	LDR		R4, [R0]
	LDR		R5, [R1]
	LDR		R6, [R2]
	MUL		R5, R5, R4
	MOV		R7, #0x00
UP	
	CMP		R5, R6
	BLO		DOWN
	SUB		R5, R6
	ADD		R7, #1
	B		UP
DOWN
	STR		R7, [R3]
STOP
	B		STOP

NUM1
	DCD		0x10
NUM2
	DCD		0x0C

	AREA	store, DATA, READWRITE
GCD
	DCD		0x00
LCM
	DCD		0x00
	END