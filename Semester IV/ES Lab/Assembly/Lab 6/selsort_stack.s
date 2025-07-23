	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000
	DCD Reset_Handler
	
	AREA	selsort_stack, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
I		RN	R1
MAX		RN	R2
J		RN	R3
I_COUNT	RN	R11
J_COUNT	RN	R12

	LDR		R0, =ARRAY
	LDM		R0!, {R1-R10}
	STMEA	R13!, {R1-R10}
	
	MOV		I, R13
OUT
	MOV		MAX, I
	SUB		J, I, #4
	ADD		J_COUNT, I_COUNT, #1
IN
	LDMEA	MAX, {R4}
	LDMEA	J, {R5}
	CMP		R4, R5
	MOVLO	MAX, J
	
	SUB		J, #4
	ADD		J_COUNT, #1
	CMP		J_COUNT, #10
	BNE		IN
	
	LDMEA	MAX!, {R4}
	LDMEA	I!, {R5}
	STMEA	I, {R4}
	STMEA	MAX, {R5}
	ADD		I_COUNT, #1
	CMP		I_COUNT, #9
	BNE		OUT
	
	LDMEA	R13!, {R1-R10}
	STMDB	R0!, {R1-R10}
STOP
	B		STOP
	ALIGN
		
	AREA	arraydata, DATA, READONLY
ARRAY
	DCD		10,9,8,7,6,5,4,3,2,1
	END