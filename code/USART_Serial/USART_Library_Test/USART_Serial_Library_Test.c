#define F_CPU (1000000UL)
#include "USART.h"


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t character;

int main(void){
  Serial_init();
  for(;;){
    while(1){
      if(Serial_available()){
        character = Serial_receiveByte();
        break;
      }
    }

    Serial_transmitByte(character);
    //char message01[] = "Hello";
    Serial_printString("\t");
    //_delay_ms(500);
  }
  return 0;
}
