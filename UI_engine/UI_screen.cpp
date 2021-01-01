#include "UI_widget.h"
#include "UI_screen.h"
#include "dso_debug.h"

/**
 * 
 * @param tft
 * @param xpt2046
 */
UI_Screen::UI_Screen( TFT_eSPI_stm32duino  *tft,  XPT2046              *xpt2046)
{
    _tft=tft;
    _xpt2046=xpt2046;
    _xpt2046->setHooks(this);
}
/**
 * 
 */
UI_Screen::~UI_Screen()
{    
        _xpt2046->setHooks(NULL);
        int n=_widgets.size();
        for(int i=0;i<n;i++)
        {
            delete _widgets[i];                            
        }
        _widgets.clear();

}
/**
 * 
 * @param x
 * @param y
 */
void   UI_Screen::pressEvent(int x,int y)
{
    Logger("Press event : x=%d y=%d\n",x,y);
    bool dirty=false;
    int n=_widgets.size();
    for(int i=0;i<n;i++)
    {
        if(_widgets[i]->hitBox(x,y))
        {
            _widgets[i]->press(false);
            dirty=true;
        }
    }
    if(dirty)
    {
         for(int i=0;i<n;i++)        
            _widgets[i]->redraw();
    }
}
/**
 * 
 * @param widget
 */
void    UI_Screen::addWidget(UI_Widget *widget)
{
    _widgets.push_back(widget);
}
/**
 * 
 */
void    UI_Screen::process()
{
    xDelay(100);
}
void UI_Screen::begin()
{
    int n=_widgets.size();
    for(int i=0;i<n;i++)
        _widgets[i]->draw();
}
// EOF