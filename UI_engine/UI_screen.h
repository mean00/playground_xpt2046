#pragma once
#include <vector>
#include "xpt2046.h"
class UI_Widget;
class TFT_eSPI_stm32duino;
/**
 * 
 */
class UI_Screen : public XPT2046Hook
{
public:
                UI_Screen( TFT_eSPI_stm32duino  *tft,  XPT2046              *xpt2046);               
virtual         ~UI_Screen();
 virtual void   pressEvent(int x,int y);
        void    addWidget(UI_Widget *widget);
virtual void    process();
virtual void    begin();
    
protected:    
    std::vector <UI_Widget *>_widgets;
    TFT_eSPI_stm32duino     *_tft;
    XPT2046                 *_xpt2046;
    
};
// EOF