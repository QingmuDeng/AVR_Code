#define F_CPU (1000000L)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

static inline void initTimer1(void){
  TCCR1B |= _BV(CS11) | _BV(CS10);
}

int main(void){
  uint16_t timeValue;
  initTimer1();
  DDRC |= 0b00011111;
  DDRB |= _BV(PB0);
  DDRB &= ~( _BV(PB1) );
  PORTB |= _BV(PB1);

  for(;;){
    //PORTC = 0x1F;
    //_delay_ms(2000);
    //PORTC = 0x00;
    //TCNT1 = 0x00;

    if(!(PINB & _BV(PB1))){
      PORTB |= _BV(PB0);
    }else{
      PORTB &= ~(_BV(PB0));
      /*do{

      }while(PINB & _BV(PB1));
      timeValue = TCNT1;
      PORTB |= _BV(PB0);
      _delay_ms(timeValue);
      /*do{

      }while((TCNT1>>4) >= timeValue);*/
    //PORTB &= ~(_BV(PB0));
    //}
    }

  }
  return 0;
}
