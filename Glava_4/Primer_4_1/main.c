#include <avr/io.h>
#include <avr/interrupt.h>

//#define NORMAL_TOGGLE
//#define NORMAL_CLEAR
//#define NORMAL_SET

//#define CTC_TOGGLE

//#define FAST_PWM_0_NON_INVERTED
//#define FAST_PWM_0_INVERTED

//#define FAST_PWM_1_TOGGLE

//#define PC_PWM_0_NON_INVERTED
//#define PC_PWM_0_INVERTED

//#define PC_PWM_1_TOGGLE

ISR(TIMER0_OVF_vect)
{
	PORTD |= 0x01;
	PORTD ^= 0x04;
}

ISR(TIMER0_COMPA_vect)
{
	PORTD &= ~0x01;
	PORTD ^= 0x10;
}

int16_t main()
{
	DDRB |= 1 << 4;
	DDRD = 0xFF;
	PORTD = 0xff;
	PORTB &= ~(1 << 4);

	#ifdef NORMAL_TOGGLE
	TCCR0A = 0x40; // 0100 0000
	TCCR0B = 0x05; // 0000 0101
	#endif

	#ifdef NORMAL_CLEAR
	TCCR0A = 0x80; // 1000 0000
	TCCR0B = 0x05; // 0000 0101
	#endif

	#ifdef NORMAL_SET
	TCCR0A = 0xC0; // 1100 0000
	TCCR0B = 0x05; // 0000 0101
	#endif

	#ifdef CTC_TOGGLE
	TCCR0A = 0x42; // 0100 0010
	TCCR0B = 0x05; // 0000 0101
	#endif

	#ifdef FAST_PWM_0_NON_INVERTED
	TCCR0A = 0x83; // 1000 0011
	TCCR0B = 0x05; // 0000 0101
	#endif

	#ifdef FAST_PWM_0_INVERTED
	TCCR0A = 0xC3; // 1100 0011
	TCCR0B = 0x05; // 0000 0101
	#endif

	#ifdef FAST_PWM_1_TOGGLE
	TCCR0A = 0x43; // 0100 0011
	TCCR0B = 0x0D; // 0000 1101
	#endif

	#ifdef PC_PWM_0_NON_INVERTED
	TCCR0A = 0x83; // 1000 0001
	TCCR0B = 0x05; // 0000 0101
	#endif

	#ifdef PC_PWM_0_INVERTED
	TCCR0A = 0xC3; // 1100 0001
	TCCR0B = 0x05; // 0000 0101
	#endif

	#ifdef PC_PWM_1_TOGGLE
	TCCR0A = 0x43; // 0100 0001
	TCCR0B = 0x0D; // 0000 1101
	#endif

	OCR0A = 250;
	TIMSK0 = 0x03;
	sei();

	while(1);
}
