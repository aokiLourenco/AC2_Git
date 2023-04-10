#include <detpic32.h>
#define N 1

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    TRISD = TRISD & 0xFF9F;   // 1111 1111 1001 1111
    TRISB = TRISB & 0x80FF;   // 1000 0000 1111 1111

    int i;
    // unsigned char array[] = {0x20 /* f */, 0x01/* a*/, 0x02 /*b*/, 0x40/*g*/, 0x10/*e*/, 0x08/*d*/, 0x04/*c*/, 0x40/*g*/};
    unsigned char array[] = {0x20 /* f */, 0x01/* a*/, 0x02 /*b*/, 0x10 , 0x08, 0x04, 0x02, 0x01, 0x20, 0x04, 0x08, 0x10};
    int temp[] = {250, 500, 750, 1000, 1250, 1500, 1750, 2000};
    int lvl;

    TRISBbits.TRISB4 = 1;               // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;              // RB4 configured as analog input
    AD1CON1bits.SSRC = 7;               // Convertion trigger selection bits: in this mode an internal counter ends sampling and starts conversion

    AD1CON1bits.CLRASAM = 1;            // Stop conversions when the 1st A/D converter interrupt is generated. At the same time hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;              // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = N - 1;           // Interrupt is generated after N samples

    AD1CHSbits.CH0SA = 4;               // 4 is the desired input analog channel

    AD1CON1bits.ON = 1;                 // Enable A/D converter

    while(1)
    {
        for(i=0; i <12; i ++ ){
            AD1CON1bits.ASAM = 1;
            while(IFS1bits.AD1IF == 0);

            if(i<=2 || i >= 9) {
                LATDbits.LATD6 = 1;
                LATDbits.LATD5 = 0;
            } else {
                LATDbits.LATD6 = 0;
                LATDbits.LATD5 = 1;
            }
            lvl = (ADC1BUF0*8)/1024;

            LATB = (LATB & 0x80FF) | array[i] << 8;
            printInt10(lvl);
            delay(temp[lvl]);
        }
        IFS1bits.AD1IF = 0;
    }

    return 0;
}