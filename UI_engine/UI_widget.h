

#pragma once

class UI_Screen;
/**
 */
class UI_Widget
{
public:
    UI_Widget(UI_Screen *screen, int posX, int posY, int event)
    {
        _screen=screen;
        _posX=posX;
        _posY=posY;
        _event=event;        
    }
    virtual ~UI_Widget()
    {
        
    }
    virtual void draw()=0;
    virtual void redraw()=0;    
    virtual void press(bool longPress)=0;    
protected:
    UI_Screen *_screen;
    int        _posX,_posY,_event;
};

// EOF