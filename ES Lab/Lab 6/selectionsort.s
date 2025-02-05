	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000
	DCD Reset_Handler
	
	AREA	selsort, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler

ARR	RN		R0
I	RN		R1
J	RN		R2
MIN	RN		R3

	LDR		ARR, =ARRAY
	MOV		I, #0x00
	
OUT
	CMP		I, #0x04
	BHS		STOP
	MOV		MIN, I
	ADD		J, I, #0x01
	
IN
	CMP		J, #0x05
	BHS 	EXIT_IN
	ADD		R4, ARR, MIN, LSL #2
	ADD		R5, ARR, J, LSL #2
	LDR		R4, [R4]
	LDR 	R5, [R5]
	CMP		R4, R5
	MOVHI	MIN, J
	ADD		J, #0x01
	B		IN
	
EXIT_IN
	ADD		R4, ARR, MIN, LSL #2
	ADD		R5, ARR, I, LSL #2
	LDR		R4, [R4]
	LDR		R5, [R5]
	STR		R5, [ARR, MIN, LSL #2]
	STR		R4, [ARR, I, LSL #2]
	
	ADD		I, #0x01
	B		OUT
	
STOP
	B		STOP
	
	AREA	arraydata, DATA, READWRITE
ARRAY
	DCD	0x05, 0x04,0x03,0x02,0x01
	END