#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "vgmPlayer.h"
#define BAUD 1048576
#include <util/setbaud.h>

#define BUFF_SIZE 64

unsigned char buffer[BUFF_SIZE];
char log_buffer[100];

volatile int counter = 0;
//Serial.println(TCNT1); // value of the counter
void my_delay(int ms) {
	counter = 0;
	//4.09ms pro tick
	int new_ms = (ms / 4.09);
	while (counter < new_ms) {
	}
}

void readNext(void) {
	for (int i = 0; i < BUFF_SIZE; i++) {
		unsigned char c = uart_getchar();
		buffer[i] = c;
	}
}

void debug_str(char* str) {
	for (int i = 0; str[i] != '\0'; i++) {
		uart_putchar(str[i]);
	}
	uart_putchar('\0');
}

int main(void) {
	//init
	YM_CTRL_DDR |=
	_BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD) | _BV(YM_A0) | _BV(YM_A1);
	YM_DATA_DDR = 0xff;
	DDRB = 0xff;

	YM_CTRL_PORT |= _BV(YM_IC) | _BV(YM_CS) | _BV(YM_WR) | _BV(YM_RD); /* IC, CS, WR and RD HIGH by default */
	YM_CTRL_PORT &= ~(_BV(YM_A0) | _BV(YM_A1)); /* A0 and A1 LOW by default */

	init_timer1();
	uart_init();

	/* Reset YM2612 */
	YM_CTRL_PORT &= ~_BV(YM_IC);
	_delay_ms(10);
	YM_CTRL_PORT |= _BV(YM_IC);
	_delay_ms(10);

	unsigned long samples = 0;
	unsigned int last_time = TCNT1;
	unsigned int delay = 0;

	for (;;) {
		//sprintf(log_buffer, "TCNT1: %u us: %u", TCNT1, delay * 4);
		//debug_str(log_buffer);
		delay = TCNT1 - last_time;
		last_time = TCNT1;

		uart_putchar('k');
		readNext();
		//sprintf(log_buffer, "%u %u", buffer[0], buffer[1]);
		//debug_str(log_buffer);


		/*
		for (long i = 0; i < sizeof(buffer) - 2; i++) {
			unsigned char b1 = buffer[i];
			unsigned char b2 = buffer[i + 1];
			unsigned char b3 = buffer[i + 2];
			unsigned long timeout = 0;

			if (b1 == 0x52) {
				write_data(b2, b3, PORT_0);
				i += 2;
			} else if (b1 == 0x53) {
				write_data(b2, b3, PORT_1);
				i += 2;
			} //0x50  dd  PSG (SN76489/SN76496) write value dd
			else if (b1 == 0x50) {
				// skip
				i += 1;
			} //0x61  nn nn Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
			else if (b1 == 0x61) {
				//All integer values are *unsigned* and written in "Intel" byte order (Little Endian), so for example 0x12345678 is written as 0x78 0x56 0x34 0x12.
				uint16_t wait_n = b2 << 0 | (b3 << 8);
				samples += wait_n;
				timeout = (wait_n * 1000000) / 44100;

				i += 2;
			} else if (b1 >= 0x70 && b1 <= 0x7f) {
				uint16_t wait_n = b1 & 0x0f;
				samples += wait_n;
				timeout = (wait_n * 1000000) / 44100;
			} else {
			}

			if (timeout > 0) {
				uint16_t cur_clock = TCNT1;
				 while(TCNT1 - cur_clock < timeout * 16 ) {

				 }
				for (unsigned long c = 0; c < timeout / 22; c++) {
					_delay_us(22);
				}
			}
		}
		 */
	}
	return 0;
}

/* data direction register to enable ports
 DDRC = A0-A5
 DDRD = D0-D7
 DDRB = D8-D13
 */
/**
 A1 A0
 0 0 - Writes address to PORT_0
 0 1 - Writes data to the selected PART PORT_0 address
 1 0 - Writes address to PORT_1
 1 1 - Writes data to the selected PART PORT_1 address
 */

void write_data(uint8_t reg, uint8_t data, uint8_t port) {
	if (port == PORT_0) {
		YM_CTRL_PORT &= ~(_BV(YM_A1) | _BV(YM_A0)); // a1 and a0 low
	} else {
		YM_CTRL_PORT &= ~(_BV(YM_A0)); // a0 low
		YM_CTRL_PORT |= _BV(YM_A1); // a1 high
	}
	write_ym(reg);

	if (port == PORT_0) {
		YM_CTRL_PORT &= ~(_BV(YM_A1)); // a1 low
		YM_CTRL_PORT |= _BV(YM_A0); // a0 high
	} else {
		YM_CTRL_PORT |= _BV(YM_A1) | _BV(YM_A0); // a1 and a0 high
	}
	write_ym(data);
}

void write_ym(uint8_t data) {
	YM_CTRL_PORT &= ~_BV(YM_CS); // CS LOW
	PORTB = data >> 6;
	YM_DATA_PORT = data << 2;
	_delay_us(2);
	YM_CTRL_PORT &= ~_BV(YM_WR); // Write data
	_delay_us(5);
	YM_CTRL_PORT |= _BV(YM_WR);
	_delay_us(5);
	YM_CTRL_PORT |= _BV(YM_CS); // CS HIGH
}

void uart_init(void) {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));
#endif

	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */
}

void uart_putchar(char c) {
	loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
	UDR0 = c;
}

char uart_getchar(void) {
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}

ISR(TIMER1_OVF_vect)
 {
 counter++;
 }

void init_timer1(void) {
	cli();
	// disable global interrupts
	TCCR1A = 0; // reset register
	TCCR1B = 0;

	TIMSK1 |= (1 << TOIE1); // enable overflow interrupt
//TCNT1 can be used for a timer start value

	/**
	 CS12  CS11  CS10  Description
	 0 0 0 No clock source (Timer/Counter stopped)
	 0 0 1 clki/o/1 (No prescaling)      62ns   4ms
	 0 1 0 clki/o/8 (From Prescaler)    0.5us  32ms
	 0 1 1 clki/o/64 (From Prescaler)     4us 262ms
	 1 0 0 clki/o/256 (From Prescaler)   16us   1s
	 1 0 1 clki/o/1024 (From Prescaler)  64us   4s
	 1 1 0 External clock source on T1 pin. Clock on falling edge
	 1 1 1 External clock source on T1 pin. Clock on rising edge
	 */
	TCCR1B |= (1 << CS11) | (1 << CS10);
	//TCCR1B |= (1 << CS12);
	//TCCR1B |= (1 << CS10);
// 1 / (16*10^6 / 1) * 65535
// = 0.00409
	sei();

}

/*
 if(CHECKBIT(TIFR1, TOV1)) {
 // reset overflow flag
 TIFR1 = _BV(TOV1);
 break;*/

