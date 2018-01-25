#define F_CPU (8000000UL)

#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include "USART.h"

#define WRITE_ADDRESS 0xA2
#define READ_ADDRESS 0xA3
#define SECOND_ADDRESS 0x02
#define SECOND_CORRECT 0b01111111
#define MINUTE_CORRECT 0b01111111
#define HOUR_CORRECT 0b00111111
#define DAY_CORRECT 0b00111111
#define WEEKDAY_CORRECT 0b00000111
#define MONTH_CORRECT 0b00011111

// void init_I2C(){
//   TWBR = 32;
//
//   TWCR |= _BV(TWEN);
//   //TWCR |= _BV(TWIE);
// }
//
// void I2C_waitForComplete(){
//   //loop_until_bit_is_set(TWCR, TWINT);
//   while( !(TWCR & (1<<TWINT)) );
//   //TWCR |= _BV(TWINT);
//   // while( !((TWCR & _BV(TWINT)) | _BV(TWINT)) );
// }
//
// void I2C_start(){
//   // reset TWI control register
// 	//TWCR = 0;
//   TWCR = _BV(TWEN) | _BV(TWSTA) | _BV(TWINT);
//   I2C_waitForComplete();
//   //TWCR |= _BV(TWINT);
//   //TWCR &= ~_BV(TWSTA);
// }
//
// void I2C_stop(){
//   TWCR = _BV(TWEN) | _BV(TWSTO) | _BV(TWINT);
//   TWCR &= ~_BV(TWSTA);
// }
//
// uint8_t I2C_readAck(){
//   TWCR = _BV(TWEA) | _BV(TWEN) | _BV(TWINT);
//   I2C_waitForComplete();
//   return TWDR;
// }
//
// uint8_t I2C_readNoAck(){
//   TWCR = _BV(TWEN) | _BV(TWINT);
//   TWCR &= ~_BV(TWEA);
//   I2C_waitForComplete();
//   return TWDR;
// }
//
// void I2C_send(uint8_t byte){
//   TWDR = byte;
//   TWCR = _BV(TWEN) | _BV(TWINT);
//   I2C_waitForComplete();
// }

static inline uint8_t bcd_decimal(uint8_t hex){
    //assert(((hex & 0xF0) >> 4) < 10);  // More significant nybble is valid
    //assert((hex & 0x0F) < 10);         // Less significant nybble is valid
    uint8_t dec = ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F);
    return dec;
}

static inline uint8_t decimal_bcd(uint8_t dec){
    uint8_t LSN = dec % 10;
    uint8_t MSN = ((dec - LSN) / 10) << 4;
    uint8_t hex = (MSN & 0xF0) + (LSN & 0x0F);
    return hex;
}

// void printDecimalByte(uint8_t byte) {
//               /* Converts a byte to a string of decimal text, sends it */
//   //char* a = byte / 100;
//   Serial_transmitByte('0' + (byte / 100));                        /* Hundreds */
//   Serial_transmitByte('0' + ((byte / 10) % 10));                      /* Tens */
//   Serial_transmitByte('0' + (byte % 10));                             /* Ones */
// }
//
// char nibbleToHexCharacter(uint8_t nibble) {
//                                    /* Converts 4 bits into hexadecimal */
//   if (nibble < 10) {
//     return ('0' + nibble);
//   }
//   else {
//     return ('0' + nibble - 10);
//   }
// }
//
// void printHexByte(uint8_t byte) {
//                         /* Prints a byte as its hexadecimal equivalent */
//   uint8_t nibble;
//   nibble = (byte & 0b11110000) >> 4;
//   Serial_transmitByte(nibbleToHexCharacter(nibble));
//   nibble = byte & 0b00001111;
//   Serial_transmitByte(nibbleToHexCharacter(nibble));
// }


int main(){
  // Set the mcu to run at 8 MHz with the internal oscillator
  clock_prescale_set(clock_div_1);
  DDRD |= _BV(PD7) | _BV(PD6);

  init_I2C(32);
  Serial_init();
  I2C_start();
  I2C_send(WRITE_ADDRESS);
  //I2C_start();
  //PORTD |= 1 << PD7;
  //I2C_start();
  I2C_send(SECOND_ADDRESS);
  //Serial_printString("\t");
  //I2C_stop();
  //_delay_ms(10);
  //I2C_start();
  // printHexByte(TWSR);
  // Serial_printString("\t");
  I2C_send(decimal_bcd(30)&SECOND_CORRECT);
  //I2C_stop();
  //I2C_start();
  //I2C_send(WRITE_ADDRESS);
  //I2C_start();
  //I2C_send(SECOND_ADDRESS+1);
  //I2C_start();
  I2C_send(decimal_bcd(39)&MINUTE_CORRECT);
  I2C_send(decimal_bcd(21)&HOUR_CORRECT);
  //I2C_stop();


  //I2C_send(SECOND_ADDRESS+1);
  // I2C_send(decimal_bcd(19)&MINUTE_CORRECT);
  //I2C_start();
  //I2C_send(SECOND_ADDRESS+2);
  //I2C_send(decimal_bcd(10)&HOUR_CORRECT);
  I2C_send(decimal_bcd(17)&DAY_CORRECT);
  I2C_send(decimal_bcd(3)&WEEKDAY_CORRECT);
  I2C_send(decimal_bcd(1)&MONTH_CORRECT);
  I2C_send(decimal_bcd(18));
  I2C_stop();
  PORTD |= 1 << PD7;

  uint8_t second, minute, hour, day, weekday, month, year;
  for(;;){
    I2C_start();
    I2C_send(WRITE_ADDRESS);
    //I2C_start();
    I2C_send(SECOND_ADDRESS);
    // I2C_start();
    // I2C_stop();
    // I2C_start();
    // I2C_stop();
    I2C_start();
    I2C_send(READ_ADDRESS);
    second = I2C_readAck() & SECOND_CORRECT;
    minute = I2C_readAck() & MINUTE_CORRECT;
    hour = I2C_readAck() & HOUR_CORRECT;
    day = I2C_readAck() & DAY_CORRECT;
    weekday = I2C_readAck() & WEEKDAY_CORRECT;
    month = I2C_readAck() & MONTH_CORRECT;
    year = I2C_readNoAck();
    I2C_stop();
    PORTD ^= 1 << PD6;
    printDecimalByte(bcd_decimal(hour));
    Serial_printString(":");
    printDecimalByte(bcd_decimal(minute));
    Serial_printString(":");
    printDecimalByte(bcd_decimal(second));
    Serial_printString(";\t");
    printDecimalByte(bcd_decimal(year));
    Serial_printString(".");
    printDecimalByte(bcd_decimal(month));
    Serial_printString(".");
    printDecimalByte(bcd_decimal(day));
    Serial_printString("\t");
    printDecimalByte(bcd_decimal(weekday));
    Serial_printString("\n");
    _delay_ms(500);
  }

  return 0;
}
