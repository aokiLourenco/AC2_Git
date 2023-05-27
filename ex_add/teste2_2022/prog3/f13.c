#include <detpic32.h>

#define BAUDRATE 9600

volatile char cenas = 0;

void putc(char byte)
{
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte;
}

// void getc(void)
// {
//     while(U2STAbits.URXDA == 0);
//     return U2RXREG;
// }

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
    // U2
    U2BRG = ((PBCLK + 8 * BAUDRATE) / (16 * BAUDRATE)) - 1;

    U2MODEbits.PDSEL = 1;
    U2MODEbits.STSEL = 0;

    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;

    // Interrupts
    IPC8bits.U2IP = 2;
    IFS1bits.U2RXIF = 0;
    IEC1bits.U2RXIE = 1;

    IFS1bits.U2TXIF = 0;
    IEC1bits.U2TXIE = 1;
}

void _int_(32) isr_U2(void)
{
    static int lowercaseCount = 0;

    putc(cenas);

    if(lowercaseCount >= 'a' && lowercaseCount <= 'z')
    {
        lowercaseCount++;
    }

    if(cenas == 0x0A)
    {
        // number of LOWERCASE characters received
        putstr("O numero de minusculas digitadas foi " + lowercaseCount + '\n');
        lowercaseCount = 0;
    }
}

int main(void)
{
    configureAll();
    while(1);
    return 0;
}
