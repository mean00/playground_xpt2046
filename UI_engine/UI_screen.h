#pragma once
#include <vector>
class UI_Widget;
/**
 * 
 */
class UI_Screen
{
public:
                UI_Screen();
virtual         ~UI_Screen();
        void    addWidget(UI_Widget *widget);
virtual void    process();
    
protected:    
    std::vector <UI_Widget *>_widgets;
};
// EOF