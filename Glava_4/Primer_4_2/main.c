#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

uint8_t t0_flag = 0;
uint8_t mask = 0xFF;

ISR(TIMER0_COMPA_vect)
{
	//prekid tajmer/brojac modula 0 usled dostizanja vrednosti registra OCR0A
	t0_flag = 1;
}

int16_t main()
{
	DDRB |= 1 << 5; // PB5 -> izlaz
	PORTB = 0xFF;

	//inicijalizacija tajmera 0:
	TCCR0A = 0x02;  // tajmer 0: CTC mod
	TCCR0B = 0x03;  // tajmer 0: fclk = fosc/64
	OCR0A = 123;    // perioda tajmera 0: 124 Tclk
	// (OCR0A + 1 = 124)
	TIMSK0 = 0x02;  // dozvola prekida tajmera 0
	// usled dostizanja vrednosti registra OCR0A

	sei(); // I = 1 (globalna dozvola prekida)

	while(1)
	{
		if (t0_flag)
		{
			t0_flag = 0;
			mask ^= (1 << 5);
			PORTB = mask;
		}
	}

	return 0;
}
