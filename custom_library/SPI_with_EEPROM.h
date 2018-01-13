#ifndef SPI_with_EEPROM_h
#define SPI_with_EEPROM_h

#include <avr/io.h>
#include "USART.h"

#define EEPROM_READ 0b00000011
#define EEPROM_WRITE 0b00000010#ifndef SS_PORT
#define SS_PORT PORTB
#endif
#ifndef SS_PIN
#define SS_PIN PB2
#endif
#define EEPROM_WRDI 0b00000100
#define EEPROM_WREN 0b00000110
#define EEPROM_RDSR 0b00000101
#define EEPROM_WRSR 0b00000001

#ifndef SS_PORT
#define SS_PORT PORTB
#endif
#ifndef SS_PIN
#define SS_PIN PB2
#endif

#define SLAVE_SELECT (SS_PORT &= ~_BV(SS_PIN))
#define SLAVE_DESELECT (SS_PORT |= _BV(SS_PIN))

void initSPI(uint8_t mode);
void SPI_tradeByte(uint8_t byte);
void EEPROM_address(uint16_t address);
uint8_t EEPROMM_readStatus();
uint8_t EEPROM_readByte(uint16_t address);
void EEPROM_writeEnable();
void EEPROM_writeDisable();
void EEPROM_writeIn();
void EEPROM_writeSingleByte(uint16_t address, uint8_t byte);
void EEPROM_writeMax64Byte(uint16_t address, uint8_t byte);
void EEPROM_readStringToSerial(uint16_t address, uint16_t numByteToRead);
void EEPROM_writeString(uint16_t address, const char myString[]);

#endif
