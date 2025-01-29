	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD	Reset_Handler
	
	AREA	BCDconv, CODE, READONLY
	ENTRY
	EXPORT	Reset_Handler
Reset_Handler
	LDR		R0, =SRC
	LDR		R1, =DST
	MOV		R2, #2
LOOP
	LDRB	R3, [R0], #1
	LDRB	R4, [R0], #1
	LSL		R4, #4
	ORR		R3, R4
	STRB	R3, [R1], #1
	SUBS	R2, #1
	BNE		LOOP
STOP
	B		STOP
	ALIGN
SRC
	DCD	0x01020304
	
	AREA	data, DATA, READWRITE
DST
	DCD	0x00
	END