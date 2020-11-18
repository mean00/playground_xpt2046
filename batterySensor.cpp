
#include "batterySensor.h"
#include "dso_debug.h"


int VCalibration5v=2483; // Voltage *1000 when 5v is applied

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
/**
 */
 BatterySensor::BatterySensor(int pinV, int pinA)
 {
     
    pinMode(pinV,INPUT_FLOATING);
    pinMode(pinA,INPUT_FLOATING);
     
    _pinA=pinA;
    _pinV=pinV;
    _adc=new simpleAdc(_pinV);
    _vcc=_adc->getVcc();
 }
 
/**
 */
bool  BatterySensor::rawRead( float &voltage,float &current)
{
    
    uint16_t *samples;
    int nb=16;    
    _adc->clearSamples();
    _adc->changePin(_pinV);
    if(!_adc->dualTimeSample(_pinA,nb,&samples,5000))
    {
         Logger("Failed\n");
         return false;
    }
    nb>>=1;       
    voltage=averageMe(samples,nb)*_vcc/4095.;    
    current=averageMe(samples+1,nb)*_vcc/4095.;
    return true;
}
    
 
/**
 */
bool  BatterySensor::readVoltageCurrent( float &voltage,float &current)
{
    if(!rawRead(voltage,current)) return false;
    
    voltage=voltage*5./(float)VCalibration5v;
    
    return true;
}
// EOF
