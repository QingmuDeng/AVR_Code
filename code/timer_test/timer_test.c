#define F_CPU (1000000L)

#define STEPSIZE ((2048%2000)>>3)
#define FRACT_MAX (2000>>3)

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned long timer0_overflow_num = 0;
static volatile unsigned long timer0_millis = 0;
volatile unsigned long timer0_frac = 0;
volatile unsigned long counter = 0;
unsigned long time_now, timEr;
uint8_t previous;

static inline void initTimer0(void){
  //ENABLE THE FREAKING OVERFLOW INTERRUPT
  //BEFORE ANYTHING ELSE!!!!
  TIMSK0 |= _BV(0);

  //8 times prescaling
  TCCR0B |= _BV(CS01);
  TCCR0B &= ~_BV(CS00);
  TCCR0B &= ~_BV(CS02);
  sei();
}

ISR(TIMER0_OVF_vect){
  //See this website for details:
  /*https://ucexperiment.wordpress.com/2012/03/16/examination-of-the-arduino-millis-function/*/
  unsigned long m = timer0_millis;
  unsigned long f = timer0_frac;

  m += 2;
  f += STEPSIZE;
  if(f >= FRACT_MAX){
    f -= FRACT_MAX;
    m += 2;
  }

  timer0_millis = m;
  timer0_frac = f;
  timer0_overflow_num++;
  //PORTD ^= _BV(PB7);
  //_delay_ms(10);
}

unsigned long millis(){
  uint8_t oldSREG = SREG;
  cli();
  uint16_t m = timer0_millis;
  SREG = oldSREG;

  return m;
}

int main(void){
  initTimer0();
  //Set PD3 to input with pull-up
  DDRD &= ~_BV(PD3);
  PORTD |= _BV(PD3);
  //Set PD6 to Output
  DDRD |= _BV(PD6) | _BV(PD7) | _BV(PD5);
  uint8_t status;

  for(;;){
    status = PIND;
    time_now = millis();
    if( (!(status & _BV(PD3))) && (time_now - timEr > 100) && (status != previous)){
      PORTD ^= _BV(PD6);
      timEr = millis();
    }
    previous = status;
  }
  return 0;
}
