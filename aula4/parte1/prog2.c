#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

int main(void)
{
    int counter = 0;
    TRISE = TRISE & 0xFF87;
    while (1)
    {
        LATE = (LATE & 0xFF87) | counter << 3;
        delay(250);
        counter++;
        if(counter >= 10)
            counter = 0;  
    }
    return 0;   
}