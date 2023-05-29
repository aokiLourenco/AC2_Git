#include <detpic32.h>

#define BAUDRATE 1200

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void putc(char byte)
{
    while (U2STAbits.UTXBF == 1);
    U2TXREG = byte;
}

void putstr(char *str)
{
    int i;
    for(i=0; str[i] != '\0'; i++)
    {
        putc(str[i]);
    }
}

void configureall()
{
    U2BRG = ((PBCLK + 8 * BAUDRATE) / (16 * BAUDRATE)) -1;

    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 0;
    U2MODEbits.STSEL = 0;
    U2STAbits.URXEN = 0;
    U2STAbits.UTXEN = 1;

    U2MODEbits.ON = 1;

    TRISB = TRISB | 0x000F;

}

int main(void)
{
    configureall();
    while(1)
    {
        putstr("RB30= ");
        putc(PORTBbits.RB3 + '0');
        putc(PORTBbits.RB2 + '0');
        putc(PORTBbits.RB1 + '0');
        putc(PORTBbits.RB0 + '0');
        putc('\n');
        delay(500);
    }
    return 0;
}
