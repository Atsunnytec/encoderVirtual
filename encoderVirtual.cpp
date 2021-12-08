#include "encoderVirtual.h"

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile int interruptCounter;

void IRAM_ATTR onTimer()
{
    portENTER_CRITICAL_ISR(&timerMux);
    interruptCounter++;
    portEXIT_CRITICAL_ISR(&timerMux);
}

void encoderVirtual::setVelocidade_metrosPorMinuto(uint32_t _metrosPorMinuto)
{
    velocidade_mmPorSegundo = _metrosPorMinuto * 1000 / 60;
    Serial.print("mm/s:");
    Serial.println(velocidade_mmPorSegundo);
    calculos();
    changeAlarmTimeout(usPorPulso);
}

void encoderVirtual::setVelocidade(uint32_t _mmPorSegundo)
{
    velocidade_mmPorSegundo = _mmPorSegundo;
    Serial.print("mm/s:");
    Serial.println(velocidade_mmPorSegundo);
    calculos();
    changeAlarmTimeout(usPorPulso);
}

float encoderVirtual::getPulsosPorSegundo()
{
    return pulsosPorSegundo;
}

uint32_t encoderVirtual::getCounter()
{
    return interruptCounter;
}

float encoderVirtual::getPosition()
{
    return interruptCounter / pulsosPorMm;
}

void encoderVirtual::setCounter(uint32_t value)
{
    portENTER_CRITICAL(&timerMux);
    interruptCounter = value;
    portEXIT_CRITICAL(&timerMux);
}

bool encoderVirtual::compareDistance(float milimetros)
{
    uint32_t distanceInPulses = round((milimetros * pulsosPorMm));
    return getCounter() == distanceInPulses;
}

void encoderVirtual::calculos()
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

void encoderVirtual::configureTimer()
{
    timer = timerBegin(0, prescaler, true);
    timerAttachInterrupt(timer, onTimer, true);
    changeAlarmTimeout(usPorPulso);
    timerAlarmEnable(timer);
}

void encoderVirtual::changeAlarmTimeout(float valueInUs)
{
    timerAlarmWrite(timer, round(ticksPorUs * valueInUs), true);
}