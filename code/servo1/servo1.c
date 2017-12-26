#define F_CPU (16000000UL)
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint16_t pot_reading, servo_output;

uint16_t map(float x, float in_min, float in_max, float out_min, float out_max){
  return ((x - in_min) / (in_max-in_min) * (out_max-out_min) + out_min);
}

ISR(ADC_vect){
  PORTB |= _BV(PB1);
  pot_reading = ADC;
  servo_output = map((float)pot_reading, 0.0, 1024.0, 1700.0, 4500.0);
  ADCSRA |= _BV(ADSC);
}


void initTimer1Servo(void){
  //Clear on Match, Set on Bottom
  //Fasr PWM with ICR1 as TOP (part1)
  TCCR1A |= _BV(COM1A1) | _BV(WGM11);
  TCCR1A &= ~_BV(COM1A0);
  //1/8 Prescaling
  //Fasr PWM with ICR as TOP (part2)
  TCCR1B |= _BV(CS11) | _BV(WGM12) | _BV(WGM13);
  ICR1 = 40000;
  //Enable timer1 overflow interrupt
  //TIMSK1 |= _BV(TOIE);
  DDRD |= _BV(PD2);
}

void init_ADC5(void){
  DDRB &= ~_BV(PB2);
  // 1/128 Prescaling
  //ADC Interrupt Enabled
  ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
  //Use ADC5
  //Reference AVCC (part1)
  ADMUX |= _BV(MUX2) | _BV(MUX0) | _BV(REFS0);
  ADMUX &= ~_BV(REFS1);
  //Reference AVCC (part1)
  ADCSRB |= _BV(AREFEN);
}



int main(){
  sei();
  DDRB |= _BV(PB0) | _BV(PB1);
  PORTB |= _BV(PB0);
  init_ADC5();
  initTimer1Servo();
  ADCSRA |= _BV(ADSC);
  for(;;){
   OCR1A = servo_output;
  }

  return 0;
}
