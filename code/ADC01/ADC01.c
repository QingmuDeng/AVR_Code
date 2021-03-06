#define F_CPU (1000000L)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int main(void){
  //uint16_t reading;

  //setting up A1 pin
  DDRC &= ~(_BV(PC1));

  //setting PB1 to be input with pull up
  DDRB &= ~( _BV(PB1) );
  PORTB |= _BV(PB1);

  //Enabling ADCSRA with 128 pre-scaling
  ADCSRA |= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  //ADCSRA &= ~_BV(ADPS1);
  //AV CC with external capacitor at AREF pin
  ADMUX |= _BV(REFS0) | _BV(MUX0);

  //Enabling 8-bit Clock with no prescaling
  TCCR0B |= _BV(CS00);
  //Enabling PD6 as PWM output using PWM Phase Correct
  TCCR0A |= _BV(COM0A1) | _BV(WGM00);
  TCCR0A &= ~_BV(COM0A0);
  //Setting PD6/OCR0A to output
  DDRD |= _BV(PD6) | _BV(PD7);


  for(;;){
    ADCSRA |= _BV(ADSC);
    //equivalent of bit_is_set(ADCSRA, ADSC)
    while((ADCSRA & _BV(ADSC)) & _BV(ADSC)){
      PORTD ^= _BV(PD7);
    }

    uint16_t reading = ADC;
    OCR0A = (uint8_t) (reading>>2);

    // if(!(PINB & _BV(PB1))){
    //   PORTB |= _BV(PB0);
    // }else{
    //   PORTB &= ~(_BV(PB0));
    // }

  }
  return 0;
}
