/*
    V3021.h - Library for v3021 real-time clock.
    Created by Martin Aavastik, 14 august, 2014.
    Released into GPL v3.
*/
#ifndef V3021_h
#define V3021_h

#include "Arduino.h"

class V3021
{
    public:
        V3021(uint8_t strPin, uint8_t clkPin, uint8_t ioPin);
        void begin();

        uint8_t readSeconds();
        uint8_t readMinutes();
        uint8_t readHours();

        uint8_t readDays();
        uint8_t readMonths();
        uint16_t readYears();

        void setSeconds(uint8_t data);
        void setMinutes(uint8_t data);
        void setHours(uint8_t data);

        void setDays(uint8_t data);
        void setMonths(uint8_t data);
        void setYears(uint16_t data);

        void allowClockChange();
        void disallowClockChange();

    private:
        uint8_t _tempData;
        uint8_t _read(uint8_t adress);
        uint8_t _readData();
        uint8_t _readAdress(uint8_t adress);
        void _write(uint8_t adress, uint8_t data);
        void _writeAdress(uint8_t adress);
        void _writeData(uint8_t data);
        uint8_t _bcdToDec(uint8_t data);
        uint8_t _decToBcd(uint8_t data);

        uint8_t _strPin;
        uint8_t _clkPin;
        uint8_t _ioPin;
};

#endif
