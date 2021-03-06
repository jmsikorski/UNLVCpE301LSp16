;
; L5E1.asm
;
; Created: 2/26/2016 5:49:33 PM
; Author : jmsikorski
;


	SBI	DDRB, 1
BEGIN: 	
	LDI		R20, 0xF4
	STS		OCR1AH, R20
	LDI		R20, 0x23
	STS 	OCR1AL,R20		;OCR1A = 62,499
	LDI 	R20, 0x0B 
	STS 	TCCR1B, R20		;CTC, 8 prescaler, set on match
	LDI		R20, 0xC0
	STS		TCCR1A, R20
L1: 
	IN		R20, TIFR1
	SBRS 	R20,OCF1A	;skip next instruction if OCF1A = 1
	RJMP 	L1
	LDI		R16,1<<OCF1A 
	STS 	TIFR1,R16	;clear OCF1A		
	LDI		R20, 0xF4
	STS		OCR1AH, R20
	LDI		R20, 0x23
	STS 	OCR1AL,R20	;OCR1A = 62,499	
	LDI 	R20, 0x0B 
	STS 	TCCR1B, R20		;CTC, 8 prescaler, set on match
	LDI 	R20,0x80
	STS 	TCCR1A,R20	;CTC, clear on match
L2: 
	IN		R20, TIFR1
	SBRS 	R20,OCF1A	;skip next instruction if OCF1A = 1
	RJMP	L2
	LDI		R16,1<<OCF1A	;clear OCF0 
	STS		TIFR1,R16
	RJMP	BEGIN