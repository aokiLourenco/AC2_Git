#include <detpic32.h>

volatile int duty_cycle = 25; 

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20 * ms);
}

void configureALL()
{
    TRISD = TRISD & 0xFFFB; // 1011
    TRISB = TRISB | 0x000F;

    T2CONbits.TCKPS = 5;                // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 4166;                         // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR2 = 0;                           // Reset timer T2 count register
    T2CONbits.TON = 1;                  // Enable timer T2 (must be the last command of the timer configuration sequence)

    OC2CONbits.OCM = 6;                 // PWM mode on OCx; fault pin disabled
    OC2CONbits.OCTSEL = 0;              // Use timer T2 as the time base for PWM generation
    OC2RS = 833;                        // Ton constant
    OC2CONbits.ON = 1;                  // Enable OC1 module
}

int dutyCycle()
{
    if((PORTB = PORTB & 0xFFF7) && (PORTB = PORTB & 0xFFFE))
    {
        duty_cycle = 25;
    } else if((PORTB = PORTB & 0xFFF7) && (PORTB = PORTB & 0xFFFE)) {
        duty_cycle = 70;       
    }
    return duty_cycle;
}

int main(void)
{
    configureALL();
    delay(250);
    while(1)
    {
        dutyCycle();
    }
    return 0;
}