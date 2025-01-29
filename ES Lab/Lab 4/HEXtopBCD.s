	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD	Reset_Handler
	
	AREA	BCDconv, CODE, READONLY
	ENTRY
	EXPORT	Reset_Handler
Reset_Handler
	LDR		R0, =HEX
	LDR		R1, =BCD
	LDR		R2, [R0]
	
LOOP
	CMP		R2, #10
	BLO		STORE
	BL		DIVISION
	
	MOV		R2, R4
	MOV		R4, #0
	B		LOOP
STORE
	STRB	R2, [R1]
	
	LDR		R0, =BCD
	LDR		R1, =BCD
	MOV		R2, #2
CONV
	LDRB	R3, [R0], #1
	LDRB	R4, [R0], #1
	LSL		R4, #4
	ORR		R3, R4
	STRB	R3, [R1], #1
	SUBS	R2, #1
	BNE		CONV
	
	MOV		R5, #0
	STRH	R5, [R1]
STOP
	B		STOP
		
DIVISION
	CMP		R2, #10
	BCC		DIVSTORE
	SUB		R2, #10
	ADD		R4, #1
	B		DIVISION
DIVSTORE
	STRB	R2, [R1], #1
	BX		LR
	
	ALIGN
HEX
	DCD		0x1111
		
	AREA	data, DATA, READWRITE
BCD
	DCD	0x00
	END