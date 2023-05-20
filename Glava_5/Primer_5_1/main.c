/*
* main.c - Aplikacija koja implementira ogranicen broj treptanja diode
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// Makro za podesavanje visoke vrednosti signala na pinu
#define HIGH 1
// Makro za podesavanje niske vrednosti signala na pinu
#define LOW 0

// Makro za podesavanje izlaznog smera pina
#define OUTPUT 1
// Makro za podesavanje ulaznog smera pina
#define INPUT 0

// Makro za selektovanje porta B
#define PORT_B 0
// Makro za selektovanje porta C
#define PORT_C 1
// Makro za selektovanje porta D
#define PORT_D 2

// Makro za selektovanje pina na koji je povezana dioda
#define DIODE_PIN 5

// Makro za podesavanje periode u brzom rezimu treptanja
#define FAST 200
// Makro za podesavanje periode u sporom rezimu treptanja
#define SLOW 1000

// Makro za podesavanje broja brzih treptaja
#define FAST_REPS 15
// Makro za podesavanje broja sporih treptaja
#define SLOW_REPS 3

// Promenljiva koja skladisti broj milisekundi proteklih od pokretanja aplikacije
volatile uint32_t ms = 0;

/*
* pinPulsing - Funkcija koja implementira num_of_repetitions perioda podizanja i spustanja vrednosti na pinu odgovarajucom brzinom
*/
void pinPulsing(uint8_t port, uint8_t pin, uint32_t period, uint8_t num_of_repetitions);

/*
* pinPulse - Funkcija koja implementiran podizanje i spustanje vrednosti na pinu odgovarajucom brzinom
*/
void pinPulse(uint8_t port, uint8_t pin, uint32_t period);

/*
* pinSetValue - Funkcija koja postavlja vrednost na pinu
*/
void pinSetValue(uint8_t port, uint8_t pin, uint8_t value);

/*
* pinInit - Funkcija koja implementira inicijalizaciju pina
*/
void pinInit(uint8_t port, uint8_t pin, uint8_t direction);

/*
* timer0DelayMs - Funkcija koja implementira pauzu u broju milisekundi koji je prosledjen kao parametar
*/
void timer0DelayMs(uint32_t delay_length);

/*
* timer0Millis - Funkcija koja, na bezbedan nacin, vraca kao povratnu vrednost broj milisekundi proteklih od pocetka merenja vremena
*/
uint32_t timer0Millis();

/*
* timer0Init - Funkcija koja inicijalizuje timer 0 tako da pravi prekide svake milisekunde
*/
void timer0Init();

/*
* calculateHalfPeriod - Funkcija koja izracunava polovinu prosledjene periode
*/
uint32_t calculateHalfPeriod(uint32_t period);

/*
* main - funkcija koja implementira glavni deo aplikacije
*/
int16_t main()
{
	// Inicijalizacija
	pinInit(PORT_B, DIODE_PIN, OUTPUT);
	timer0Init();

	// Glavna petlja
	while (1)
	{
		// Brzo treptanje
		pinPulsing(PORT_B, DIODE_PIN, FAST, FAST_REPS);

		// Sporo treptanje
		pinPulsing(PORT_B, DIODE_PIN, SLOW, SLOW_REPS);

		// Kraj
		while (1)
		;

	}

	return 0;
}

/***********************************************************/

void pinPulsing(uint8_t port, uint8_t pin, uint32_t period, uint8_t num_of_repetitions)
{
	uint8_t i;

	// Implementacija num_of_repetitions perioda
	for (i = 0; i < num_of_repetitions; i++)
		pinPulse(port, pin, period);
}

/***********************************************************/

void pinPulse(uint8_t port, uint8_t pin, uint32_t period)
{
	// Poluperioda u kojoj pin ima visoku vrednost
	pinSetValue(port, pin, HIGH);
	timer0DelayMs(calculateHalfPeriod(period));

	// Poluperioda u kojoj pin ima nisku vrednost
	pinSetValue(port, pin, LOW);
	timer0DelayMs(calculateHalfPeriod(period));
}

/***********************************************************/

void pinSetValue(uint8_t port, uint8_t pin, uint8_t value)
{
	// Postavljanje vrednosti pina
	switch(port)
	{
		case PORT_B:
			if (value == HIGH)
				PORTB |= 1 << pin;
			else
				PORTB &= ~(1 << pin);
		break;

		case PORT_C:
			if (value == HIGH)
				PORTC |= 1 << pin;
			else
				PORTC &= ~(1 << pin);
		break;

		case PORT_D:
			if (value == HIGH)
				PORTD |= 1 << pin;
			else
				PORTD &= ~(1 << pin);
		break;

		default:
		break;
	}
}

/***********************************************************/

void pinInit(uint8_t port, uint8_t pin, uint8_t direction)
{
	// Inicijalizacija smera pina
	switch (port)
	{
		case PORT_B:
			if (direction == OUTPUT)
				DDRB |= 1 << pin;
			else
				DDRB &= ~(1 << pin);
		break;

		case PORT_C:
			if (direction == OUTPUT)
				DDRC |= 1 << pin;
			else
				DDRC &= ~(1 << pin);
		break;

		case PORT_D:
			if (direction == OUTPUT)
				DDRD |= 1 << pin;
			else
				DDRD &= ~(1 << pin);
		break;

		default:
		break;
	}

}

/***********************************************************/

void timer0DelayMs(uint32_t delay_length)
{
	// trenutak pocevsi od kog se racuna pauza
	uint32_t t0 = timer0Millis();
	// implementacija pauze
	while(timer0Millis() - t0 < delay_length)
	;
}

/***********************************************************/

uint32_t timer0Millis()
{
	uint32_t tmp;
	cli();          // Zabrana prekida
	tmp = ms; 		// Ocitavanje vremena
	sei();          // Dozvola prekida
	return tmp;
}

/***********************************************************/

void timer0Init()
{
	// tajmer/brojac modul 0: CTC mod
	TCCR0A = 0x02;

	// Provera frekvencije takta prilikom kompilacije
	#if F_CPU > 20000000
	#error "Frekvencija takta mora biti manja od 20 MHz!"
	#endif

	// Inicijalizacija promenljivih za preskaler i periodu tajmer/brojac modula 0
	uint32_t n = F_CPU / 1000;
	uint8_t clksel = 1;

	// Odredjivanje vrednosti preskalera i periode tajmer/brojac modula 0
	while (n > 255)
	{
		n /= 8;
		clksel++;
	}

	// tajmer/brojac modul 0: Podesavanje preskalera
	TCCR0B = clksel;
	// tajmer/brojac modul 0: Podesavanje periode
	OCR0A = (uint8_t)(n & 0xff) - 1;
	// tajmer/brojac modul 0: Dozvola prekida
	TIMSK0 = 0x02;
	// Globalna dozvola prekida
	sei();
}

/***********************************************************/

/**
* ISR - prekidna rutina tajmer/brojac modula 0 u modu CTC
*/
ISR(TIMER0_COMPA_vect)
{
	// Inkrementovanje broja milisekundi koje su prosle od pokretanja aplikacije
	ms++;
}

/***********************************************************/

uint32_t calculateHalfPeriod(uint32_t period)
{
	return (period/2);
}
