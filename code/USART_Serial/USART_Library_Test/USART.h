#ifndef USART_h
#define USART_h

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef BAUD_TOL
#define BAUD_TOL 1
#endif


/* Function Prototypes */
void Serial_init();
void Serial_initInterrupt();
uint8_t Serial_available();
uint8_t Serial_transmitComplete();
uint8_t Serial_bufferEmpty();
void Serial_transmitByte(uint8_t data);
uint8_t Serial_receiveByte();
void Serial_printString(const char myString[]);

#endif
