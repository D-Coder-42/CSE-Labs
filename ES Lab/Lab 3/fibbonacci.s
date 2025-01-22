	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD Reset_Handler

	AREA	fibbonacci, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR	R0, =N
	LDR	R1, [R0]
	SUB	R1, #0x01
	MOV	R2, #0x00
	MOV	R3, #0x01
LOOP
	ADD		R2, R3
	MOV		R4, R2
	MOV		R2, R3
	MOV		R3, R4
	SUBS	R1, #1
	BNE		LOOP
	LDR		R0, =DST
	STR		R3, [R0]
STOP
	B	STOP
	ALIGN
N
	DCD	0x10
	
	AREA	data, DATA, READWRITE
DST
	DCD	0x00
	END
