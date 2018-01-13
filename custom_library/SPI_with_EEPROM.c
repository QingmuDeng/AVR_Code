#include "SPI_with_EEPROM.h"

void initSPI(uint8_t mode){
  //Set SCK(PB5) and MOSI(PB3) to output
  DDRB |= _BV(PB5) | _BV(PB3) | _BV(PB2);
  //Set MISO(PB4) and then AVR slave-select to input with internal pull-up
  DDRB &= ~_BV(PB4);
  PORTB |= _BV(PB4);

  //Modes of Operations for SPI, Phases and Polarities
  //For Atmega328, see P219 of its datasheet for reference
  switch(mode){
    case 0:
      SPCR &= ~_BV(CPOL) & ~_BV(2);
    case 1:
      SPCR &= ~_BV(CPOL);
      SPCR |= _BV(2);
    case 2:
      SPCR &= ~_BV(2);
      SPCR |= _BV(CPOL);
    case 3:
      SPCR |= _BV(CPOL) | _BV(2);
  }

  //Master Mode, 1/128 prescaler, Enable SPI
  SPCR |= _BV(MSTR) | _BV(SPR1) | _BV(SPE) | _BV(SPR0);
}

void SPI_tradeByte(uint8_t byte){
  SPDR = byte;
  loop_until_bit_is_set(SPSR, SPIF);
  //Loop until SPIF0 is set
  //while((SPSR & _BV(SPIF)) & _BV(SPIF)){};
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
  return SPDR;
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
  return SPDR;
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

void EEPROM_writeIn(){
  SLAVE_DESELECT;
  //loop until write-in is completed, i.e. when WIP is cleared
  while((EEPROMM_readStatus() & (0b00000001))){};
}

void EEPROM_writeSingleByte(uint16_t address, uint8_t byte){
  EEPROM_writeEnable();
  // Select the slave device
  SLAVE_SELECT;
  // Send EEPROM WRITE Instruction
  SPI_tradeByte(EEPROM_WRITE);
  // Send the address to write
  EEPROM_address(address);
  SPI_tradeByte(byte);
  // loop until the byte has been written
  EEPROM_writeIn();
}

void EEPROM_writeMax64Byte(uint16_t address, uint8_t byte){
  //Enable Write (outside this function)
  // Select the slave device (outside this function)
  // Send EEPROM WRITE Instruction
  SPI_tradeByte(EEPROM_WRITE);
  // Send the address to write
  EEPROM_address(address);
  SPI_tradeByte(byte);
}

//const char* EEPROM_readString(uint16_t address, size_t numByteToRead = 1);
void EEPROM_readStringToSerial(uint16_t address, uint16_t numByteToRead){
  // Select the slave device
  SLAVE_SELECT;
  // Send EEPROM READ Instruction
  SPI_tradeByte(EEPROM_READ);
  // Store the byte in the string arrary as char
  while(address < (address+numByteToRead)){
    // Send the address to read from
    EEPROM_address(address);
    SPI_tradeByte(0);
    Serial_transmitByte(SPDR);
    address++;
  }
  // Deselect the slave device
  SLAVE_DESELECT;
}

void EEPROM_writeString(uint16_t address, const char myString[]){
  EEPROM_writeEnable();
  SLAVE_SELECT;
  // Send EEPROM WRITE Instruction
  SPI_tradeByte(EEPROM_WRITE);
  // Send the address to write
  EEPROM_address(address);
  // Send the characters in myString one by one
  uint8_t i = 0;
  while(myString[i]){
    SPI_tradeByte(myString[i]);
    i++;
    // Break if more than 64 bytes are present
    if(i > 64){
      break;
    }
  }
  // Let EEPROM flash the bytes in
  EEPROM_writeIn();
}
