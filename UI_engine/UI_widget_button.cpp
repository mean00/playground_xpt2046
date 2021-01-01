
#include "UI_widget_button.h"
#include "UI_screen.h"
#include "TFT_eSPI/TFT_eSPI_stm32duino.h"
#include "dso_debug.h"

void UI_WidgetButton::draw()
{    
    TFT_eSPI_stm32duino *tft=_screen->getTft();
    tft-> drawRoundRect(_posX,_posY,_w,_h,10,ILI9341_WHITE);
    
}
void UI_WidgetButton::redraw()
{
    draw();
}
void UI_WidgetButton::press(bool longPress)
{
    Logger("Button %s pressed",_txt);
}
