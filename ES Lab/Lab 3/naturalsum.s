	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000	;SP
	DCD Reset_Handler

	AREA	natsum, CODE, READONLY
	ENTRY
	EXPORT Reset_Handler
Reset_Handler
	LDR	R0, =N
	LDR	R1, [R0]
	MLA	R2, R1, R1, R1	;R2 = N*N + N
	LSR	R2, #1			;R2 = R2/2
	LDR	R0, =RES
	STR	R2, [R0]
STOP
	B	STOP
	ALIGN
N
	DCD	0x64
	
	AREA	data, DATA, READWRITE
RES
	DCD	0x00
	END
