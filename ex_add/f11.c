#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    int counter = 1;
    TRISBbits.TRISB2 = 1;  
    TRISE = TRISE & 0xFFC0;          // 1111 1111 1100 0000

    while(1)
    {
        if (PORTBbits.RB2 == 0)
        {
            delay(333);
        } else
        {
            delay(143);
        }

        LATE = (LATE & 0xFFC0) | counter;
        counter = counter << 1;
        if (counter > 8);
            counter = 1;
    }

    return 0;
}