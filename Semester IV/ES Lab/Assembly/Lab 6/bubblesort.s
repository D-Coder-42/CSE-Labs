	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000
	DCD Reset_Handler
	
	AREA	bubblesort, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler

ARR	RN		R0
I	RN		R1
J	RN		R2

	LDR		ARR, =ARRAY
	MOV		I, #0x00
	
OUT
	CMP		I, #0x04
	BHS		STOP
	MOV		J, #0x00
	RSB		R3, I, #0x04
IN
	CMP		J, R3
	BHS 	EXIT_IN
	ADD		R4, ARR, J, LSL #2
	ADD		R5, R4, #4
	LDR		R6, [R4]
	LDR 	R7, [R5]
	CMP		R6, R7
	STRHI	R6, [R5]
	STRHI	R7, [R4]
	ADD		J, #0x01
	B		IN
	
EXIT_IN
	ADD		I, #0x01
	B		OUT
	
STOP
	B		STOP
	
	AREA	arraydata, DATA, READWRITE
ARRAY
	DCD		0x05,0x04,0x03,0x02,0x01
	END