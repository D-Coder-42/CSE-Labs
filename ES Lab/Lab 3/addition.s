	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD Reset_Handler

	AREA	addition, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR		R0, =NUM1
	LDR		R1, [R0]
	LDR		R0, =NUM2
	LDR 	R2, [R0]
	ADDS	R3, R1, R2	;R3 = NUM1 + NUM2
	LDR		R0, =RES
	STR		R3, [R0]
STOP
	B	STOP
	ALIGN
NUM1
	DCD	0x89ABCDEF
NUM2
	DCD	0x01234567
	
	AREA	data, DATA, READWRITE
RES
	DCD 0x00
	END