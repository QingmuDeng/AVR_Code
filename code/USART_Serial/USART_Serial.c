#define F_CPU (1000000UL)
#define BAUD (9600UL)
#define BAUD_TOL 1

\
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>

void init_USART(void){
  /* USART Baud Rate 0: Calculated within <util/setbaud.h>
  This is a 12-bit register which contains the USART baud rate. The UBRR0H
  contains the four most significant bits and the UBRR0L contains the eight
  least significant bits of the USART baud rate.*/
  UBRR0H = UBRRH_VALUE;
  UBRR0L = UBRRL_VALUE;

  //Double the USART Transmission Speed to acheive desired error tolerance
#if USE_2X
  UCSR0A |= _BV(U2X0);
#else
  UCSR0A &= ~_BV(U2X0);
#endif

  //Enable Receiver and Transmitter
  UCSR0B |= _BV(RXEN0) | _BV(TXEN0);

  //USART Character Size: 8 bit
  UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
}

void init_USART_int(void){
  /*Initial USART intterupts*/
  UCSR0B |= _BV(UDRIE0) | _BV(RXCIE0);
}

uint8_t serialAvailable(){
  /*RXC0: USART Receive Complete
  This flag bit is SET when there are UNREAD data in the receive buffer and
  cleared when the receive buffer is empty (i.e., does not contain any unread
  data). If the Receiver is disabled, the receive buffer will be flushed and
  consequently the RXC0 bit will become zero. The RXC0 Flag can be used to
  generate a Receive Complete interrupt (see description of the RXCIE0 bit).*/
  if ((UCSR0A & _BV(RXC0)) & _BV(RXC0)){
    return 1;
  }else{
    return 0;
  }
}

uint8_t serialTransmitComplete(){
  /*TXC0: USART Transmit Complete
  This flag bit is SET when the entire frame in the Transmit Shift Register has
  BEEN SHIFT OUT and there are no new data currently present in the transmit
  buffer (UDR0). The TXC0 Flag bit is automatically cleared when a transmit
  complete interrupt is executed, or it can be cleared by writing a one to its
  bit location. The TXC0 Flag can generate a Transmit Complete interrupt (see
  description of the TXCIE0 bit).*/
  if ((UCSR0A & _BV(TXC0)) & _BV(TXC0)){
    return 1;
  }else{
    return 0;
  }
}

uint8_t bufferEmpty(){
  /*UDRE0: USART Data Register Empty
  The UDRE0 Flag indicates if the transmit buffer (UDR0) is ready to receive new
  data. If UDRE0 is ONE, the buffer is EMPTY, and therefore READY TO BE WRITTEN.
  The UDRE0 Flag can generate a Data Register Empty interrupt (see description
  of the UDRIE0 bit). UDRE0 is set after a reset to indicate that the
  Transmitter is ready*/
  if ((UCSR0A & _BV(UDRE0)) & _BV(UDRE0)){
    return 1;
  }else{
    return 0;
  }
}

void transmitByte(uint8_t data){
  while(!bufferEmpty());
  PORTD |= _BV(PD6);
  UDR0 = data;
  while(!serialTransmitComplete());
  PORTD &= ~_BV(PD6);
}

uint8_t receiveByte(){
  return UDR0;
}

void printString(const char message[]){
  /*Send a series of character over Serial*/
  uint8_t i = 0;
  while(message[i]){
    transmitByte((uint8_t)message[i]);
    i++;
  }
}

uint8_t character;

int main(void){
  init_USART();
  for(;;){
    while(1){
      if(serialAvailable()){
        character = receiveByte();
        break;
      }
    }

    transmitByte(character);
    //char message01[] = "Hello";
    printString("Hello\t");
    //_delay_ms(500);
  }
  return 0;
}
