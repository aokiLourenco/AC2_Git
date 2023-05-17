#include <detpic32.h>

void delay(unsigned int ms);
void putc(char byte);
void putstr(char *str);

int main(void)
{
    // Configure UART2: 
    // 1 - Configure BaudRate Generator (registo UxBRG e bit BRGH do registo UxMODE)
    
    // 2 – Configure number of data bits, parity and number of stop bits 
    //     (see U2MODE register) 
    // 3 – Enable the trasmitter and receiver modules (see register U2STA) 
    // 4 – Enable UART2 (see register U2MODE)
    
    U2BRG = 10;   // Set Baud rate

    U2STA = 0;
    U2MODE = 0x8006;    // Enable UART for 8-bit data
                        // no parity, 1 stop bit
    U2STASET = 0x1400;   // Enable Transmit and Receive

    // Configure UART2 (115200, N, 8, 1)
    while(1)
    {
        putstr("String de teste\n");
        // wait 1 s
        delay(1000);
    }

    return 0;

}

void putc(char byte)
{
    // wait while UART2 UTXBF == 1 
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U2TXREG = byte; 
}

void putstr(char *str)
{
    // use putc() function to send each charater ('\0' should not be sent)
    while (*str != '\0')
    {
        putc(*str);
        str++;
    }
}

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}
