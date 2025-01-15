	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000  ;SP
	DCD	Reset_Handler

	AREA	movedata, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR	R0, =SRC
	LDR	R1, =DST
	LDR	R2, [R0]
	STR	R2, [R1]
STOP
	B	STOP
	
	AREA	srcdata, DATA, READONLY
SRC
	DCD	0x12345678
	
	AREA	dstdata, DATA, READWRITE
DST
	DCD	0x00
	END