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
/**
 * 
 * @param widgetId
 * @param event
 */
void    UI_Screen::post(int widgetId, int event)
{
    uint32_t ev=(widgetId<<16)+event;
    _eventMutex.lock();
    this->_events.push_back(ev);
    _eventMutex.unlock();
}
/**
 * 
 * @param widgetId
 * @param event
 * @return 
 */
bool UI_Screen::getEvent(int &widgetId, int &event)
{
    _eventMutex.lock();
    if(!_events.size())
    {
        _eventMutex.unlock();
        return false;
    }
    uint32_t ev=_events[0];
    _events.erase(_events.begin());
    _eventMutex.unlock();
    widgetId=ev>>16;
    event=ev&0xffff;
    return true;
    
}
// EOF