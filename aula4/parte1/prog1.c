#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 10000000 * ms);
}

int main(void)
{
    TRISCbits.TRISC14 = 0;  // TRISC = TRISC & 0xBFFF
    LATCbits.LATC14 = 0;    // LATC = LATC & 0xBFFF
    while(1)
    {
        delay(1);   
        LATCbits.LATC14 = !LATCbits.LATC14;  // LATC = LATC14 ^ 0x4000
    }
}