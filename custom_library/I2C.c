#include "I2C.h"

void init_I2C(uint8_t bit_rate){
  TWBR = bit_rate;

  TWCR |= _BV(TWEN);
  //TWCR |= _BV(TWIE);
}

void I2C_waitForComplete(){
  //loop_until_bit_is_set(TWCR, TWINT);
  while( !(TWCR & (1<<TWINT)) );
  //TWCR |= _BV(TWINT);
  // while( !((TWCR & _BV(TWINT)) | _BV(TWINT)) );
}

void I2C_start(){
  // reset TWI control register
	//TWCR = 0;
  TWCR = _BV(TWEN) | _BV(TWSTA) | _BV(TWINT);
  I2C_waitForComplete();
  //TWCR |= _BV(TWINT);
  //TWCR &= ~_BV(TWSTA);
}

void I2C_stop(){
  TWCR = _BV(TWEN) | _BV(TWSTO) | _BV(TWINT);
  TWCR &= ~_BV(TWSTA);
}

uint8_t I2C_readAck(){
  TWCR = _BV(TWEA) | _BV(TWEN) | _BV(TWINT);
  I2C_waitForComplete();
  return TWDR;
}

uint8_t I2C_readNoAck(){
  TWCR = _BV(TWEN) | _BV(TWINT);
  TWCR &= ~_BV(TWEA);
  I2C_waitForComplete();
  return TWDR;
}

void I2C_send(uint8_t byte){
  TWDR = byte;
  TWCR = _BV(TWEN) | _BV(TWINT);
  I2C_waitForComplete();
}
