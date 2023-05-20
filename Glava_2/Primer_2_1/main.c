#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

int16_t main()
{
	DDRB |= 1 << 5; 		//PB5 je izlaz

	while (1)
	{
		PORTB |= 1 << 5; 	//dioda je ukljucena
		_delay_ms(1000); 	//pauza 1s
		PORTB &= ~(1 << 5); //dioda je iskljucena
		_delay_ms(1000); 	//pauza 1s
	}
	return 0;
}
