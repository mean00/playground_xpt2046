#include "touchScreen.h"
#include "UI_widget_button.h"
#include "UI_widget_numbers.h"
#include "dso_debug.h"
/**
 */
testScreen::testScreen(TFT_eSPI_stm32duino  *tft,  XPT2046              *xpt2046) : UI_Screen(tft,xpt2046)
{
    UI_WidgetButton *button1=new UI_WidgetButton(this,10,10,300,52,1,"GO!");    
    
    float v=1.2;
    //UI_Screen *screen, int posX, int posY, int w, int h, int event,float & value, int intg, int decimal) : UI_Widget(screen,posX,posY,even
    UI_WidgetNumbers *numb1=new UI_WidgetNumbers(this,10,66,
                                                      300,240-66-10,
                                                      40,v, 
                                                      4, 0);
    addWidget(button1);
    addWidget(numb1);
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
    