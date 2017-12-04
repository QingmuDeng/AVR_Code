#define F_CPU (1000000L)
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

int main(void){
  DDRC = 0b00011111;
  DDRB = _BV(PB0);

  uint8_t i;
  int randNum;

  for(;;){
    PORTB ^= _BV(PB0);
    for(i = 0; i < 5; i++){
      PORTC |= _BV(i);
      _delay_ms(100);
    }
    PORTB ^= _BV(PB0);
    for(i = 0; i < 5; i++){
      PORTC &= ~(_BV(i));
      _delay_ms(100);
    }
    PORTB ^= _BV(PB0);
    _delay_ms(400);
    for(i = 5; i < 255; i--){
      PORTC |= _BV(i);
      _delay_ms(100);
    }
    PORTB ^= _BV(PB0);
    for(i = 5; i < 255; i--){
      PORTC &= ~(_BV(i));
      _delay_ms(100);
    }
    _delay_ms(400);
    randNum = random()%17;
    PORTC = randNum;
    _delay_ms(500);
  }
  return 0;

}
