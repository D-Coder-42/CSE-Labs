	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000  ;SP
	DCD	Reset_Handler

	AREA	reverse, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	MOV		R4, #5
	LDR		R0, =SRC
	ADD		R1, R0, #36
LOOP
	LDR		R2, [R0]
	LDR		R3, [R1]
	STR		R3, [R0], #4
	STR		R2, [R1], #-4
	SUBS	R4, #1
	BNE		LOOP
STOP
	B	STOP
	
	AREA	srcdata, DATA, READWRITE
SRC
	DCD		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A
    ; values get overriden at start of execution (manually update in memory)
	END