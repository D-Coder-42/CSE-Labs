	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD	Reset_Handler
	
	AREA	BCDconv, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	MOV		R5, #4
	LDR		R0, =NUM
	LDR		R3, =RES
UP
	LDRB	R1, [R0], #1
	AND		R2, R1, #0xF
	ADD		R2, #0x30
	STR		R2, [R3], #4
	AND		R4, R1, #0xF0
	MOV		R4, R4, LSR #4
	ADD		R4, #0x30
	STR		R4, [R3], #4
	SUBS	R5, #1
	BNE		UP
STOP
	B		STOP
	ALIGN
NUM
	DCD		0x12345678
	
	AREA	data, DATA, READWRITE
RES
	DCD 0x00
	END