#ifndef encoderVirtual_h
#define encoderVirtual_h

#include <Arduino.h>

class encoderVirtual
{
public:
encoderVirtual(uint32_t _ppr, uint32_t _diametroDaPolia): ppr{_ppr}, diametroDaPolia{_diametroDaPolia}
{

}

void setVelocidade_metrosPorMinuto(uint32_t _velocidade)
{
    velocidade_mmPorSegundo = _velocidade * 1000 / 60;
}

private:
uint32_t ppr = 2000; // pulsos por rotação
uint32_t diametroDaPolia = 60; // mm
uint32_t velocidade_mmPorSegundo = 200; //mm/s
uint32_t pulsosPorMm = round(ppr/((float)diametroDaPolia * 3.14));
uint32_t pulsosPorSegundo = pulsosPorMm * velocidade_mmPorSegundo;

};

#endif