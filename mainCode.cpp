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
extern const GFXfont FreeSans24pt7b ;
extern const GFXfont FreeSans18pt7b ;
extern const GFXfont FreeSans9pt7b ;

#define mySetup setup
#define myLoop loop

//#define TEST_DIS 

// ILI9341 is using HW SPI + those pins
#define TFT_DC          PB0
#define TFT_RST         PB1
#define TFT_CS          PB10

#define INA219_I2C_ADR  0x40
#define MCP7245_I2C_ADR 0x60

// TOUCH SCREEN
#define TOUCH_CS        PB11
#define TOUCH_IRQ       PB8
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

    tft = new TFT_eSPI_stm32duino(SPI,spiMutex,240,320,PB10,PB0,PB1);
    
    tft->init();  
    tft->setRotation(3);
    tft->fillScreen(ILI9341_BLACK);
        
    tft->setFontFamily(&FreeSans9pt7b,&FreeSans18pt7b,&FreeSans24pt7b);
    tft->setFontSize(TFT_eSPI::MediumFont);
    tft->setTextColor(ILI9341_WHITE,ILI9341_BLACK);
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
 */

#define PWM_PIN PA1
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
    
#define WAIT xDelay(5*1000)
  
  while(1)
  {
    Serial1.print("half\n");
    pwmSetRatio(PWM_PIN, 512);
    WAIT;  
  
    Serial1.print("25%\n");
    pwmSetRatio(PWM_PIN, 256);
    WAIT;  

    Serial1.print("75%\n");
    pwmSetRatio(PWM_PIN, 3*256);
    WAIT;  

    
  
  }  
}
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
