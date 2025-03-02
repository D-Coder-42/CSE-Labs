	AREA	RESET, DATA, READONLY
	EXPORT	__Vectors
__Vectors
	DCD	0x10001000
	DCD	Reset_Handler
	
	AREA	factorial, CODE, READONLY
	ENTRY
	EXPORT	Reset_Handler
Reset_Handler
	LDR		R0, =INPUT
	LDR		R1, =RESULT
	LDR		R2, [R0]
	BL		FACT
	STR		R2, [R1]
STOP
	B		STOP
	
FACT
	PUSH	{R3, LR}
	MOV		R3, R2
	CMP		R2, #0
	BNE		DOWN
	MOV		R2, #1
	B		DONE
DOWN
	SUB		R2, #1
	BL		FACT
	MUL		R2, R2, R3
DONE
	POP		{R3, LR}
	BX		LR
	
	ALIGN
INPUT
	DCD		0x04

	AREA	store, DATA, READWRITE
RESULT
	DCD		0x00
	END