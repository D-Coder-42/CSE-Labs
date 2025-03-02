	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD Reset_Handler
	
	AREA	factorial, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR		R0, =NUM
	LDR		R1, [R0]
	MOV		R2, #0x01
	
LOOP
	MUL		R2, R2, R1
	SUBS	R1, #0x01
	BNE		LOOP
	
	LDR		R0, =FACT
	STR		R2, [R0]

STOP
	B		STOP
	ALIGN
NUM
	DCD		0x0C
	
	AREA	store, DATA, READWRITE
FACT
	DCD		0x00
	END