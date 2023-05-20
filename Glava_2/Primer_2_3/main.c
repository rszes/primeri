#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

int16_t main()
{
	// PC0 -> ulaz
	DDRC = 0x00;
	// PB4 -> izlaz
	DDRB |= 1 << 5;

	while (1)
	{
		if (!(PINC & 0x01))
			PORTB |= (1 << 5);
		else
			PORTB &= ~(1 << 5);
	}

	return 0;
}
