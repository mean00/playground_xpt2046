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
#define BAT_ENABLE      PB10 

#define ADC_VOLT_PIN PA3   // V
#define ADC_VOLT_PIN2 PA4  //A

static float averageMe(uint16_t *data, int nb,int interleave)
{
    float sum=0;
    for(int i=0;i<nb;i++)     
    {
         sum+=*data;
         data+=interleave;
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
    float voltage=averageMe(samples,nb,1)*vcc/4095.;    
    voltage=voltage*2./1000.; // in volt    
    Logger("SimpleTime=%f\n",(voltage));
}
/***
 */
void simpleNormalRead()
{
        
    uint16_t *samples;
    int nb=16;    
    adc->clearSamples();    
    adc->changePin(ADC_VOLT_PIN);
    if(!adc->sample( nb, &samples, ADC_SMPR_13_5,DSOADC::ADC_PRESCALER_6))
    {
        Logger("Error\n");
        return;
    }
    float voltage=averageMe(samples,nb,1)*vcc/4095.;    
    voltage=voltage*2./1000.; // in volt    
    Logger("Simple=%f\n",(voltage));
}
/**
 * 
 */
void doubleTimeRead()
{
    uint16_t *samples;
    int nb=16;    
    adc->clearSamples();
    adc->changePin(ADC_VOLT_PIN);
    if(!adc->dualTimeSample(ADC_VOLT_PIN2,nb,&samples,5000))
    {
         Logger("Failed\n");
         return ;
    }
    nb>>=1;       
    float voltage=averageMe(samples,nb,2)*vcc/4095.;    
    float current=averageMe(samples+1,nb,2)*vcc/4095.;
    voltage*=2./1000.;
    current=current*500./1440.;
    Logger("DoubleTime=v=%fV a=%dmV\n",voltage,(int)current);
}
/**
 * 
 */
void adcTest()
{
    Logger("ADC TEST\n");
    pinMode(ADC_VOLT_PIN,INPUT_FLOATING);
    pinMode(ADC_VOLT_PIN2,INPUT_FLOATING);
    
    adc=new simpleAdc(ADC_VOLT_PIN);
    vcc=adc->getVcc();
    Logger("VCC=%d\n",(int)vcc);
    digitalWrite(  BAT_ENABLE,0);
    xDelay(10);
    while(1)
    {
        Logger("vcc=%f\n",vcc/1000.);
        simpleTimeRead();
        simpleNormalRead();
     //   doubleTimeRead();
        xDelay(1000);
    }
}
    