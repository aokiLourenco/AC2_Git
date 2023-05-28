#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2LEDs(char counter)
{
    LATE = (LATE & 0xFF00) | counter;
}

void configureAll()
{
    TRISE = TRISE & 0xFF00;   // 1111 1111 0000 0000
    TRISB = TRISB | 0x000F;
}

int main(void)
{
    configureAll();
    int counter = (PORTB = PORTB & 0x000F);
    send2LEDs(counter);

    while(1)
    {
        if(counter == 0b11111111)
        {
            counter = (PORTB & 0x000F);
        }   
        counter = ((counter << 1) + 1);
        send2LEDs(counter);
        delay(1000);
    }
    return 0;
}

