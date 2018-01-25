#ifndef I2C_h
#define I2C_h

#include <avr/io.h>
#include <avr/interrupt.h>


/* Function Prototypes */

// Initiate I2C Bus with a given bit rate
void init_I2C(uint8_t bit_rate);

// Hold on I2C operations until the last one is finished
void I2C_waitForComplete();

// Initiate a start/restart condition on I2C Bus
void I2C_start();

// Initiate a stop condition on I2C Bus
void I2C_stop();

// In receiver mode, read and acknowledge
uint8_t I2C_readAck();

// In receiver mode, read but not acknowledge to end reading
uint8_t I2C_readNoAck();

// In transmitter mode, send a byte over I2C
void I2C_send(uint8_t byte);


#endif
