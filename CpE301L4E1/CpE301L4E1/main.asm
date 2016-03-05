;
; CpE301L4E1.asm
;
; Created: 2/19/2016 4:47:00 PM
; Author : jmsikorski
;


; Replace with your application code
.MACRO INITSTACK		;Initialize the Stack Pointer
	LDI 	R16, HIGH(RAMEND)
	OUT 	SPH, R16
	LDI 	R16, LOW(RAMEND)
	OUT	SPL, R16
.ENDMACRO

INITSTACK
	LDI		R16, 0X30		;R16 5th and 4th bit is 1
	SBI		DDRB, 5			;Set PORTB.5 to output
	SBI		DDRB, 4
	LDI		R17, 0X10		;Clear R17
	OUT		PORTB, R17		;Clear PORTB

BEGIN:
	RCALL	DELAY			;Call DELAY subroutine
	EOR		R17, R16		;XOR R17 with 0x20, inverting 5th bit 
	OUT		PORTB, R17		;Output R17
	RJMP	BEGIN			;Jump to BEGIN

DELAY:
	LDI		R20, 0x48		;Load R20 with 0x85
	STS		TCNT1H, R20		;Store 0x85 into TCNT1 HIGH
	LDI		R20, 0XE5		;Load R20 with 0xE
	STS		TCNT1L, R20		;Store 0xee into TCNT1 LOW - TCNT1 = 34286
	LDI		R20, 0X00		;Clear R20
	STS		TCCR1A, R20		;Set WGM11:10 to 00
	LDI		R20, 0X05		
	STS		TCCR1B, R20		;Set WGM13:12 to 00, Normal mode, prescaler = 1024
AGAIN:
	IN		R20, TIFR1		;Read TIFR
	SBRS	R20, TOV1		;Check TOV1 bit, if 1 skip instruction
	RJMP	AGAIN
	LDI		R20, 0X00
	STS		TCCR1B, R20		;Stop Timer
	LDI		R20, 0X01
	OUT		TIFR1, R20		;Clear TOV1 flag
	RET						;Return from subroutine
	