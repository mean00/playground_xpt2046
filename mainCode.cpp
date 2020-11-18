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
#include "embedded_printf/printf.h"
#include "batterySensor.h"
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


#define BAT_ENABLE      PB10 // active low

#define BootSequence(x,y) {Logger(x);  tft->setCursor(10, y*2);       tft->myDrawString(x);xDelay(10);}



#define PWM_PIN PB0
#define PWM_FQ  (20*1000)
#define ADC_VOLT_PIN PA3
#define ADC_VOLT_PIN2 PA4


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
    
  digitalWrite(  BAT_ENABLE,1);
  pinMode(BAT_ENABLE,OUTPUT_OPEN_DRAIN);
  
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
    
  initTft();   
  char s[200];

  
  // Volt frac=  2./1000.;
  // Current frac=1/2.8
  
  int scaler, ovf,cmp;
  pinMode(PWM_PIN,PWM);  
  pwmWrite(PWM_PIN,1000);
  myPwm(PWM_PIN,PWM_FQ);
  
  pwmGetScaleOverFlowCompare(PWM_FQ,scaler,ovf,cmp);
  pwmFromScalerAndOverflow(PWM_PIN,scaler,ovf);
  pwmRestart(PWM_PIN);
    
  
  BatterySensor *batSensor=new BatterySensor(ADC_VOLT_PIN,ADC_VOLT_PIN2);
    
  // Connect battery
  digitalWrite(  BAT_ENABLE,0);
    
    // do a dummy one to setup things
    //timeLoop(ADC_VOLT_PIN);    // OFFset    

    float vd,va,vt,vv;
    // 100 mA => 300 mv
    // =~ 1/9 VCC= 1000/9=110
    //pwmSetRatio(PWM_PIN, 128);
    pwmSetRatio(PWM_PIN, 0);
    int inc=10,target=0;
    float current,voltage;

    while(1)
    {
        //batSensor->rawRead(voltage,current);
        batSensor->readVoltageCurrent(voltage,current);
        
        sprintf(st,"V:%2.2f, A:%2.2f\n",voltage,current);
        Serial1.print(st);
        xDelay(3000);
    }
}

   
/**
 * 
 */
void myLoop()
{
    
}

//--EOF
