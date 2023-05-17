#include <detpic32.h>

void putc(char byte)
{
    // wait while UART2 UTXBF == 1 
    while(U1STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U1TXREG = byte;
}

void puts(char *str)
{
    // use putc() function to send each charater ('\0' should not
    // be sent)
    int i = 0;
    while(str[i] != '\0')
    {
        putc(str[i++]);
    }
}

int main(void) 
{ 
    // Configure UART2 (115200, N, 8, 1)
    U1BRG = 10;   // Set Baud rate

    U1STA = 0;
    U1MODE = 0x8006;    // Enable UART for 8-bit data
                        // no parity, 1 stop bit
    U1STASET = 0x1400;   // Enable Transmit and Receive
    // config RD11 as output
    TRISDbits.TRISD11 = 0;

    while(1) 
    { 
        // Wait while TRMT == 0 
        while(U1STAbits.TRMT == 0);
        // Set RD11
        LATDbits.LATD11 = 1; 
        puts("12345"); 
        // Reset RD11
        LATDbits.LATD11 = 0;
    } 
    
    return 0; 

}

