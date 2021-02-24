#include "gd32vf103.h"
#include "drivers.h"
#include "dac.h"
#include "pwm.h"

int main(void){
    int ms=0, s=0, key, pKey=-1, c=0, idle=0, value = 0, start = 0;
    int lookUpTbl[16]={15,14,0,13,12,9,8,7,11,6,5,4,10,3,2,1}; // 1,4,7,14,2,5,8,0,3,6,9,15,10,11,12,13
    // 15 = D, 16 = #, 17 = *
    int dac=0, speed=-100;

    t5omsi();                               // Initialize timer5 1kHz
    colinit();                              // Initialize column toolbox
    l88init();                              // Initialize 8*8 led toolbox
    keyinit();                              // Initialize keyboard toolbox
    //DAC0powerUpInit();                      // Initialize DAC0/PA4 toolbox
    //T1powerUpInitPWM(0x4);                  // Timer #1, Ch #2 PWM
    T1powerUpInitPWM(0x1);                  // Timer #1, Ch #2 & 3 PWM

    while (1) {
        idle++;                             // Manage Async events
 
        if (t5expq()) 
        {                     // Manage periodic tasks
            l88row(colset());               // ...8*8LED and Keyboard
            ms++;                           // ...One second heart beat
            if (ms==1000)
                {
                ms=0;
                l88mem(0,s++);
                T1setPWMmotorB(speed+=10);          // PWM T1/C23 Unit test
                if (speed>=100) speed=-100;
                }

            if(start == 0)
            {
                T1setPWMch2(0);
                start+=1;
            }

            if ((key=keyscan())>=0) 
                {       // ...Any key pressed?
                    if (pKey==key) c++; 
                    else {c=0; pKey=key;
                }
            l88mem(1,lookUpTbl[key]+(c<<4));

            if(lookUpTbl[key]>= 0 && lookUpTbl[key] <= 9)
                {
                value *= 10;
                value += lookUpTbl[key];
                }
            if(lookUpTbl[key] == 14)
                {
                    value = 0;
                }
            
            if(lookUpTbl[key] == 13)
                {
                    value = value / 10;
                }
                
            }
            if(lookUpTbl[key]==15 && value <= 100)
                {
               
                    T1setPWMch2(value * 150);               // PWM T1/C2 Unit tes // 15000 är 100%
                    value = 0;
                }
            }
            l88mem(2,idle>>8);              // ...Performance monitor
            l88mem(3,idle); idle=0;
            //DAC0set(dac++);                 // DAC0 Unit test

        }
    }