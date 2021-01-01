#include "touchScreen.h"
#include "UI_widget_button.h"
/**
 */
testScreen::testScreen(TFT_eSPI_stm32duino  *tft,  XPT2046              *xpt2046) : UI_Screen(tft,xpt2046)
{
    UI_WidgetButton *button1=new UI_WidgetButton(this,10,10,300,80,1,"THIS IS A TEXT");
    
    addWidget(button1);
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
    
}
/***/
    