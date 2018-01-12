#define F_CPU (8000000UL)

#include <avr/io.h>

#include <util/delay.h>
#include <avr/power.h>
#define EEPROM_READ 0b00000011
#define EEPROM_WRITE 0b00000010
#define EEPROM_WRDI 0b00000100
#define EEPROM_WREN 0b00000110
#define EEPROM_RDSR 0b00000101
#define EEPROM_WRSR 0b00000001
#define SLAVE_SELECT (PORTB |= _BV(PB1))
#define SLAVE_DESELECT (PORTB &= ~_BV(PB1))

void initSPI(uint8_t mode){
  //Set SCK(PB5) and MOSI(PB3) to output
  DDRB |= _BV(PB5) | _BV(PB3) | _BV(PB1);
  //Set MISO(PB4) and then AVR slave-select to input with internal pull-up
  DDRB &= ~_BV(PB2) & ~_BV(PB4);
  PORTD |= _BV(PB2);

  //Modes of Operations for SPI, Phases and Polarities
  //For Atmega328, see P219 of its datasheet for reference
  switch{
    case 0:
      SPCR0 &= ~_BV(CPOL0) & ~_BV(CPAH0);
    case 1:
      SPCR0 &= ~_BV(CPOL0);
      SPCR0 |= _BV(CPAH0);
    case 2:
      SPCR0 &= ~_BV(CPAH0);
      SPCR0 |= _BV(CPOL0);
    case 3:
      SPCR0 |= _BV(CPOL0) | _BV(CPAH0);
  }

  //Master Mode, 1/16 prescaler, Enable SPI
  SPCR0 |= _BV(MSTR0) | _BV(SPR01) |_BV(SPE0);
}

void SPI_tradeByte(uint8_t byte){
  SPDR0 = byte;
  //Loop until SPIF0 is set
  while(!(SPSR & _BV(SPIF0))){};
}

void EEPROM_address(uint16_t address){
  //Bit-shift 8 to the left to transmit the 8 MSBs
  SPI_tradeByte((uint8_t) (address >> 8));
  //Transmit the 8 LSBs
  SPI_tradeByte((uint8_t) address);
}

uint8_t EEPROMM_readStatus(){
  //Select the slave device
  SLAVE_SELECT;
  //Send EEPROM Status Register READ Instruction
  SPI_tradeByte(EEPROM_RDSR);
  SPI_tradeByte(0);
  //Deselect the slave device
  SLAVE_DESELECT;
  return SPDR0;
}

uint8_t EEPROM_readByte(uint16_t address){
  //Select the slave device
  SLAVE_SELECT;
  //Send EEPROM READ Instruction
  SPI_tradeByte(EEPROM_READ);
  //Send the address to read from
  EEPROM_address(address);
  SPI_tradeByte(0);
  //Deselect the slave device
  SLAVE_DESELECT;
  return SPDR0;
}

void EEPROM_writeEnable(){
  SLAVE_SELECT;
  SPI_tradeByte(EEPROM_WREN);
  SLAVE_DESELECT;
}

void EEPROM_writeDisable(){
  SLAVE_SELECT;
  SPI_tradeByte(EEPROM_WRDI);
  SLAVE_DESELECT;
}

void EEPROM_writeIn(uint16_t address, uint8_t byte){
  SLAVE_DESELECT;
  //loop until write-in is completed, i.e. when WIP is cleared
  while((EEPROMM_readStatus() & (0x01))){};
}

void EEPROM_writeSingleByte(uint16_t address, uint8_t byte){
  EEPROM_writeEnable();
  //Select the slave device
  SLAVE_SELECT;
  //Send EEPROM WRITE Instruction
  SPI_tradeByte(EEPROM_WRITE);
  //Send the address to write
  EEPROM_address(address);
  SPI_tradeByte(byte);
  //loop until the byte has been written
  EEPROM_writeIn();
}

void EEPROM_writeMax64Byte(uint16_t address, uint8_t byte){
  EEPROM_writeEnable();
  //Select the slave device
  SLAVE_SELECT;
  //Send EEPROM WRITE Instruction
  SPI_tradeByte(EEPROM_WRITE);
  //Send the address to write
  EEPROM_address(address);
  SPI_tradeByte(byte);
}

char EEPROM_readString(uint16_t address, uint8_t numByteToRead = 0){
  if(numByteToRead > 64) numByteToRead = 64;
  char *string = NULL;
  SLAVE_SELECT;
  SPI_tradeByte(EEPROM_READ);
  EEPROM_address(address);
  for(uint8_t i=0; i<numByteToRead; i++){
    SPI_tradeByte(0);
    string += (char) SPDR0;
  }
  SLAVE_DESELECT;
  return string;
}

int main(){
  //Set the mcu to run at 8 MHz with the internal oscillator
  clock_prescale_set(clock_div_1);

  initSPI(0);

  // DDRD |= _BV(PD7);
  //
  // for(;;){
  //   PORTD ^= _BV(PD7);
  //   _delay_ms(100);
  // }

  return 0;
}
