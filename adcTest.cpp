/**********************************************/

#include <Wire.h>
#include "SPI.h"
#include "cpuID.h"
#include "dso_debug.h"
#include "dso_eeprom.h"
#include "TFT_eSPI_stm32duino.h" 
#include "stopWatch.h"
#include "myPwm.h"
#include "embedded_printf/printf.h"
#include "batterySensor.h"
        
simpleAdc *adc;

static float vcc;


#define ADC_VOLT_PIN PA3   // V
#define ADC_VOLT_PIN2 PA4  //A

static float averageMe(uint16_t *data, int nb)
{
    float sum=0;
    for(int i=0;i<nb;i++)     
    {
         sum+=*data;
         data+=2;
    }
    float n=nb;
    return sum/n;
}

void simpleTimeRead()
{
        
    uint16_t *samples;
    int nb=16;    
    adc->clearSamples();    
    
    if(!adc->timeSample( nb, &samples, 5000))
    {
        Logger("Error\n");
        return;
    }
     
    float voltage=averageMe(samples,nb)*vcc/4095.;    
    Logger("Value=%d\n",(int)(2000.*voltage));
}


void adcTest()
{
    Logger("ADC TEST\n");
    pinMode(ADC_VOLT_PIN,INPUT_FLOATING);
    pinMode(ADC_VOLT_PIN2,INPUT_FLOATING);
    
    adc=new simpleAdc(ADC_VOLT_PIN);
    vcc=adc->getVcc();
    Logger("VCC=%d\n",(int)vcc);
    xDelay(10);
    while(1)
    {
        simpleTimeRead();
        xDelay(1000);
    }
}
    