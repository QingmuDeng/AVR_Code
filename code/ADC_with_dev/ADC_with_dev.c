#define F_CPU (1000000UL)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint16_t reading;

ISR(ADC_vect){
  PORTB ^= _BV(PB1);
  reading = ADC;
  ADCSRA |= _BV(ADSC);
}

void init_ADC5(void){
  //Set PB2 to Input
  DDRB &= ~_BV(PB2);
  //Enable ADC and ADC Interrupt
  ADCSRA |= _BV(ADEN) | _BV(ADIE) |_BV(ADPS2) |_BV(ADPS1) |_BV(ADPS0);
  //Connect ADC5 to ADC Input
  ADMUX |= _BV(MUX2) |_BV(MUX0); //|_BV(ADLAR);
  //Set AREF TO AVCC
  ADMUX |= _BV(REFS0);
  ADMUX &= ~_BV(REFS1);
  //Enable AREF Pin
  ADCSRB |= _BV(AREFEN);
}

void init_PWM1(void){
  //10 bit Phase Correct PWM
  TCCR1A |= _BV(WGM10) | _BV(WGM11);
  TCCR1A |= _BV(COM1A1);
  TCCR1A &= ~_BV(COM1A0);
  DDRD |= _BV(PD2);
  //No prescaling
  TCCR1B |= _BV(CS10);
}

int main(void){
  //Enable Global Interrupt
  sei();

  init_ADC5();
  init_PWM1();

  //Set PB0 and PB1 to output
  DDRB |= _BV(PB0) | _BV(PB1);
  PORTB |= _BV(PB0);

  ADCSRA |= _BV(ADSC);

  for(;;){
    PORTB ^= _BV(PB0);
    OCR1A = reading;
  }

  return 0;
}
