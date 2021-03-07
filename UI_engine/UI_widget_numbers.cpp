
#include "UI_widget_numbers.h"
#include "UI_screen.h"
#include "TFT_eSPI/TFT_eSPI_stm32duino.h"
#include "dso_debug.h"

#define holdoff 500

void UI_WidgetNumbers::draw()
{    
    TFT_eSPI_stm32duino *tft=_screen->getTft();   
    
    // Draw  buttons
    int pos=_posX+4;
    int high=_posY;
    int low=_posY+_h-_squareHeight;
    int width=_w/4-8;
    int step=_w/4;
    
    for(int i=0;i< _intg;i++)
    {
        tft->drawRoundRect(pos,high,width,_squareHeight,4,ILI9341_WHITE);
        tft->drawRoundRect(pos,low, width,_squareHeight,4,ILI9341_WHITE);
        pos+=step;
    }
    pos=_posX+4;
    for(int i=0;i< _decimal;i++)
    {
        tft->drawRoundRect(pos,high,width,_squareHeight,4,ILI9341_WHITE);
        tft->drawRoundRect(pos,low ,width,_squareHeight,4,ILI9341_WHITE);
        pos+=step;
    }
    redraw();    
}
UI_WidgetNumbers::UI_WidgetNumbers(UI_Screen *screen, int posX, int posY, int w, int h, int event,float & value, int intg, int decimal) : UI_Widget(screen,posX,posY,event)
    {
            _w=w;
            _h=h;
            _screen=screen;
            _value=value;
            _intg=intg;
            _decimal=decimal;
            
            TFT_eSPI_stm32duino *tft=_screen->getTft();   
    
            int tx,ty;    
            tft->getTextSize(tx,ty);                       
            _squareHeight=tx;
    }
/**
 * 
 * @param x
 * @param y
 * @return 
 */
bool UI_WidgetNumbers::hitBox(int x,int y)
 {
     if(x<_posX || x>(_posX+_w)) return false;
     if(y<_posY || y>(_posY+_h)) return false;
     // in between
     if(y>_posY+_squareHeight && y<_posY+_h-_squareHeight) return false;
     return true;     
 }

/**
 * 
 */
void UI_WidgetNumbers::redraw()
{
    
}
/**
 * 
 * @param longPress
 */
void UI_WidgetNumbers::press(bool longPress,int x, int y)
{
    uint32_t now=millis();
    x-=_posX;
    if(x<0) return;
    if(x>_w) return;
    x=(x*4)/_w;
    int line=-1;
    if(y<_posY+_squareHeight+1) line=0;
    if(y>_posY+_h-_squareHeight-1) line=1;
    if(line<0) return;
    Logger("Column : %d Line=%d\n",x,line);
        
   
}
