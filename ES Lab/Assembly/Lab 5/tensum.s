	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD Reset_Handler

	AREA	tenadd, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR		R0, =ARR
	MOV		R2, #0x0A
	MOV		R4, #0x00
LOOP
	LDR		R1, [R0], #4
	ADDS	R3, R1
	ADC		R4, #0
	SUBS	R2, #1
	BNE		LOOP
	LDR		R0, =DST
	STR		R3, [R0], #4
	STR		R4, [R0]
STOP
	B	STOP
	ALIGN
ARR
	DCD	0x90000000,0x90000000,0x90000000,0x90000000,0x90000000,0x90000000,0x90000000,0x90000000,0x90000000,0x90000000
	
	AREA	data, DATA, READWRITE
DST
	DCD	0x00, 0x00
	END