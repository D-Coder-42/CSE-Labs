	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000  ;SP
	DCD	Reset_Handler

	AREA	moveblock, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	MOV		R3, #10
	LDR		R0, =SRC
	LDR		R1, =DST
LOOP
	LDR		R2, [R0], #4
	STR		R2, [R1], #4
	SUBS	R3, #1
	BNE		LOOP
STOP
	B	STOP
	ALIGN	;aligns data such that there is no error 
SRC
	DCD		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A
	
	AREA	dstdata, DATA, READWRITE
DST
	SPACE	40
	END