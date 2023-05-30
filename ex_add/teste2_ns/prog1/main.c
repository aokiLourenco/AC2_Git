#include <detpic32.h>

volatile int duty_cycle = 25;

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20 * ms);
}

void configureAll()
{
    //T2
    T2CONbits.TCKPS = 2; // 1:4 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 33332; //(20MHz / (4 * 150)) - 1 = PR2
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the

    //OC2
    OC2CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC2CONbits.OCTSEL = 0;// Use timer T2 as the time base for PWM generation
    OC2RS = ((PR2 + 1) * duty_cycle) / 100; // ((33332 + 1) * duty_cycle) / 100
    OC2CONbits.ON = 1; // Enable OC1 module 

    //DS4 DS1
    TRISB = TRISB | 0x0009;
}

int main()
{
    configureAll();
    while(1)
    {
        if(((PORTB & 0x0009) == 0x0001)) //RB0-1
        {
            duty_cycle = 25;
        } else if(((PORTB & 0x0009) == 0x0008)) {
            duty_cycle = 70;
        } else {
            duty_cycle;
        }
        delay(250);
    }
    return 0;
}