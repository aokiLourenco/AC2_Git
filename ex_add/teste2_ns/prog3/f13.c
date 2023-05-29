#include <detpic32.h>

#define BAUDRATE 9600

volatile int counter = 15;

void putc(unsigned char byte)
{
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte;
}

void send2LEDs(char valor)
{
    LATE = (LATE & 0xFFE1) | (valor << 1);
}

char getc(void)
{
    while( U2STAbits.URXDA == 0);
    return U2RXREG;
}

void putstr(char *str)
{
    int i;
    for(i = 0; str[i] != '\0'; i++)
    {
        putc(str[i]);
    }
}

void configureAll()
{
    U2BRG = ((PBCLK * 8 + BAUDRATE) / (16 * BAUDRATE)) - 1;

    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 0;
    U2MODEbits.STSEL = 1;
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;

    IPC8bits.U2IP = 2;
    IEC1bits.U2RXIE = 1;
    IFS1bits.U2RXIF = 0;

    TRISE = TRISE & 0xFFE1;
}

void _int_(32) isr_U2(void)
{
    char c;
    c = getc();
    if(c == 'U')
    {
        counter++;
        if (counter > 15) {
            counter = 0 ;
        }
    } else if( (c == 'R') || (counter == 15)) {
        putstr("RESET");
        putc('\n');
        counter = 0;
    }

    IFS1bits.U2RXIF = 0;
}

int main(void)
{
    configureAll();
    EnableInterrupts();
    while(1)
    {
        send2LEDs(counter);
    }
    return 0;
}