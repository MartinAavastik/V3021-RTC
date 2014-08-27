/*
    V3021.cpp - Library for v3021 real-time clock.
    Created by Martin Aavastik, 14 august, 2014.
    Released into GPL v3.
*/

#include "Arduino.h"
#include "V3021.h"

V3021::V3021(uint8_t strPin, uint8_t clkPin, uint8_t ioPin)
{
    pinMode(strPin, OUTPUT);
    pinMode(clkPin, OUTPUT);
    pinMode(ioPin, OUTPUT);

    _strPin = strPin;
    _clkPin = clkPin;
    _ioPin = ioPin;

}

void V3021::begin()
{
    digitalWrite(_clkPin, HIGH);
    digitalWrite(_strPin, HIGH);
    digitalWrite(_ioPin, HIGH);

    // the very first time after power loss while initlizing you must read 8 times.
    _readData();

}

void V3021::allowClockChange()
{
    // allows to change the time
    _writeAdress(0x0);
    _writeData(0x0);
}

void V3021::disallowClockChange()
{
    // disallows to change the time
    _writeAdress(0x0);
    _writeData(0b00010000);
}

uint8_t V3021::_readData()
{
    // reads 8 bits(1 byte) of data.

    pinMode(_ioPin, INPUT);
    uint8_t byte = 0;
    uint8_t bit;

    for(uint8_t i = 0; i < 8; i++) {
        digitalWrite(_clkPin, LOW);
        bit = digitalRead(_ioPin);
        bitWrite(byte, i, bit);
        digitalWrite(_clkPin, HIGH);
    }

    pinMode(_ioPin, OUTPUT);
    return (byte);
}

uint8_t V3021::_read(uint8_t adress)
{
    //internal read function, it takes adress adress from where to read.
    uint8_t _tempdata;
    // must write F so time updates to RAM.
    _writeAdress(0xF);
    // write data adress.
    _writeAdress(adress);
    // return data
    _tempdata = _readData();
    // convert bcd to decimal.
    _tempdata = _bcdToDec(_tempdata);
    // return data
    return _tempdata;
}

void V3021::_write(uint8_t adress, uint8_t data)
{
    // internal write, takes adress and data to write.
    uint8_t _tempdata;
    _tempdata = _decToBcd(data);
    _writeAdress(adress);
    _writeData(_tempdata);
}

void V3021::_writeAdress(uint8_t adress)
{
    // writes data adress which is 4 bits and it can be from 0x0 to 0x9

    uint8_t bit;
    // when writing strPin must be low.
    digitalWrite(_strPin, LOW);

    for(uint8_t i = 0; i < 4; i++) {
        bit = bitRead(adress, i);
        digitalWrite(_ioPin, bit);
        // clock cycle so V3021 can read.
        digitalWrite(_clkPin, LOW);
        digitalWrite(_clkPin, HIGH);
    }
    digitalWrite(_strPin, HIGH); // reverting it to high.
}

void V3021::_writeData(uint8_t data)
{
    // write data. This must come after writing adress.
    uint8_t bit;
    // when writing strPin must be low.
    digitalWrite(_strPin, LOW);

    for(uint8_t i = 0; i < 8; i++) {
        bit = bitRead(data, i);
        digitalWrite(_ioPin, bit);
        // clock cycle so V3021 can read.
        digitalWrite(_clkPin, LOW);
        digitalWrite(_clkPin, HIGH);
    }
    digitalWrite(_strPin, HIGH); // reverting it to high.
}

uint8_t V3021::_bcdToDec(uint8_t data)
{
    // source: https://stackoverflow.com/questions/1408361/decimal-to-bcd-conversion
    return ( (data/16)*10) + (data%16);
}

uint8_t V3021::_decToBcd(uint8_t data)
{
    // same as previous
    return ( (data/10)*16) + (data%10);

}

uint8_t V3021::readSeconds()
{
    return _read(0x2);
}

uint8_t V3021::readMinutes()
{
    return _read(0x3);
}

uint8_t V3021::readHours()
{
    return _read(0x4);
}

uint8_t V3021::readDays()
{
    return _read(0x5);
}

uint8_t V3021::readMonths()
{
    return _read(0x6);
}

uint16_t V3021::readYears()
{
    return (2000 + _read(0x7) );
}

void V3021::setSeconds(uint8_t data)
{
    // write adress to RAM
    _write(0x2, data);
    // write RAM to internal clock which keeps time.
    // only works when allowClockChange() is previously called.
    _writeAdress(0xE);
}

void V3021::setMinutes(uint8_t data)
{
    _write(0x3, data);
    _writeAdress(0xE);
}

void V3021::setHours(uint8_t data)
{
    _write(0x4, data);
    _writeAdress(0xE);
}

void V3021::setDays(uint8_t data)
{
    _write(0x5, data);
    _writeAdress(0xE);
}

void V3021::setMonths(uint8_t data)
{
    _write(0x6, data);
    _writeAdress(0xE);
}

void V3021::setYears(uint16_t data)
{
    // year should be in format of 2007, but the clock itself takes only 07.
    uint16_t _tempData = data - 2000;
    _write(0x7, _tempData);
    _writeAdress(0xE);
}

