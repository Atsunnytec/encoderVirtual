#ifndef encoderVirtual_h
#define encoderVirtual_h

#include <Arduino.h>

class encoderVirtual
{
public:
    encoderVirtual(uint32_t _ppr, uint32_t _diametroDaPolia) : ppr{_ppr}, diametroDaPolia{_diametroDaPolia}
    {
    }

    void setVelocidade_metrosPorMinuto(uint32_t _velocidade)
    {
        velocidade_mmPorSegundo = _velocidade * 1000 / 60;
        Serial.print("mm/s:");
        Serial.println(velocidade_mmPorSegundo);
        calculos();
    }

    float getPulsosPorSegundo()
    {
        return pulsosPorSegundo;
    }

private:
    uint32_t ppr = 2000;                    // pulsos por rotação
    uint32_t diametroDaPolia = 60;          // mm
    float velocidade_mmPorSegundo = 200; //mm/s
    float pulsosPorMm = ppr / ((float)diametroDaPolia * 3.14159);
    float pulsosPorSegundo = pulsosPorMm * velocidade_mmPorSegundo;

    void calculos()
    {
        pulsosPorMm = ppr / ((float)diametroDaPolia * 3.14159);
        pulsosPorSegundo = pulsosPorMm * velocidade_mmPorSegundo;

        Serial.print("p/mm:");
        Serial.println(pulsosPorMm);
        Serial.print("p/s:");
        Serial.println(pulsosPorSegundo);
    }
};

#endif