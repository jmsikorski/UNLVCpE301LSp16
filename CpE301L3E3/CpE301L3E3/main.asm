;
; CpE301L3E3.asm
;
; Created: 2/12/2016 3:14:25 PM
; Author : jmsikorski
;


; Replace with your application code
.org 0
	SBI DDRB,5 ;set PB5 as an output
	SBI DDRB,3 ;set PB5 as an output
	LDI R17,0x28 ;value is 0x88 to light up bit 5 & 3
	OUT PORTB,R17 ;sends value of R17 to corresponding bit