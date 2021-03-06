
#include "UI_widget_button.h"
#include "UI_screen.h"
#include "TFT_eSPI/TFT_eSPI_stm32duino.h"
#include "dso_debug.h"

#define holdoff 500

void UI_WidgetButton::draw()
{    
    TFT_eSPI_stm32duino *tft=_screen->getTft();
    tft-> drawRoundRect(_posX,_posY,_w,_h,10,ILI9341_WHITE);
    
    int tx,ty;    
    tft->getTextSize(tx,ty);
    
    int size=tft->getStringSize(_txt);
    
    int offy=(_h-ty)/2;
    if(offy<8) offy=8;
    
    int offx=(_w-size)/2;
    if(offx<4) offx=4;
    
    tft->setCursor(offx+_posX,offy+_posY);
    tft->myDrawString(_txt);
    
    
}
/**
 * 
 */
void UI_WidgetButton::redraw()
{
    draw();
}
/**
 * 
 * @param longPress
 */
void UI_WidgetButton::press(bool longPress,int x, int y)
{
    Logger("Button %s pressed",_txt);
    uint32_t now=millis();
    if(_lastEvent==-1 || (now>(_lastEvent+holdoff)))
    {        
        _screen->post(0,this->_event);
        _lastEvent=now;
    }
}
