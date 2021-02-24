#include "gd32vf103.h"
#include "drivers.h"
#include "dac.h"
#include "pwm.h"
#include "adc.h"
#include "eclicw.h"

void mt40k_init(void (*pISR)(void)){
   eclicw_enable(CLIC_INT_TMR, 1, 1, pISR);
   // Set the timer expier (compar) value to 675 (27Mhz/40kHz).
   *( volatile uint64_t * )( TIMER_CTRL_ADDR + TIMER_MTIMECMP ) = 675;
   // Reset the timer value to zero.
   *( volatile uint64_t * )( TIMER_CTRL_ADDR + TIMER_MTIME ) = 0;
}
/*
void lp(void){
    static int x=0;

    // Be aware of possible spirous int updating mtimecmp...
    // LSW = -1; MSW = update; LSW = update, in this case safe.
    *( volatile uint64_t * )( TIMER_CTRL_ADDR + TIMER_MTIMECMP )+=675;

    // Calcylate next output value...
    DAC0set(x++%2?4000:0);
    l88mem(7,x);
}
*/

void lp(void){
    static int x=0, xp=0, y=0, yp=0;
    static int g=0b0010001100000000; //0.1367287359(<<16)
    static int p=0b1011100111111110; //0.7265425280(<<16)

    // Be aware of possible spirous int updating mtimecmp...
    // LSW = -1; MSW = update; LSW = update, in this case safe.
    *( volatile uint64_t * )( TIMER_CTRL_ADDR + TIMER_MTIMECMP )+=675;

    // Calcylate next output value...
    xp=x; yp=y;
    x = ((adc_regular_data_read(ADC0)-2048))*g;
    y = (x+xp)+(int)(((int64_t)yp*(int64_t)p)>>16);
    DAC0set((y>>16)+2048);              // ...and present it to hw-driver.

    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}
void crash (int cars);
int main(void){
    int ms=0, s=0, key, pKey=-1, c=0, idle=0, i = 0;
    int lookUpTbl[16]={1,4,7,14,2,5,8,0,3,6,9,15,10,11,12,13};
    int dac=0, speed=-100;
    int array[55]={0,0,0,0,0, 0,0,0,1,4,2,32,4,8,64, 2,128,16,32,64,32,2,4,128,64, 8,64,16,8,16,2,1,8,32,16, 18,64,32,2,0,16,64,128,2,8, 128,4,0,0,0,0,0,0,0,0};
    int adcr, tmpr;
    int stop = 0, cars = 0;

    t5omsi();                               // Initialize timer5 1kHz
    colinit();                              // Initialize column toolbox
    l88init();                              // Initialize 8*8 led toolbox
    keyinit();                              // Initialize keyboard toolbox
    //DAC0powerUpInit();                      // Initialize DAC0/PA4 toolbox
    //T1powerUpInitPWM(0x4);                  // Timer #1, Ch #2 PWM
    //T1powerUpInitPWM(0xC);                  // Timer #1, Ch #2 & 3 PWM
    ADC3powerUpInit(1);                     // Initialize ADC0, Ch3 & Ch16
    //mt40k_init(&lp);                        // Initialize mtime 40kHz

    //eclic_global_interrupt_enable();        // !!!!! Enable Interrupt !!!!!

    while (1) {
        idle++;                             // Manage Async events

        if (adc_flag_get(ADC0,ADC_FLAG_EOC)==SET) { // ...ADC done?
          if (adc_flag_get(ADC0,ADC_FLAG_EOIC)==SET) { //...ch3 or ch16?
            tmpr = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);
            //l88mem(6,((0x680-tmpr)/5)+25);
            adc_flag_clear(ADC0, ADC_FLAG_EOC);
            adc_flag_clear(ADC0, ADC_FLAG_EOIC);
          }           
          else 
          {
            adcr = adc_regular_data_read(ADC0); // ......get data
            if(adcr <= 512)
            {
              adcr = 1;
            }
            if (adcr <= 1024 && adcr > 512)
            {
              adcr = 2;
            }
            if (adcr <= 1536 && adcr > 1024)
            {
              adcr = 4;
            }
            if (adcr <= 2048 && adcr > 1536)
            {
              adcr = 8;
            }
            if (adcr <= 2560 && adcr > 2048)
            {
              adcr = 16;
            }
            if (adcr <= 3072 && adcr > 2560)
            {
              adcr = 32;
            }
            if (adcr <= 3584 && adcr > 3072)
            {
              adcr = 64;
            }
            if (adcr <= 4096 && adcr > 3584)
            {
              adcr = 128;
            }
            
            if(stop == 0)
            {
              l88mem(6,(adcr + array[i]));                  // ......move data
            } 
            if(array[i] == adcr ) // crash
              {
                  l88mem(0,cars-8); 
                  l88mem(1,0x00); 
                  l88mem(2,0xc3);
                  l88mem(3,0x66);
                  l88mem(4,0x3c);
                  l88mem(5,0x3c);
                  l88mem(6,0x66);
                  l88mem(7,0xc3); 
                  stop = 1;
              }                  
            adc_flag_clear(ADC0, ADC_FLAG_EOC); // ......clear IF
          }
        }

        if (t5expq()) {                     // Manage periodic tasks
            l88row(colset());               // ...8*8LED and Keyboard
            ms++;                           // ...One second heart beat
            if (ms==1000-(i*10)){
              ms=0;
            
            if(stop == 0)
            {
              l88mem(0, array[i+7]);
              l88mem(1, array[i+6]);
              l88mem(2, array[i+5]);
              l88mem(3, array[i+4]);
              l88mem(4, array[i+3]);
              l88mem(5, array[i+2]);
              //l88mem(6, array[i+1]);
              l88mem(7, array[i]);
              i++;
              cars++;  
             if(i >= 48) // reset
              {
                i = 0; 
                cars = cars + i - 16; 
              }
            }
            }
              T1setPWMmotorB(speed+=10);    // PWM T1/C23 Unit test
              if (speed>=100) speed=-100;
            }
                        if ((key=keyscan())>=0) {       // ...Any key pressed?
              if (pKey==key) c++; else {c=0; pKey=key;}
              //l88mem(1,lookUpTbl[key]+(c<<4));
            }
            //l88mem(2,idle>>8);              // ...Performance monitor
            //l88mem(3,idle); idle=0;
            //DAC0set(dac++);                   // DAC0 Unit test
            //T1setPWMch2(15000);               // PWM T1/C2 Unit test
            adc_software_trigger_enable(ADC0, //Trigger another ADC conversion!
                                        ADC_REGULAR_CHANNEL);
        }
}

           
