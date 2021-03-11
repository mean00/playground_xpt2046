
#include "batterySensor.h"
#include "dso_debug.h"
#include "hwSettings.h"


int VCalibration4v=2008; // Voltage *1000 when 4v is applied
int ACalibration500mA=1440;
// Current
// at 0A => 88 mv
// at 500 ma => 1440


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
 BatterySensor::BatterySensor(AdcPoll *poller, int pinV, int pinA) :
     xTask("BatSense",  3, 200),_currentPoller(pinA,*poller),  _voltagePoller(pinV,*poller)
 { 
    _pinA=pinA;
    _pinV=pinV;
    _adc=poller;
    _vcc=_adc->getVcc();
    _voltage=0;
    _current=0;
 }
 /**
  * 
  */
 void BatterySensor::run()
 {
  
     while(1)
     {
        float v,a;
        if(readVoltageCurrent(v,a))
        {
            _voltage=v;
            _current=a;
        }
       // Logger("K\n");
        xDelay(100);
     }
 }
 
/**
 * 
 * @param voltage
 * @param current
 * @return 
 */
 bool  BatterySensor::readVoltageCurrent( float &voltage,float &current)
{
    voltage=((float)_voltagePoller.getValue()*_vcc)/4095.;    
    current=((float)_currentPoller.getValue()*_vcc)/4095.;
    
    voltage=voltage*4./(float)VCalibration4v;
    current=current*0.5/(float)ACalibration500mA;    
    if(current<0.05) current=0; // noise floor
    // The high side A03401 has a 80 mOhm internal resistance
    // take that into account
    voltage=voltage+current*HIGH_SIDE_RDSON/1000.;    
    return true;
}
// EOF
