#include "gd32vf103.h"
#include "drivers.h"
#include "dac.h"
#include "pwm.h"

void four();
void three();
void two();
void one();
void zero();
void mione();
void mitwo();
void mithree();
void mifour();
void swap(int throttle);

int main(void){
    int ms=0, key, throttle = 0, value = 0;
    int lookUpTbl[16]={15,14,0,13,12,9,8,7,11,6,5,4,10,-1,0,1}; 
    // 15 = D, 16 = #, 17 = *
    // 1 = 1, 0 = 2, -1 = 3
    int dac=0, speed=-100;

    t5omsi();                               // Initialize timer5 1kHz
    colinit();                              // Initialize column toolbox
    l88init();                              // Initialize 8*8 led toolbox
    keyinit();                              // Initialize keyboard toolbox
    //DAC0powerUpInit();                      // Initialize DAC0/PA4 toolbox
    //T1powerUpInitPWM(0x4);                  // Timer #1, Ch #2 PWM
    T1powerUpInitPWM(0x3);                  // Timer #1, Ch #2 & 3 PWM

while (1) {
        idle++;                                 // Manage Async events
 
        if (t5expq()) 
        {                         // Manage periodic tasks
            l88row(colset());                   // ...8*8LED and Keyboard
             ms++;                               // ...One second heart beat
            if (ms==1000)
            {
                ms=0;
                if (speed>=100) 
                speed=-100;
            } 

            if ((key=keyscan())>=0) // ...Any key pressed?
            {       
                throttle = throttle + lookUpTbl[key];
                 if (lookUpTbl[key] == 0)
                    {
                        throttle = 0;
                    }
                if (throttle == 5)
                    {
                        throttle = 4;
                    }
                if(throttle == -5)
                    {
                        throttle = -4;
                    }    
            }
            swap(throttle);
        }
    }
}

void swap(int throttle)
{
    switch (throttle)
    {
    case 4:
        four();
        break;
    case 3:
        three();
        break;            
    case 2:
        two();
        break;            
    case 1:
        one();
        break;            
    case 0:
        zero();
        break;            
    case -1:
        mione();
        break;            
    case -2:
        mitwo();
        break;            
    case -3:
        mithree();
        break;
    case -4:
        mifour();
        break;   
    }
}

void four()
{   T1setPWMmotorB(100);  
    l88mem(0,0x00);
    l88mem(1,0x0c);
    l88mem(2,0x1c);
    l88mem(3,0x2c);
    l88mem(4,0x4c);
    l88mem(5,0x7e);
    l88mem(6,0x0c);
    l88mem(7,0x0c);    
}

void three()
{
    T1setPWMmotorB(75);  
    l88mem(0,0x00);
    l88mem(1,0x3c);
    l88mem(2,0x66);
    l88mem(3,0x06);
    l88mem(4,0x1c);
    l88mem(5,0x06);
    l88mem(6,0x66);
    l88mem(7,0x3c);    
}

void two()
{
    T1setPWMmotorB(50);  
    l88mem(0,0x00);
    l88mem(1,0x3c);
    l88mem(2,0x66);
    l88mem(3,0x06);
    l88mem(4,0x0c);
    l88mem(5,0x30);
    l88mem(6,0x60);
    l88mem(7,0x7e);    
}

void one()
{
    T1setPWMmotorB(25);  
    l88mem(7,0x7e);
    l88mem(6,0x18);
    l88mem(5,0x18);
    l88mem(4,0x18);
    l88mem(3,0x38);
    l88mem(2,0x18);
    l88mem(1,0x18);
    l88mem(0,0x00);
}

void zero()
{        
        T1setPWMmotorB(0);
        l88mem(0,0x00); 
        l88mem(1,0x3c); 
        l88mem(2,0x66);
        l88mem(3,0x76);
        l88mem(4,0x6e);
        l88mem(5,0x66);
        l88mem(6,0x66);
        l88mem(7,0x3c);    
}

void mione()
{   
    T1setPWMmotorB(-25);  
    l88mem(7,0x81);  
    l88mem(6,0xe7);
    l88mem(5,0xe7);
    l88mem(4,0xe7);
    l88mem(3,0xc7);  
    l88mem(2,0xe7);
    l88mem(1,0xe7);  
    l88mem(0,0xff);
}

void mitwo()
{
    T1setPWMmotorB(-50);  
    l88mem(0,0xff); 
    l88mem(1,0xc3); 
    l88mem(2,0x99);
    l88mem(3,0xf9);
    l88mem(4,0xf3);
    l88mem(5,0xcf);
    l88mem(6,0x9f);
    l88mem(7,0x81);    
}

void mithree()
{
    T1setPWMmotorB(-75);  
    l88mem(0,0xff);
    l88mem(1,0xc3);
    l88mem(2,0x99);
    l88mem(3,0xf9);
    l88mem(4,0xe3);
    l88mem(5,0xf9);
    l88mem(6,0x99);
    l88mem(7,0xc3);   
}

void mifour()
{
    T1setPWMmotorB(-100);  
    l88mem(0,0xff);
    l88mem(1,0xf3);
    l88mem(2,0xe3);
    l88mem(3,0xd3);
    l88mem(4,0xb3);
    l88mem(5,0x81);
    l88mem(6,0xf3);
    l88mem(7,0xf3);    
}
  