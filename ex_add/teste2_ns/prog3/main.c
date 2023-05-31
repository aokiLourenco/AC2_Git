#include <detpic32.h>

#define BAUDRATE 9600

volatile char c;
volatile int counter = 15;

void putc(unsigned char byte)
{
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte;
}

char getc()
{
    while(U2STAbits.URXDA == 0);
    return U2RXREG;
}

void putstr(char *str)
{
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        putc(str[i]);
    }
}

void configureAll()
{
    // UART2 9600,O,8,2
    U2BRG = ((PBCLK + 8 * BAUDRATE) / (16 * BAUDRATE)) - 1;

    U2MODEbits.PDSEL = 0b10;
    U2MODEbits.STSEL = 1;
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;

    IPC8bits.U2IP = 2;
    IEC1bits.U2RXIE = 1;
    IFS1bits.U2RXIF = 0;

    TRISE = TRISE & 0xFFE1;
    
}

void send2LED(char value)
{
    LATE = (LATE & 0xFFE1) | (value << 1);
}

int main()
{
    configureAll();
    EnableInterrupts();
    while(1)
    {
        if (c == 'U')
        {
            counter++;
            if (counter > 15)
            {
                counter = 0;
            }
            send2LED(counter);
        } else if ((c == 'R') || (counter = 15)) {
            putstr("RESET");
            putc('\n');
            counter = 0;
            send2LED(counter);
        }
    }
    return 0;
}

void _int_(32) isr_U2()
{
    c = getc();
    IFS1bits.U2RXIF = 0;
}
