#include "touchScreen.h"
#include "UI_widget_button.h"
#include "dso_debug.h"
/**
 */
testScreen::testScreen(TFT_eSPI_stm32duino  *tft,  XPT2046              *xpt2046) : UI_Screen(tft,xpt2046)
{
    UI_WidgetButton *button1=new UI_WidgetButton(this,10,10,300,80,1,"GO!");    
    UI_WidgetButton *button2=new UI_WidgetButton(this,10,100,300,80,2,"Settings");    
    addWidget(button1);
    addWidget(button2);
}
/**
 */
testScreen::~testScreen()
{
    
}
/**
 */
void    testScreen::process()
{
    int wid,event;
    while(getEvent(wid,event))
    {
        switch(event)
        {
            case 1: Logger("Got go event\n");break;
            case 2: Logger("Got settings event\n");break;
            default: xAssert(0);break;
        }
    }
    xDelay(10);
}
/***/
    