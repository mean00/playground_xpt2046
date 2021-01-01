
#include "UI_widget.h"
#include "UI_screen.h"

/**
 * \brief default hitbox
 * @param x
 * @param y
 * @return 
 */
 bool UI_Widget::hitBox(int x,int y)
 {
     if(x<_posX || x>(_posX+_w)) return false;
     if(y<_posY || y>(_posY+_h)) return false;
     return true;     
 }