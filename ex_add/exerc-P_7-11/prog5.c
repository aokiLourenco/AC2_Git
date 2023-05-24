#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void putstr(char *str)
{
    int i;
    for(i=0; str[i] = '\0'; i++)
    {
        putc(str[i]);
    }
}

void putc(char byte)
{
    while (U2STAbits.URXBF == 0);
    U2RXREG = byte;
}

void configureall()
{
    U2BRG = ((PBCLK + 8 * BAUDRATE) / (16 * BAUDRATE)) -1;

    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 0;
}

int main(void)
{
    
    EnableInterrupts();
    while(1);
    return 0;
}