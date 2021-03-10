
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
#include "xpt2046.h"
#include "dso_eeprom.h"
#include "myPinout.h"

#include "touchScreen.h"
#include "adc/simpleADC.h"
#include "math.h"
#include "adcPoll.h"
#define PIN PA1
#define PIN2 PA2

int angle[256];

void joyTest(TFT_eSPI_stm32duino *tft)
{
    pinMode(PIN,INPUT_ANALOG);
    pinMode(PIN2,INPUT_ANALOG);
    
    AdcPoll adcPoll(1000);
    
    AdcPollClient xAxis(PIN,adcPoll);
    AdcPollClient yAxis(PIN2,adcPoll);
    
    
    for(int i=0;i<1024;i++)
    {
        float a=M_PI*(float)(i-512.)/512.; // -pi to pi
        int   d=(int)(180.*(float)(i-512.)/512.); // -pi to pi
        float f=(4095/2)*(1+sin(a));
        f/=16.;
        if(d>0) d=180-d;
        angle[(int)f]=(int)d;
    }
       
    adcPoll.start();
    while(1)
    {    
      
        int sum=xAxis.getValue()/16;
        int sum2=yAxis.getValue()/16;
        
        Logger("Value =%d value2=%d\n",sum,sum2);
        Logger("Angle =%d Angle=%d\n",angle[sum],angle[sum2]);
        

        xDelay(1000);
    }
    
}
