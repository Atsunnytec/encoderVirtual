#ifndef encoderVirtual_h
#define encoderVirtual_h

#include <Arduino.h>

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile int interruptCounter;

void IRAM_ATTR onTimer()
{
    portENTER_CRITICAL_ISR(&timerMux);
    interruptCounter++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

class encoderVirtual
{
public:
    encoderVirtual(uint32_t _ppr, uint32_t _diametroDaPolia) : ppr{_ppr}, diametroDaPolia{_diametroDaPolia}
    {
        calculos();
        configureTimer();
    }

    void setVelocidade_metrosPorMinuto(uint32_t _metrosPorMinuto)
    {
        velocidade_mmPorSegundo = _metrosPorMinuto * 1000 / 60;
        Serial.print("mm/s:");
        Serial.println(velocidade_mmPorSegundo);
        calculos();
        changeAlarmTimeout(usPorPulso);
    }

    void setVelocidade(uint32_t _mmPorSegundo)
    {
        velocidade_mmPorSegundo = _mmPorSegundo;
        Serial.print("mm/s:");
        Serial.println(velocidade_mmPorSegundo);
        calculos();
        changeAlarmTimeout(usPorPulso);
    }

    float getPulsosPorSegundo()
    {
        return pulsosPorSegundo;
    }

    uint32_t getCounter()
    {
        return interruptCounter;
    }

    void setCounter(uint32_t value)
    {
        portENTER_CRITICAL(&timerMux);
        interruptCounter = value;
        portEXIT_CRITICAL(&timerMux);
    }

    bool compareDistance(float milimetros)
    {
        uint32_t distance = round((milimetros * pulsosPorMm));
        return getCounter() == distance;
    }

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

    void calculos()
    {
        pulsosPorMm = ppr / ((float)diametroDaPolia * 3.14159);
        pulsosPorSegundo = (float)pulsosPorMm * velocidade_mmPorSegundo;
        usPorPulso = (float)1000000 / pulsosPorSegundo;
        ticksPorUs = freqDoESP32 / prescaler;

        Serial.print("us/p:");
        Serial.println(usPorPulso);
        Serial.print("p/mm:");
        Serial.println(pulsosPorMm);
        Serial.print("p/s:");
        Serial.println(pulsosPorSegundo);
    }

    void configureTimer()
    {
        timer = timerBegin(0, prescaler, true);
        timerAttachInterrupt(timer, onTimer, true);
        changeAlarmTimeout(usPorPulso);
        timerAlarmEnable(timer);
    }

    void changeAlarmTimeout(float valueInUs)
    {
        timerAlarmWrite(timer, round(ticksPorUs * valueInUs), true);
    }
};

#endif