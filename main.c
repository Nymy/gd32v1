#include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"

int main(void){
    int ms=0, s=0, key, pKey=-1, c=0, idle=0;
    int X=0, Y=0, X1=0, Y1=0, R=0, K=0, keypressed=0, color=0, F = 0; 
    int lookUpTbl[16]={13,15,0,14,12,9,8,7,11,6,5,4,10,3,2,1};
    int dac=0, speed=-100;
    int adcr, tmpr;
    char digits[10][10]={"Zero ","One  ","Two  ","Three","Four ","Five ","Six  ","Seven","Eight","Nine "};

    t5omsi();                               // Initialize timer5 1kHz
    colinit();                              // Initialize column toolbox
    l88init();                              // Initialize 8*8 led toolbox
    keyinit();                              // Initialize keyboard toolbox
    Lcd_SetType(LCD_INVERTED);                // or use LCD_INVERTED!
    Lcd_Init();
    LCD_Clear(WHITE);
    //LCD_ShowStr(10, 10, "POLL VERSION", WHITE, TRANSPARENT);

    while (1) {
        idle++;                             // Manage Async events
        LCD_WR_Queue();                   // Manage LCD com queue!

        if (t5expq()) {                     // Manage periodic tasks
            l88row(colset());               // ...8*8LED and Keyboard
            ms++;                           // ...One second heart beat
            if (ms==1000){
              ms=0;
            }
            if ((key=keyscan())>=0) {       // ...Any key pressed?
              if (pKey==key) c++; else {c=0; pKey=key;}
            if(9 >= lookUpTbl[key] && lookUpTbl[key] >= 0)
            {
              if(keypressed == 0)
              {
                R = lookUpTbl[key];
                F = R;
                keypressed = 1;
              }
              else if(keypressed == 1)
              {
                K = lookUpTbl[key];
                F = F * 10;
                F = F + lookUpTbl[key];
                keypressed = 0;
              }
            }

            if(lookUpTbl[key] == 10)
            {
              X = K *16;
              Y = R * 8;
            }

            if(lookUpTbl[key] == 11)
            {
              X1 = K *16;
              Y1 = R * 8;
            }

            if(lookUpTbl[key] == 12)
            {
                switch(F)
                {
                  case 0:
                    color = BLACK;
                    break; 
                  case 1:
                    color = BLUE;
                    break;
                  case 2:
                    color = GREEN;
                    break;
                  case 3:
                    color = CYAN;
                    break;
                  case 4:
                    color = YELLOW;
                    break;
                  case 5:
                    color = GRAY;
                    break;
                  case 6:
                    color = LIGHTBLUE;
                    break;
                  case 7:
                    color = LGRAY;
                    break;
                  case 8:
                    color = LGRAYBLUE;
                    break;
                  case 9:
                    color = WHITE;
                    break;
                }
            keypressed = 0;  
            }

            if(lookUpTbl[key] == 13)
            {
            LCD_DrawLine(X,Y,X1,Y1,color);
            keypressed=0;  
            }

            if(lookUpTbl[key] == 14) 
            {
                if(X1>=X && Y1>=Y)
                {
                  LCD_Fill(X,Y,X1,Y1, color);
                }
                if(X>=X1 && Y>=Y1)
                {
                  LCD_Fill(X1,Y1,X,Y, color);
                }
                if(X1>=X && Y>=Y1)
                {
                  LCD_Fill(X,Y1,X1,Y, color);
                }
                if(X>=X1 && Y1>=Y)
                {
                  LCD_Fill(X1,Y,X,Y1, color);
                }
                keypressed=0;  
            }

            if(lookUpTbl[key] == 15)
              {
                Draw_Circle(X,Y,Y1, color);
                keypressed=0;
              }
            }

        }
    }
}