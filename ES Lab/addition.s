	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0x10001000	;SP
	DCD Reset_Handler
	ALIGN
	
	AREA	addition, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	MOV	R0, #0x03
	MOV	R1, #0x0A
	ADD	R2, R0, R1
STOP
	B	STOP
	END