/***************************************************
 Battery Tester
 * GPL v2
 * (c) mean 2018 fixounet@free.fr
 ****************************************************/

#include <Wire.h>
#include "SPI.h"
#include "dso_debug.h"
#include "dso_eeprom.h"
#include "TFT_eSPI_stm32duino.h" 
#include "stopWatch.h"
#include "myPwm.h"
#include "adc.h"
#include "dso_adc.h"
#include "embedded_printf/printf.h"
extern const GFXfont FreeSans24pt7b ;
extern const GFXfont FreeSans18pt7b ;
extern const GFXfont FreeSans9pt7b ;

#define mySetup setup
#define myLoop loop

//#define TEST_DIS 

// ILI9341 is using HW SPI + those pins
#define TFT_DC          PB14
#define TFT_RST         PB13
#define TFT_CS          PB12


// TOUCH SCREEN
#define TOUCH_CS        PB5
#define TOUCH_IRQ       PB4
#define BootSequence(x,y) {Logger(x);  tft->setCursor(10, y*2);       tft->myDrawString(x);xDelay(10);}

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
            void    loop(void) ;
            void    analogLoop(int pin);
            void    dmaLoop(int pin);
protected:
            TFT_eSPI_stm32duino  *tft=NULL;
            xMutex               *spiMutex;
};


/**
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

  // switch to uart ASAP    
  Serial.end();
  Serial1.begin(115200);  //Wire.begin();
  Serial.end();
  Serial1.begin(115200);  
    
  Logger("Init"); 
  
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
 * @param pin
 */
void MainTask::dmaLoop(int pin)
{
    pinMode(pin,INPUT_ANALOG);
    
    analogRead(pin);
    
    DSOADC adc(pin);    
    adc.setupADCs ();
  
    float vcc=adc.readVCCmv();
    float volt;
    char st[60];
    while(1)
    {
        adc.clearSamples();
        adc.setADCPin(pin);
#if 0
        adc.prepareTimerSampling (2000,false,ADC_SMPR_28_5 , DSOADC::ADC_PRESCALER_4);
        adc.startTimerSampling(16);
#else        
        adc.prepareDMASampling(ADC_SMPR_28_5, DSOADC::ADC_PRESCALER_4);
        adc.startDMASampling(16);
#endif
        uint16_t *samples;
        int nb;

        if(adc.getSamples(&samples,nb))
        {
          int sum=0;
          for(int i=0;i<nb;i++) sum+=samples[i];
          sum/=nb;        
          volt=((float)sum)*vcc/4095.;
          volt*=2./1000.;
           tft->fillScreen(ILI9341_BLACK);
           sprintf(st,"%2.2f v",volt);
           tft->setCursor(100,100);
           tft->myDrawString(st);        
        }
        xDelay(1000);
    }
}
/**
 * 
 * @param pin
 */
void MainTask::analogLoop(int pin)
{
    pinMode(pin,INPUT_ANALOG);
    char st[60];
    float volt;  
    float vcc=3.300;
    while(1)
    {
           float f=analogRead(pin);
           volt=2*f*vcc/4095.;
           tft->fillScreen(ILI9341_BLACK);
           sprintf(st,"%2.2f v",volt);
           tft->setCursor(100,100);
           tft->myDrawString(st);        
           xDelay(1000);
    }  
}
/**
 * 
 */

#define PWM_PIN PA1
#define ADC_VOLT_PIN PA3
void    MainTask::run(void)
{  
  Wire.setClock(100*1000);
  Wire.begin();
    
  initTft();   
  char s[200];

#if 0  
  int scaler, ovf,cmp;
  pinMode(PWM_PIN,PWM);  
  pwmWrite(PWM_PIN,1000);
#endif  
  
  int scaler, ovf,cmp;
  pinMode(PWM_PIN,PWM);  
  pwmWrite(PWM_PIN,1000);
  myPwm(PWM_PIN,10000);
  
  pwmGetScaleOverFlowCompare(10*1000,scaler,ovf,cmp);
  pwmFromScalerAndOverflow(PWM_PIN,scaler,ovf);
  pwmRestart(PWM_PIN);
    
#if 0  
  analogLoop(ADC_VOLT_PIN);
#else
  dmaLoop(ADC_VOLT_PIN);
#endif
  
  
}
     
#if 0      
    Serial1.print("half\n");
    pwmSetRatio(PWM_PIN, 512);
    WAIT;  
  
    Serial1.print("25%\n");
    pwmSetRatio(PWM_PIN, 256);
    WAIT;  

    Serial1.print("75%\n");
    pwmSetRatio(PWM_PIN, 3*256);
    WAIT;  
#endif

   
/**
 * 
 */
void MainTask::loop(void) 
{
       xDelay(50);
}
/**
 * 
 */
void myLoop()
{
    
}

//--EOF
