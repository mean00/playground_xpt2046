/**
 *  \brief Extended ILI9341 driver with
 *    * Support for adafruit "truetype" fonts
 *    * Mono bitmap draw (compressed or not)
 * 
 * @param _CS
 * @param _DC
 * @param _RST
 */
#include "TFT_eSPI_extended.h"

/**
 * \fn mySquare
 * \brief Draw a square of w*xheight size at position x,y
 * \param filler is a prefill color array
 */
int TFT_eSPI_extended::mySquare(int x, int y, int w, int xheight, uint16_t *filler)
{
    if(w+x>=width())
    {
        w=width()-x;
        if(w<=0) return 0;
    }
    if(xheight+y>=height())
    {
        xheight=height()-y;
        if(xheight<=0)
            return 0;
    }
    setWindowLocked(x,y,                  x+w-1, y+xheight);
    for(int row=0;row<xheight;row++)
    {
         pushColors(filler,w,0);
    }
    return 0;
}

/**
 * 
 * @param x
 * @param y
 * @param c
 * @param color
 * @param bg
 * @return 
 */

#define SETCOLOR(x)  for(int i=0;i<infos.maxWidth*2;i++)        column[i]=x; //bg; /
#if 0
#define SETCOLORdebug SETCOLOR
#else 
#define SETCOLORdebug(...) {}
#endif
int TFT_eSPI_extended::myDrawChar(int x, int y, unsigned char c,  int color, int bg,FontInfo &infos)
{
    c -= infos.font->first;
    GFXglyph *glyph  = infos.font->glyph+c;
    
    uint8_t *p= infos.font->bitmap+glyph->bitmapOffset;        
    int  w  = glyph->width;
    int  h  = glyph->height;
    uint16_t *column=infos.filler;    
    uint16_t *col=column;

    // prepare filler
    SETCOLOR(bg);
    SETCOLORdebug(ILI9341_BLUE);
    
    // Special case, space, it has yOffsset > 0
    if(infos.font->first+c==' ')
    {
        int adv=glyph->xAdvance;
        int top=infos.maxHeight;
         mySquare(x,y-top,
                  adv,
                  top+2,column);
         return adv;
    }
    

    // top & bottom
    int top=infos.maxHeight+glyph->yOffset;
    mySquare(x,y-infos.maxHeight,glyph->xAdvance,top,column);

    int bottom=-glyph->yOffset-h;
    mySquare(x,y-bottom,glyph->xAdvance,bottom+2,column);      

    SETCOLOR(ILI9341_GREEN);

    y+= glyph->yOffset;   
    
    int left=glyph->xOffset;
    int right=glyph->xAdvance-(w+left);
    if(right<0) right=0;
       
    int    finalColor;    
    int  bits = 0, bit = 0;
    setWindowLocked(x,y,
                  x+glyph->xAdvance-1, y+h);
    for( int line=0;line<h;line++)
    {
      //setWindowLocked(x,y+line,  x+glyph->xAdvance, y+line+1);
        col=column;
     
        // left
        for(int i=0;i<left;i++)
            *(col++)=bg;
        // mid
        for( int xcol=0;xcol<w;xcol++)
        {
            if(!bit)
            {
                bits= *p++;
                bit = 0x80;
            }            
            if(bits & bit) 
                finalColor=color;  
            else
                finalColor=bg;
            *(col++)=finalColor;            
            bit=bit>>1;
        }
        // right
        for(int i=0;i<right;i++) 
            *(col++)=bg;        
        pushColors(column,(col-column),0);
    }   
    return glyph->xAdvance;
}
#define STUBB(x) x {xAssert(0);}

STUBB( size_t  TFT_eSPI::write(uint8_t c))
//STUBB( int16_t TFT_eSPI::drawChar(unsigned short, long, long))
STUBB( void    TFT_eSPI::drawChar(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size) )
STUBB( void    TFT_eSPI::drawChar1(int32_t x, int32_t y, uint16_t c, uint32_t color, uint32_t bg, uint8_t size) )

STUBB(int16_t  TFT_eSPI::drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font))
STUBB(int16_t  TFT_eSPI::drawChar(uint16_t uniCode, int32_t x, int32_t y))

//STUBB( void TFT_eSPI::drawChar1(long, long, unsigned short, unsigned long, unsigned long, unsigned char) )
//int16_t TFT_eSPI::drawChar(short unsigned , long, long)
//{
    //xAssert(0);
//}
// EOF
