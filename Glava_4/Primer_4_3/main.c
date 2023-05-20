#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

// Sistemsko vreme
volatile uint32_t ms = 0;

/**
* ISR - prekidna rutina tajmer/brojac modula 0 u CTC rezimu
*/
ISR(TIMER0_COMPA_vect)
{
	ms++;
}

/**
* timer0Init - Funkcija koja inicijalizuje timer 0 tako da pravi prekide
* svake milisekunde
*/
void timer0Init()
{
	TCCR0A = 0x02; // tajmer 0: CTC mod

	// Provera frekvencije takta prilikom kompilacije
	#if F_CPU > 20000000
	#error "Frekvencija takta mora biti manja od 20 MHz!"
	#endif

	// Inicijalizacija promenljivih za preskaler i periodu tajmera 0
	uint32_t n = F_CPU / 1000;
	uint8_t clksel = 1;

	// Odredjivanje vrednosti preskalera i periode tajmera 0
	while (n > 255)
	{
		n /= 8;
		clksel++;
	}

	TCCR0B = clksel; // tajmer 0: Podesavanje preskalera
	OCR0A = (uint8_t)(n & 0xff) - 1; // tajmer 0: Podesavanje periode
	TIMSK0 = 0x02; // tajmer 0: Dozvola prekida
	sei(); // Globalna dozvola prekida
}

/**
* timer0Millis - Funkcija koja, na bezbedan nacin, vraca kao povratnu vrednost broj milisekundi proteklih od pocetka merenja vremena
*/
uint32_t timer0Millis()
{
	uint32_t tmp;
	cli();		// Zabrana prekida
	tmp = ms;	// Ocitavanje vremena
	sei();      // Dozvola prekida
	return tmp;
}

/**
* timer0DelayMs - Funkcija koja implementira pauzu u broju milisekundi koji je prosledjen kao parametar
*/
void timer0DelayMs(uint32_t delay_length)
{
	uint32_t t0 = timer0Millis();
	while(timer0Millis() - t0 < delay_length);
}

int16_t main()
{
	DDRB |= 1 << 5; // PB -> izlaz
	PORTB = 0xFF;

	timer0Init();

	while(1)
	{
		timer0DelayMs(1000);
		PORTB |= 1 << 5;
		timer0DelayMs(1000);
		PORTB &= ~(1 << 5);
	}
}
