	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000
	DCD Reset_Handler
	
	AREA	bcdadd, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	MOV		R4, #0x08
	LDR		R0, =NUM1
	LDR		R1, =NUM2
	LDR		R9, =DEST
	LDR		R2, [R0]
	LDR		R3, [R1]
	MOV		R5, #0x00
LOOP
	AND		R6, R2, #0x0F
	AND		R7, R3, #0x0F
	ADD		R8, R6, R7
	ADD		R8, R5
	CMP		R8, #0x09
	SUBHI	R8, #0x0A
	MOVHI	R5, #0x01
	MOVLS	R5, #0x00
	STRB	R8, [R9], #1
	LSR		R2, #0x04
	LSR		R3, #0x04
	SUBS	R4, #0x01	
	BNE		LOOP
	STRB	R5, [R9]
	
	MOV		R4, #5
	LDR		R0, =DEST
	LDR		R3, =PACK
UP
	LDRB	R1, [R0], #1
	LDRB	R2, [R0], #1
	LSL		R2, #4
	ORR		R1, R2
	STRB	R1, [R3], #1
	SUBS	R4, #1
	BNE		UP
	
STOP
	B		STOP
	ALIGN
		
NUM1
	DCD		0x87654321
NUM2
	DCD		0x98765432

	AREA	destdata, DATA, READWRITE
DEST
	SPACE	0x09
	ALIGN
PACK
	DCD		0x05
	END