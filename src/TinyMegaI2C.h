/******************************************************************************
  TinyMegaI2C.h
  i2c Library for TinyMegaCore based ATTINY Atmel chips.
  Saurabh Datta July 2021

  Development environment specifics:
  Arduino IDE:  1.8.13
  uC:           ATTINY-1607
******************************************************************************/

// #pragma once

// #ifndef TinyMegaI2CMaster_h
// #define TinyMegaI2CMaster_h

#ifndef TinyMegaI2C_h
#define TinyMegaI2C_h

#include <stdint.h>
#include <Arduino.h>
#include <avr/io.h>

// 400kHz clock
uint32_t const FREQUENCY = 400000L; // Hardware I2C clock in Hz
uint32_t const T_RISE = 300L;       // Rise time

class TinyMegaI2CMaster
{
public:
  // TinyMegaI2CMaster(void);
  TinyMegaI2CMaster(void);

  void init(void);

  uint8_t read(void);
  uint8_t readLast(void);

  bool write(uint8_t data);
  bool start(uint8_t address, int readcount);
  bool restart(uint8_t address, int readcount);
  void stop(void);

private:
  int I2Ccount;
};

extern TinyMegaI2CMaster TinyMegaI2C;

#endif
