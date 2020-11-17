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
#include "simpleADC.h"
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


#define BAT_ENABLE      PB10 // active low

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
            void    loop(void) ;
            float   analogLoop(int pin);
            float   dmaLoop(int pin);
            float   timeLoop(int pin,float frac);
            float   dualTimeLoop(int pin1, int pin2, float &current);
            float   dualDmaLoop(int pin1, int pin2, float &current);
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
 * @param pin
 */
float MainTask::dmaLoop(int pin)
{       
    float volt;
    uint16_t *samples;
    int nb=16;
    adc->changePin(pin);
    if(adc->sample(nb,&samples,ADC_SMPR_28_5,DSOADC::ADC_PRESCALER_4))
    {
      int sum=0;
      for(int i=0;i<nb;i++) 
          sum+=samples[i];
      sum=(sum+nb/2)/nb;        
      volt=((float)sum)*vcc/4095.;
      volt*=2./1000.;

      return volt;
    }
    return -1;
}

/**
 * 
 * @param pin
 */
float MainTask::timeLoop(int pin, float frac)
{    
    float volt;    
    uint16_t *samples;
    int nb=16;
    adc->changePin(pin);
    if(adc->timeSample(nb,&samples,5000))
    {
        int sum=0;
        for(int i=0;i<nb;i++) 
            sum+=samples[i];
        sum=(sum+nb/2)/nb;        
        volt=((float)sum)*vcc/4095.;
        volt=volt*frac;
        return volt;
    }
    return -1;
}
/**
 * 
 * @param pin
 */
float MainTask::analogLoop(int pin)
{
    float volt;
    float f=0;
    for(int i=0;i<16;i++)
        f+=analogRead(pin);
    f=(f+9.)/16.;
    volt=2*f*vcc/4095.;
    volt=volt/1000.;
    return volt;
}

/**
 * 
 * @param pin
 */

float averageMe(uint16_t *data, int nb)
{
    float sum=0;
    for(int i=0;i<nb;i++)     
    {
         sum+=*data;
         data+=2;
    }
    sum=(sum+nb/2.)/(float)nb;
    return sum;
}

float MainTask::dualTimeLoop(int pin1, int pin2, float &current)
{
    float volt;    
    uint16_t *samples;
    int nb=16;    
    adc->clearSamples();
    if(!adc->dualTimeSample(pin2,nb,&samples,5000))
    {
         Logger("Failed\n");
         return -1;
    }
    nb>>=1;
       
    volt=averageMe(samples,nb)*vcc/4095.;
    volt*=2./1000.;
    current=averageMe(samples+1,nb)*vcc/4095.;
    // 0.1OOhm , op amp=*28 => divide by 2.8
    current=current/2.8;
    return volt;
}
/**
 * 
 * @param pin1
 * @param pin2
 * @param current
 * @return 
 */
float MainTask::dualDmaLoop(int pin1, int pin2, float &current)
{
    float volt;    
    uint16_t *samples;
    int nb=16;    
    adc->clearSamples();
    if(!adc->dualSample(pin2,nb,&samples,ADC_SMPR_28_5,DSOADC::ADC_PRESCALER_4))
    {
         Logger("Failed\n");
         return -1;
    }
    nb>>=1;
       
    volt=averageMe(samples,nb)*vcc/4095.;
    volt*=2./1000.;
    current=averageMe(samples+1,nb)*vcc/4095.;
    // 0.1OOhm , op amp=*28 => divide by 2.8
    current=current/2.8;
    return volt;
}
/**
 * 
 */

#define PWM_PIN PB0
#define PWM_FQ  (20*1000)
#define ADC_VOLT_PIN PA3
#define ADC_VOLT_PIN2 PA4
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
  
  // Volt frac=  2./1000.;
  // Current frac=1/2.8
  
  int scaler, ovf,cmp;
  pinMode(PWM_PIN,PWM);  
  pwmWrite(PWM_PIN,1000);
  myPwm(PWM_PIN,PWM_FQ);
  
  pwmGetScaleOverFlowCompare(PWM_FQ,scaler,ovf,cmp);
  pwmFromScalerAndOverflow(PWM_PIN,scaler,ovf);
  pwmRestart(PWM_PIN);
    
  
    pinMode(ADC_VOLT_PIN,INPUT_FLOATING);
    pinMode(ADC_VOLT_PIN2,INPUT_FLOATING);

    adc=new simpleAdc(ADC_VOLT_PIN);    
    vcc=adc->getVcc();
    float volt;
    
    // Connect battery
      digitalWrite(  BAT_ENABLE,0);
    
    // do a dummy one to setup things
    //timeLoop(ADC_VOLT_PIN);    // OFFset    
    
    float vd,va,vt,vv;
    // 100 mA => 300 mv
    // =~ 1/9 VCC= 1000/9=110
    pwmSetRatio(PWM_PIN, 128);
    int inc=10,target=0;
    float current;
#if 0    
    noInterrupts();
    while(1)
    {
        
    }
#endif    
    while(1)
    {    
#if 0        
      va=analogLoop(ADC_VOLT_PIN);   // OFFSET
      vd=dmaLoop(ADC_VOLT_PIN);     // OK
      vt=dmaLoop(ADC_VOLT_PIN);    // OFFset    
       sprintf(st,"target=%d D:%2.2f d:%2.2f DD:%2.2fv\n",target,vt,vd,va);    
#endif     
       // Base ripple = ~ 25 mv, 1.6 khz
       // 40 mv ripple at 1Khz     current=timeLoop(ADC_VOLT_PIN2,1./2.8); 
    //  vv=dualTimeLoop(ADC_VOLT_PIN,ADC_VOLT_PIN2,current);
     //    current=timeLoop(ADC_VOLT_PIN2,1./2.8); 
    //   current=dmaLoop(ADC_VOLT_PIN2);
    // ==> PEAK         current=timeLoop(ADC_VOLT_PIN2); 
    // OK               vv=timeLoop(ADC_VOLT_PIN); 
    // NOISE 100 khz    vv=dmaLoop(ADC_VOLT_PIN);current=dmaLoop(ADC_VOLT_PIN2); 
    // OK               current=dmaLoop(ADC_VOLT_PIN2);       vv=dmaLoop(ADC_VOLT_PIN);
    // OK               current=timeLoop(ADC_VOLT_PIN2);       vv=timeLoop(ADC_VOLT_PIN);
    // 100 khz noise    
    //vv=dualDmaLoop(ADC_VOLT_PIN,ADC_VOLT_PIN2,current);

    // ADC-> Real current
      //  current=(current+10.)/1.2; // in mA

    // PEAKS            vv=dualTimeLoop(ADC_VOLT_PIN,ADC_VOLT_PIN2,current);
    // PEAKS adc->changePin(ADC_VOLT_PIN2);      vv=dualTimeLoop(ADC_VOLT_PIN,ADC_VOLT_PIN,current);
     //sprintf(st,"Voltage = %f , Current=%f\n" ,vv,current);    
     //Serial1.print(st);
       
#if 1       
       
       target+=inc;
       if(target>(300-inc)) inc=-inc;
       if(target<20 && inc < 0) inc=-inc;
       pwmSetRatio(PWM_PIN, target);
       sprintf(st,"PWM=%d\n",target);
       Serial1.print("----------------------\n");
       Serial1.print(st);
#endif       
        xDelay(500);
      // vv=dualDmaLoop(ADC_VOLT_PIN,ADC_VOLT_PIN2,current);

    // ADC-> Real current
      //  current=(current+10.)/1.2; // in mA
   vv=dualTimeLoop(ADC_VOLT_PIN,ADC_VOLT_PIN2,current);

    // PEAKS            vv=dualTimeLoop(ADC_VOLT_PIN,ADC_VOLT_PIN2,current);
    // PEAKS adc->changePin(ADC_VOLT_PIN2);      vv=dualTimeLoop(ADC_VOLT_PIN,ADC_VOLT_PIN,current);
     sprintf(st,"Voltage = %f , Current=%f\n" ,vv,current);    
     Serial1.print(st);
 
#if 0      
        sprintf(st,"A:%2.2f v",va);            
        tft->setCursor(20,40);
        tft->myDrawString(st);   

        sprintf(st,"D:%2.2f v",vd);    
        tft->setCursor(20,80);
        tft->myDrawString(st);   

        sprintf(st,"T:%2.2f v",vt);    
        tft->setCursor(20,120);
        tft->myDrawString(st);   
#endif        
        xDelay(10*1000);
    }

           
           
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
