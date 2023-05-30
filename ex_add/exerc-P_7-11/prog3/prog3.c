#include <detpic32.h>

#define N 4

volatile int voltage = 0;

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void configureAll()
{
    // Timer 3
    T3CONbits.TCKPS = 5;                // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR3 = 624;                          // Fout = (20MHz / (32 * 1000)) - 1 = PR3  // 1kHz = 1000Hz
    TMR3 = 0;                           // Reset timer T3 count register
    T3CONbits.TON = 1;                  // Enable timer T3 (must be the last command of the timer configuration sequence)
    
    // Configurar OC3 PWM
    OC3CONbits.OCM = 6;                 // PWM mode on OCx; fault pin disabled
    OC3CONbits.OCTSEL = 1;              // Use timer T3 as the time base for PWM generation
    OC3RS = 625;                      // Ton constant 
    OC3CONbits.ON = 1;                  // Enable OC3 module 

    // Configurar ADC
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4= 0;
    AD1CON1bits.SSRC = 7; 
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = N - 1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;
}

int toBCD(char value)
{
    return ((value / 10) << 4) + (value % 10);
}

int voltageConversion(unsigned int value)
{
    return (value * 100 + 511) / 1023;
}

void setPWM(unsigned int dutycycle) {
    if(dutycycle >= 0 && dutycycle <= 100) {
        OC3RS = (625 * dutycycle) / 100;  // Ton constant
    }
}

int main(void)
{
    configureAll();
    EnableInterrupts();
    while(1)
    {
        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);
        voltage = toBCD(voltageConversion(voltage));
        setPWM(voltage);
        IFS1bits.AD1IF = 0;
        delay(1000);
    }
    return 0;
}
