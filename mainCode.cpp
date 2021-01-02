/***************************************************
 Battery Tester
 * GPL v2
 * (c) mean 2018 fixounet@free.fr
 ****************************************************/

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

extern const GFXfont FreeSans24pt7b ;
extern const GFXfont FreeSans18pt7b ;
extern const GFXfont FreeSans9pt7b ;
extern void touchCalibration(XPT2046 *xpt, TFT_eSPI *tft);
extern void adcTest();

#define mySetup setup
#define myLoop loop

//#define TEST_DIS 


#define BootSequence(x,y) {Logger(x);  tft->setCursor(10, y*2);       tft->myDrawString(x);xDelay(10);}



char st[60];
/*
 */
void myLoop(void) ;
/**
 * 
 */
class MainTask : public xTask
{
public:
            MainTask() : xTask("MainTask",10,400)
            {

            }
            virtual void pressEvent(int x,int y)
            {
                
            }
            void    run(void);
            void    initTft();
protected:
            TFT_eSPI_stm32duino  *tft=NULL;
            xMutex               *spiMutex;
            simpleAdc            *adc;
            float                vcc;
            XPT2046              *xpt2046=NULL;
};

XPT2046 *globalXpt2046;
/*
 *  
 */
void MainTask::initTft()
{
    if(tft)
    {
        delete tft;    
        tft=NULL;
    }
    // Deep reset of screen
 
    spiMutex=new xMutex();
//(SPIClass &spi, xMutex *tex,int _W , int _H , int pinCS, int pinDC, int pinRst) 
    tft = new TFT_eSPI_stm32duino(SPI,spiMutex,240,320,TFT_CS,TFT_DC,TFT_RST);
    
    tft->init();  
    tft->setRotation(3);
    tft->fillScreen(ILI9341_BLACK);
        
    tft->setFontFamily(&FreeSans9pt7b,&FreeSans18pt7b,&FreeSans24pt7b);
    tft->setFontSize(TFT_eSPI::MediumFont);
    tft->setTextColor(ILI9341_WHITE,ILI9341_BLACK);
    tft->myDrawString("Hi There!");
}
/**
 * 
 */
void mySetup() 
{
  afio_cfg_debug_ports( AFIO_DEBUG_SW_ONLY); // get PB3 & PB4
  
   
  // set PWM to 0
  pinMode(PWM_PIN,OUTPUT);  
  digitalWrite(PWM_PIN,0);
  

  // switch to uart ASAP    
  Serial.end();
  Serial1.begin(115200);  //Wire.begin();
  Serial.end();
  Serial1.begin(115200);  
    
  Logger("Init"); 
  cpuID::identify();
  Logger(cpuID::getIdAsString());
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST); // Set the SPI bit order
  SPI.setDataMode(SPI_MODE0); //Set the  SPI data mode 0
//  SPI.setClockDivider (SPI_CLOCK_DIV4); // Given for 10 Mhz...
  SPI.setClockDivider (SPI_CLOCK_DIV8); // Given for 10 Mhz...
    
  // Start freeRTOS
  MainTask *mainTask=new MainTask();
  vTaskStartScheduler();        
}

/**
 * 
 */

void    MainTask::run(void)
{  
  Wire.setClock(100*1000);
  Wire.begin();
    
 // adcTest();
  
  initTft();   
  
  int scaler, ovf,cmp;
  pinMode(PWM_PIN,PWM);  
  pwmWrite(PWM_PIN,0);
  myPwm(PWM_PIN,PWM_FQ);
  
  pwmGetScaleOverFlowCompare(PWM_FQ,scaler,ovf,cmp);
  pwmFromScalerAndOverflow(PWM_PIN,scaler,ovf);
  pwmRestart(PWM_PIN);
  
   
  xpt2046=  XPT2046::spawn(SPI,TOUCH_CS,TOUCH_IRQ,2400*1000,spiMutex); // 2.4Mbits
  
  globalXpt2046=xpt2046;
  
  if(0 || ! DSOEeprom::read())
  {
        touchCalibration(xpt2046,tft);
        DSOEeprom::read();
  }
  xpt2046->setup(DSOEeprom::calibration);
  xpt2046->start();
#if 1  
  BatterySensor *batSensor=new BatterySensor(ADC_VOLT_PIN,ADC_CURRENT_PIN);
  
  testScreen *st=new testScreen(tft,xpt2046);
  st->begin();
  while(1)
  {
      st->process();
  }
#else
  while(1)
  {
      xDelay(100);
  }
#endif     
}



#if 0
 // do a dummy one to setup things
    //timeLoop(ADC_VOLT_PIN);    // OFFset    

    float vd,va,vt,vv;
    // 100 mA => 300 mv
    // =~ 1/9 VCC= 1000/9=110
    //pwmSetRatio(PWM_PIN, 128);
    // we want 160 mAmp =~ 480 mV => ratio = 145
    pwmSetRatio(PWM_PIN, 1024);
    int inc=10,target=0;
    float current,voltage;

    while(1)
    {
        //batSensor->rawRead(voltage,current);
#if 1        
        batSensor->getVoltageCurrent(voltage,current);        
#else        
        batSensor->rawRead(voltage,current);      
#endif        
        tft->setCursor(10,160);
        sprintf(st,"V:%2.2f, A:%2.2f\n",voltage,current);        
        tft->myDrawString(st);        
        xDelay(300);
    }
#endif

/**
 * 
 */
void myLoop()
{
    
}

//--EOF
