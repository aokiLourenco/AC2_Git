#include <detpic32.h>

#define BAUDRATE 9600

volatile char c;

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms); 
}

void putc(char byte)
{
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte;
}

char getc()
{
    while(U2STAbits.URXDA == 0);
    return U2RXREG;
}

void putStr(char *str)
{
    int i;
    for(i = 0; str[i] != '\0'; i++)
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
    U2STAbits.URXEN = 1;
    U2STAbits.UTXEN = 1;

    U2MODEbits.ON = 1;

    IPC8bits.U2IP = 3;
    IFS1bits.U2RXIF = 0;
    IEC1bits.U2RXIE = 1;

    TRISB = TRISB | 0x000F;
    TRISE = TRISE & 0xFFEF;  // 1110 1111
}

int main()
{
    configureAll();
    EnableInterrupts();
    while(1)
    {   
        LATE = (LATE & 0xFFEF) | 0x0010;  // 0001 0000
        if (c == 'P')
        {   
            putStr("DipSwitch=");
            putc(PORTBbits.RB3 + '0');
            putc(PORTBbits.RB2 + '0');
            putc(PORTBbits.RB1 + '0');
            putc(PORTBbits.RB0 + '0');
            putc('\n');
            delay(500);
        } else if (c == 'T') {
            LATE = (LATE & 0xFFEF) | 0x0000;
            delay(500);
        }
    }
    return 0;
}

void _int_(32) isr_U2()
{
    c = getc();
    IFS1bits.U2RXIF = 0;
}