#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    int counter = 0b00000001;
    TRISBbits.TRISB2 = 1;  
    TRISE = TRISE & 0xFFC0;          // 1111 1111 1100 0000

    while(1)
    {
        counter = counter << 1;
        if (counter > 0b10000000){
            counter = 0b00000001;
        }

        if (PORTBbits.RB2 == 0)
        {
            LATE = (LATE & 0xFFC0) | counter;
            delay(333);
        } else
        {
            LATE = (LATE & 0xFFC0) | counter;
            delay(143);
        }
    }

    return 0;
}
