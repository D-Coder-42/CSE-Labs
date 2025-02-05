	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD Reset_Handler

	AREA	fibonacci, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR		R0, =N
	LDR		R5, =DST
	LDR		R1, [R0]
	MOV		R2, #0x00
	MOV		R3, #0x01
	CMP		R1, #0x00
	BEQ		STOP
	STR		R2, [R5], #4
	SUBS	R1, #0x01
	BEQ		STOP
	STR		R3, [R5], #4
	SUBS	R1, #0x01
	BEQ		STOP
LOOP
	ADD		R2, R3
	MOV		R4, R2
	MOV		R2, R3
	MOV		R3, R4
	STR		R3, [R5], #4
	SUBS	R1, #1
	BNE		LOOP
STOP
	B	STOP
	ALIGN
N
	DCD	0x00
	
	AREA	data, DATA, READWRITE
DST
	DCD	0x00
	END