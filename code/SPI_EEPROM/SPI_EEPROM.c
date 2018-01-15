#define F_CPU (8000000UL)

// #include <stdio.h>
// #include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include "USART.h"
#include "SPI_with_EEPROM.h"

int main(){
  //Set the mcu to run at 8 MHz with the internal oscillator
  clock_prescale_set(clock_div_1);
  Serial_init();
  initSPI(0, 128);

  // DDRD |= _BV(PD7);
  // for(;;){
  //   PORTD ^= _BV(PD7);
  //   _delay_ms(100);
  // }
  char character;
  EEPROM_writeSingleByte(0x0, (uint8_t)"");
  uint16_t test_address = 0x1;
  const char* myString = "Fulfill your destiny!\n";
  EEPROM_clearAll();
  for(;;){
    // while(1){
    //   if(Serial_available()){
    //     character = Serial_receiveByte();
    //     break;
    //   }
    // }
    //
    // EEPROM_writeSingleByte(test_address, (uint8_t)character);
    // Serial_transmitByte((uint8_t)character);
    // EEPROM_writeString(test_address, myString);
    //EEPROM_readStringToSerial((uint16_t)0x1, (uint16_t) 25);
    //Serial_transmitByte(EEPROM_readByte(test_address));
    //test_address++;
  }

  return 0;
}
