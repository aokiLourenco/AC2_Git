#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    TRISB = TRISB & 0x000F;  // 1111 1111 1111 1111
    
    while(1)
    {
        delay(500);
        int c = PORTB & 0x000F;
        printInt(c, 2 | 4 << 16);
    }

    return 0;
}