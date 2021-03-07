#pragma once
#include "UI_widget.h"
/**
 */
class UI_WidgetNumbers : public UI_Widget
{
  public:
    UI_WidgetNumbers(UI_Screen *screen, int posX, int posY, int w, int h, int event,float & value, int intg, int decimal) ;
    virtual ~UI_WidgetNumbers()
    {
        
    }
    virtual bool hitBox(int x,int y);
    virtual void draw();
    virtual void redraw();    
    virtual void press(bool longPress,int x, int y);   
    
protected:
    float       _value;
    int         _intg, _decimal;
    UI_Screen   *_screen;
    int         _squareHeight;
};  
// EOF
