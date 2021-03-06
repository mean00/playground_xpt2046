#pragma once
#include "UI_widget.h"
/**
 */
class UI_WidgetButton : public UI_Widget
{
  public:
    UI_WidgetButton(UI_Screen *screen, int posX, int posY, int w, int h, int event, const char *text) : UI_Widget(screen,posX,posY,event)
    {
            _w=w;
            _h=h;
            _txt=text;
            _screen=screen;
            _lastEvent=-1;
    }
    virtual ~UI_WidgetButton()
    {
        
    }
    virtual void draw();
    virtual void redraw();    
    virtual void press(bool longPress,int x, int y);   
    
protected:
    const char  *_txt;
    UI_Screen   *_screen;
    int         _lastEvent;
};  
// EOF
