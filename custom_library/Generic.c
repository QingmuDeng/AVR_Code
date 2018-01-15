#include "Generic.h"

#if F_CPU == (1000000UL)
#define MILLIS_PRESCALE 3
#define MILLIS_RESOLUTION 2000UL
#elif F_CPU == (8000000UL)
#define MILLIS_PRESCALE 6
#define MILLIS_RESOLUTION 2000UL
#elif F_CPU == (1600000UL)
#define MILLIS_PRESCALE 6
#define MILLIS_RESOLUTION 1000UL
#else
ERROR();
#endif

#define TIMER0_CLOCK_FREQ (F_CPU >> MILLIS_PRESCALE)
#define FULLCYCLE_IN_MICROUNIT 25600000
#define TIMER0_OVERFLOW_MICROSEC (FULLCYCLE_IN_MICROUNIT/TIMER0_CLOCK_FREQ)
#define MILLIS_INCREMENT (TIMER0_OVERFLOW_MICROSEC/MILLIS_RESOLUTION)
#define FRAC_STEP ((TIMER0_OVERFLOW_MICROSEC-MILLIS_RESOLUTION) >> 3)
#define FRAC_MAX (MILLIS_RESOLUTION >> 3)

volatile unsigned long timer0_millis = timer0_frac = timer0_overflow_num = 0;

void init_millis(){
  // Enable global interrupt
  sei();
  // Enable Overflow interrupt
  TIMSK0 |= _BV(TOIE);
#if F_CPU == (1000000UL)
  // Timer 0 with 8 prescaler if F_CPU == 1,000,000
  TCCR0B |= _BV(CS01) | _BV(CS00);
#elif F_CPU == (8000000UL)
  // Timer 0 with 64 prescaler if F_CPU == 8,000,000
  TCCR0B |= _BV(CS01) | _BV(CS00);
#elif F_CPU == (1600000UL)
  // Timer 0 with 64 prescaler if F_CPU == 16,000,000
  TCCR0B |= _BV(CS01) | _BV(CS00);
#else
  ERROR();
#endif
}

ISR(TIMER0_OVF_vect){
  unsigned long f = timer0_frac;
  unsigned long m = timer0_millis;
  m += MILLIS_INCREMENT;
  f += FRAC_STEP;
  if(f>FRAC_MAX){
    f -= FRAC_MAX;
    m += MILLIS_INCR EMENT;
  }
  timer0_frac = f;
  timer0_millis = m;
  timer0_overflow_num++;
}

unsigned long millis(){
  oldSREG = SREG;
  cli();
  unsigned long m = timer0_millis;
  SREG = oldSREG;
  
  return m;
}
