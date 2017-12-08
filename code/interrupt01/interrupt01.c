#define FCPU (1000000L)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void setupInterrupt(){
  EIMSK |= _BV(INT1);
  EICRA |= _BV(ISC10);
  sei();
}

ISR(INT1_vect){
  if(!(PIND & _BV(PD3))){
    PORTD |= _BV(PD7);
  }else{
    PORTD &= ~_BV(PD7);
  }
}

int main(void){
  DDRD &= ~_BV(PD3);
  DDRD |= _BV(PD6) | _BV(PD7);
  setupInterrupt();
  //Pull up PD3
  PORTD |= _BV(PD3);

  for(;;){
    _delay_ms(200);
    PORTD ^= _BV(PD6);
  }
}
