#define F_CPU (1000000L)
#include <avr/io.h>
#include <util/delay.h>

int main(void){
  DDRD |= _BV(PD7);
  DDRD &= ~( _BV(PD3) );

  while(1){
    if(PIND & _BV(PD3) ){
      PORTD ^= _BV(PD7);
      _delay_ms(100);
    }else{
      PORTD &= ~(_BV(PD7) );
    }
  }

  return 0;
}
