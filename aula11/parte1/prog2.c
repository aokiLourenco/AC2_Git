#include <detpic32.h>

#define VECTOR_UART2 32

void putc1(char byte)
{
    // wait while UART2 UTXBF == 1 
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U2TXREG = byte;
}

void _int_(VECTOR_UART2) isr_uart2(void)
{


    if (/*UART2 Rx interrupt flag is set*/  IFS1bits.U2RXIF == 1)
    {
    // Read character from FIFO (U2RXREG)
    char c = U2RXREG;
    // Send the character using putc()
    putc1(c);
    // Clear UART2 Rx interrupt flag
    IFS1bits.U2RXIF = 0;
    }
}

int main(void)
{
    // Configure UART2: 115200, N, 8, 1
    // Configure UART2 interrupts, with RX interrupts enabled
    // and TX interrupts disabled:
    // enable U2RXIE, disable U2TXIE (register IEC1)
    // set UART2 priority level (register IPC8)
    // clear Interrupt Flag bit U2RXIF (register IFS1)
    // define RX interrupt mode (URXISEL bits)
    // Enable global Interrupts

    U2BRG = 10;   // Set Baud rate

    U2MODEbits.BRGH = 0; // 16x baud clock enabled
    U2MODEbits.PDSEL = 0; // 8-bit data, no parity
    U2MODEbits.STSEL = 0; // 1 stop bit

    IEC1bits.U2RXIE = 1;    // Enable RX interrupts
    IEC1bits.U2TXIE = 0;    // Disable TX interrupts

    IPC8bits.U2IP = 1;      // Configure priority of interrupts
    IFS1bits.U2RXIF = 0;    // Clear interrupt flag
    
    U2STAbits.URXISEL = 0;  // Interrupt is set when a char is received

    EnableInterrupts();

    while(1);
    return 0;
}
