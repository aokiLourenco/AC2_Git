#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void configureAll()
{
    TRISE = TRISE & 0xFF00;   // 1111 1111 0000 0000
    TRISB = TRISB & 0x000F;
}

int main(void)
{
    configureAll();
    int counter = 0b00000000;
    while(1)
    {
        counter = counter << 1;
        if(counter > 0b11111111)
        {
            counter = 0b00000000;
        }

        delay(1000);

        if(PORTB == (PORTB & 0x0001))  // 0000 0000 0000 1111 | 1111 1111 1111 1110
        {
            LATE = ((LATE & 0xFF00) | 0x0001) | counter;
        } else if(PORTB == (PORTB & 0x0002)) {
            LATE = ((LATE & 0xFF00) | 0x0002) | counter;
        } else if(PORTB == (PORTB & 0x0004)) {
            LATE = ((LATE & 0xFF00) | 0x0004) | counter;
        } else if(PORTB == (PORTB & 0x0008)) {
            LATE = ((LATE & 0xFF00) | 0x0008) | counter;
        }
        
        delay(1000);      
    }
    return 0;
}
