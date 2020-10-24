#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "gfxfont.h"

int main(int a,char **b)
{
  
    const GFXfont *font=&THEFONT;
    uint8_t bufferOut[100*1000];
    int totalIn=0,totalOut=0;
    for(int i=font->first; i<font->last;i++)
    {
        int r=i-font->first;
        const GFXglyph *glyph=font->glyph+r;
        
        uint8_t *out=bufferOut;
        int w=(glyph->width+7)>>3;
        for(int y=0;y<glyph->height;y++)
        {
            uint8_t *d=glyph->bitmapOffset+font->bitmap+y*w;
            int x=0;
            int rpt=0;
            
            while((x+rpt)<w-1)
            {
                if(d[x]==d[x+1]) 
                {
                    rpt++;
                    continue;
                }
                if(rpt>2)
                {
                    printf("RPT=%d\n",rpt);
                    *out++=0x5a;
                    *out++=d[x];
                    *out++=rpt;
                    x+=rpt;
                }
                else
                {
                    if(d[x]==0x5a)
                    {
                        *out++=0x5a;
                        *out++=1;
                        x++;
                    }else
                    {
                        *out++=d[x];
                        x++;
                    }
                }
                rpt=0;
            }
            // flush last one
            if(rpt>2)
            {
                printf("RPT2=%d\n",rpt);
                *out++=0x5a;
                *out++=d[x];
                *out++=rpt;
                x+=rpt;
            }
            else
            {
                if(d[x]==0x5a)
                {
                    *out++=0x5a;
                    *out++=1;
                    x++;
                }else
                {
                    *out++=d[x];
                    x++;
                }
            }
                
        }
        printf("%d -> %d\n",w*glyph->height,(int)(out-bufferOut));
        totalIn+=w*glyph->height;
        totalOut+=(int)(out-bufferOut);
        
    }
      printf("total : %d -> %d\n",totalIn,totalOut);
    return 0;
    
}