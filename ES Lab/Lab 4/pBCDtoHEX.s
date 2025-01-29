	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD	Reset_Handler
	
	AREA	BCDconv, CODE, READONLY
	ENTRY
	EXPORT	Reset_Handler
Reset_Handler
	LDR		R0, =BCD
	LDR		R1, [R0]
	MOV		R2, #10
	MOV		R3, #1 
LOOP
	MOV		R4, R1
	AND		R4, #0xF
	MLA		R5, R4, R3, R5
	MUL		R3, R2
	LSRS	R1, #4
	BNE		LOOP
	
	LDR		R0, =HEX
	STR		R5, [R0]
STOP
	B		STOP
	ALIGN
BCD
	DCD	0x12345678
	
	AREA	data, DATA, READWRITE
HEX
	DCD	0x00
	END