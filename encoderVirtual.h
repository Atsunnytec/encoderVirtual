#ifndef encoderVirtual_h
#define encoderVirtual_h

#include <Arduino.h>

class encoderVirtual
{
public:
    encoderVirtual(uint32_t _ppr, uint32_t _diametroDaPolia) : ppr{_ppr}, diametroDaPolia{_diametroDaPolia}
    {
        calculos();
        configureTimer();
    }

    void setVelocidade_metrosPorMinuto(uint32_t _metrosPorMinuto);
    void setVelocidade(uint32_t _mmPorSegundo);
    float getPulsosPorSegundo();
    uint32_t getCounter();
    float getPosition();
    void setCounter(uint32_t value);
    bool compareDistance(float milimetros);
    // to do: pause()
    // to do: resume()

private:
    const int16_t prescaler = 8;     // !=0
    const uint32_t freqDoESP32 = 80; // MHz

    uint32_t ppr = 2000;                 // pulsos por rotação
    uint32_t diametroDaPolia = 60;       // mm
    float velocidade_mmPorSegundo = 200; //mm/s
    float pulsosPorMm = 10.61;
    float pulsosPorSegundo = 848.83;
    uint32_t ticksPorUs = 10;
    float usPorPulso = 471;

    hw_timer_t *timer = NULL;

    void calculos();
    void configureTimer();
    void changeAlarmTimeout(float valueInUs);
};

#endif