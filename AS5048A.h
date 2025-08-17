#pragma once

#include <Arduino.h>
#include <SPI.h>

class AS5048A {
   private:
    uint8_t _cs;
    SPISettings settings;

   public:
    AS5048A(uint8_t arg_cs, bool debug = false);
    void begin();
    uint8_t calcEvenParity(uint16_t value);
    uint16_t readRegister(uint16_t address);
    uint16_t writeRegister(uint16_t address, uint16_t data);
    uint16_t getAngleDigit();
    float getAngleDegrees();
    float getAngleRadians();
    String getErrors();
};