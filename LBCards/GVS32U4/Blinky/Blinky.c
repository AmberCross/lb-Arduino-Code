#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

// Blinks LED connected between PB2 and VCC
// PB2 on GVS-32U4 board is near the reset switch wiring

void setup(void);
void loop(void);

int main (void)
{
	setup();
	while(1)
		loop();
}

void setup(void)
{
	CLKPR = 0x80; 	// Enable writing to the clock prescaler register
	CLKPR = 0x00;	// Clock divided by 1
	DDRB = 0x04;	// set PORTB2 LED pins for output
}

void loop(void)
{
	PORTB = 0x04;
	_delay_ms(500);
	PORTB = 0x04;
	_delay_ms(1000);
}
