
#include "UI_screen.h"


class testScreen : public UI_Screen
{
public:
                testScreen(TFT_eSPI_stm32duino  *tft,  XPT2046              *xpt2046);
virtual         ~testScreen();
virtual void    process();
    
protected:    
    std::vector <UI_Widget *>_widgets;
};