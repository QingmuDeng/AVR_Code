#ifndef USART_h
  #define USART_h


  #include <avr/io.h>
  #include <avr/interrupt.h>
  #include <util/setbaud.h>

  class Serial{
    public:
      init(unsigned long baud);
      initInterrupt();
      available();
      transmitComplete();
      bufferEmpty();
      transmitByte(uint8_t data);
      receiveByte();
      printString(const char myString[]);
    private:
      _baud;
  }

#endif
