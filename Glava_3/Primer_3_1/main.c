#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

ISR(PCINT0_vect)
{
	// Prekidna rutina za obradu prekida usled promene stanja pina PB5
	_delay_ms(500);
	PORTB ^= (1 << 5);
}

int16_t main()
{
	DDRB |= 1 << 5; // PB5 -> izlaz

	// Dozvola prekida usled promene stanja pina PB5
	PCICR = (1 << PCIE0);
	PCMSK0 = (1 << 5);

	sei(); // I = 1 (globalna dozvola prekida)

	// Inicijalna promena stanja pina PB5
	PORTB |= (1 << 5);

	while(1);
}
