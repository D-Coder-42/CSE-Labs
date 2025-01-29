; This program is incorrect
; 1. carry propogation is incorrect
; 2. the result is unpacked (can be easily fixed)

	AREA	RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD	0x10001000
	DCD	Reset_Handler
	
	AREA	BCDadd, CODE, READONLY
	ENTRY
	EXPORT	Reset_Handler
Reset_Handler
	LDR		R0, =BCD1
	LDR		R1, =BCD2
	LDR		R2, =RESULT
	MOV		R3, #4
	MOV		R4, #0

LOOP
	LDRB	R5, [R0], #1
	LDRB	R6, [R1], #1
	AND		R7, R5, #0x0F
	AND		R8, R6, #0x0F
	ADD		R9, R7, R8
	ADD		R9, R9, R4
	CMP		R9, #0x0A
	BLO		STORE_LOW
	ADD		R9, R9, #0x06
	MOVLO	R4, #0
	MOVHS	R4, #1

STORE_LOW
	STRB	R9, [R2], #1

	AND		R7, R5, #0xF0
	MOV		R7, R7, LSR #4
	AND		R8, R6, #0xF0
	MOV		R8, R8, LSR #4
	ADD		R9, R7, R8
	ADD		R9, R9, R4
	CMP		R9, #0x0A
	BLO		STORE_UP
	ADD		R9, R9, #0x06
	MOVLO	R4, #0
	MOVHS   R4, #1

STORE_UP
	STRB	R9, [R2], #1

	SUBS	R3, R3, #1
	BNE     LOOP

STOP
	B 	 STOP
	ALIGN

BCD1 
	DCD 0x12335679
BCD2 
	DCD 0x87654321

	AREA store, DATA, READWRITE 
RESULT 
	DCD 0x00
	END