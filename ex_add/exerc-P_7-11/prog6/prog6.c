#include <detpic32.h>

#define BAUDRATE 57600

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
    // UART2 57600,O,8,1
    U2BRG = ((PBCLK + 8 * BAUDRATE) / (16 * BAUDRATE)) - 1;

    U2MODEbits.PDSEL = 0b10;
    U2MODEbits.STSEL = 0;
    U2STAbits.UTXEN = 0;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;
    

    // Interrupts U2RX
    IPC8bits.U2IP = 2;
    IEC1bits.U2RXIE = 1;
    IFS1bits.U2RXIF = 0;

    // RB30
    TRISB = TRISB | 0x000F;
}

void send2Buffer()
{
    char buffer[] = {};
    if(c == 'D')
    {
        int i, nabo;
        for(i = 0; buffer[i] < 16; i++)
        {
            buffer[(PORTB | 0x000F) | 0xFFF0];
        }
        
        putStr("#elem: 5-");

        for(nabo = 0; nabo < 6; nabo++)
        {
            putStr(buffer);
            putc(0x44);
            putc(' ');
        }

    }
    delay(666);
}

int main()
{
    configureAll();
    while(1)
    {
        send2Buffer();
    }
    return 0;
}

void _int_(32) isr_U2()
{
    c = getc();
    IFS1bits.U2RXIF = 0;
}
