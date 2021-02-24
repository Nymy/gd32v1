#include "gd32vf103.h"
#include "drivers.h"
#include "dac.h"
#include "pwm.h"



int main(void){
    int ms=0, s=0, key, pKey=-1, c=0, idle=0, sound = 10000;
    float tmp = 0;
    int lookUpTbl[16]={13,15,0,14,12,9,8,7,11,6,5,4,10,3,2,1};
    float wave[10]={0.2, 0.4, 0.6, 0.8, 0.9, 0.9, 0.8, 0.6, 0.4, 0.2};
    int dac=0, speed=-100;

    t5omsi();                               // Initialize timer5 1kHz
    colinit();                              // Initialize column toolbox
    l88init();                              // Initialize 8*8 led toolbox
    keyinit();                              // Initialize keyboard toolbox
    DAC0powerUpInit();                      // Initialize DAC0/PA4 toolbox
    //T1powerUpInitPWM(0x4);                  // Timer #1, Ch #2 PWM
    T1powerUpInitPWM(0xC);                  // Timer #1, Ch #2 & 3 PWM

    while (1) {
        idle++;                             // Manage Async events
 
        if (t5expq()) {                     // Manage periodic tasks
            l88row(colset());               // ...8*8LED and Keyboard
            ms++;                           // ...One second heart beat
            if (ms==1000){
              ms=0;
              l88mem(0,s++);
              T1setPWMmotorB(speed+=10);          // PWM T1/C23 Unit test
              if (speed>=100) speed=-100;
            }


            if ((key=keyscan())>=0) 
            {       // ...Any key pressed?
              if (pKey==key) c++; else {c=0; pKey=key;}
              //l88mem(1,lookUpTbl[key]+(c<<4));
                sound = 10000;

                switch (lookUpTbl[key]) // 108MHz / PSC / CAR = Hz // 2000000/Hz= setCar
                {
                case 1:
                        setCar(7634); // 262Hz
                        break;
                case 2:
                        setCar(7220); // 277Hz
                        break;
                case 3:
                        setCar(6803); // 294Hz
                        break;
                case 4:
                        setCar(6431); // 311Hz
                        break;
                case 5:
                        setCar(6061); // 330Hz
                        break;
                case 6:
                        setCar(5731); // 349Hz
                        break;
                case 7:
                        setCar(5405); //370Hz
                        break;
                case 8:
                        setCar(5102); //392Hz
                        break;
                case 9:
                        setCar(4819); //415Hz
                        break;
                case 0:
                        setCar(4545); // 440Hz
                        break;
                case 10:
                        setCar(4292); // 466Hz
                        break;
                case 11:
                        setCar(4049); // 494Hz
                        break;
                case 12:
                        setCar(3824); // 523Hz
                        break;
                case 13:
                        setCar(3407); // 587Hz
                        break;
                case 14:
                        setCar(3035); // 659Hz
                        break;
                case 15:
                        setCar(2865); //698Hz
                        break;
                
                }


            }
                l88mem(2,idle>>8);              // ...Performance monitor
                l88mem(3,idle); idle=0;
                if (ms > 9) // reset för ms
                {
                        ms = 0;
                }
                
                tmp = wave[ms]; 
                dac = tmp * (sound / 20); // sound = speed / volume

                if(sound > 0)
                {
                        sound = sound - 20;
                }
                DAC0set(dac);
        }
    }
}

    
