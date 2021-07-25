/******************************************************************************
  TinyMegaI2C.cpp
  i2c Library for TinyMegaCore based ATTINY Atmel chips.
  Saurabh Datta July 2021

  Development environment specifics:
  Arduino IDE:  1.8.13
  uC:           ATTINY-1607
******************************************************************************/

#include "TinyMegaI2C.h"

TinyMegaI2CMaster::TinyMegaI2CMaster(void)
{
}

void TinyMegaI2CMaster::init()
{
  pinMode(PIN_WIRE_SDA, INPUT_PULLUP);
  pinMode(PIN_WIRE_SCL, INPUT_PULLUP);
  uint32_t baud = ((F_CPU / FREQUENCY) - (((F_CPU * T_RISE) / 1000) / 1000) / 1000 - 10) / 2;
  TWI0.MBAUD = (uint8_t)baud;
  TWI0.MCTRLA = TWI_ENABLE_bm; // Enable as master, no interrupts
  TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

uint8_t TinyMegaI2CMaster::read(void)
{
  if (I2Ccount != 0)
    I2Ccount--;
  while (!(TWI0.MSTATUS & TWI_RIF_bm))
    ; // Wait for read interrupt flag
  uint8_t data = TWI0.MDATA;
  // Check slave sent ACK?
  if (I2Ccount != 0)
    TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc; // if ACK = more bytes to read
  else
    TWI0.MCTRLB = TWI_ACKACT_bm | TWI_MCMD_RECVTRANS_gc; // else Send NAK
  return data;
}

uint8_t TinyMegaI2CMaster::readLast(void)
{
  I2Ccount = 0;
  return TinyMegaI2CMaster::read();
}

bool TinyMegaI2CMaster::write(uint8_t data)
{
  while (!(TWI0.MSTATUS & TWI_WIF_bm))
    ; // Wait for write interrupt flag
  TWI0.MDATA = data;
  TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;   // Do nothing
  return !(TWI0.MSTATUS & TWI_RXACK_bm); // Returns true if slave gave an ACK
}

// Start transmission by sending address
bool TinyMegaI2CMaster::start(uint8_t address, int readcount)
{
  bool read;
  if (readcount == 0)
    read = 0; // Write
  else
  {
    I2Ccount = readcount;
    read = 1;
  }                                 // Read
  TWI0.MADDR = address << 1 | read; // Send START condition
  while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm)))
    ; // Wait for write or read interrupt flag
  if ((TWI0.MSTATUS & TWI_ARBLOST_bm))
    return false;                        // Return false if arbitration lost or bus error
  return !(TWI0.MSTATUS & TWI_RXACK_bm); // Return true if slave gave an ACK
}

bool TinyMegaI2CMaster::restart(uint8_t address, int readcount)
{
  return TinyMegaI2CMaster::start(address, readcount);
}

void TinyMegaI2CMaster::stop(void)
{
  TWI0.MCTRLB = TWI_ACKACT_bm | TWI_MCMD_STOP_gc; // Send STOP
}

TinyMegaI2CMaster TinyMegaI2C = TinyMegaI2CMaster();
