#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

int16_t main()
{
	// PD0 -> izlaz
	DDRD = 1 << 0;
	// PB4 -> izlaz
	DDRB |= 1 << 4;
	// PB4 = 0, cime se ukljucuje PNP tranzistor
	PORTB &= ~(1 << 4);

	while (1)
	{
		PORTD |= 0x01;	// ukljucivanje diode
		_delay_ms(1000); // pauza 1s
		PORTD &= ~(0x01); // iskljucivanje diode
		_delay_ms(1000); // pauza 1s
	}
	return 0;

}
