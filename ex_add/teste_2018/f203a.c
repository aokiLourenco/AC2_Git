#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    TRISD = TRISD & 0xFF9F;   // 1111 1111 1001 1111
    TRISB = TRISB & 0x80FF;   // 1000 0000 1111 1111

    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;

    int i;

    unsigned char array[] = {0x20 /* f */, 0x01/* a*/, 0x02 /*b*/, 0x40/*g*/, 0x10/*e*/, 0x08/*d*/, 0x04/*c*/, 0x40/*g*/};

    while(1)
    {   
        i = 0;
        for(; i <8; i ++ ){
            LATB = (LATB & 0x80FF) | array[i] << 8;
            delay(250);
        }
    }

    return 0;
}