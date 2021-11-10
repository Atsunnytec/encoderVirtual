#ifndef encoderVirtual_h
#define encoderVirtual_h

#include <Arduino.h>

class encoderVirtual
{
public:
encoderVirtual(uint32_t _ppr, uint32_t _diametroDaPolia): ppr{_ppr}, diametroDaPolia{_diametroDaPolia}
{

}

private:
uint32_t ppr = 2000;
uint32_t diametroDaPolia = 60; // mm


};

#endif