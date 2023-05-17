#include <detpic32.h>

void putc1(char byte);
void delay(unsigned int ms);

int main(void)
{
    // Configure UART2: 
    // 1 - Configure BaudRate Generator (registo UxBRG e bit BRGH do registo UxMODE)
    
    // 2 – Configure number of data bits, parity and number of stop bits 
    //     (see U2MODE register) 
    // 3 – Enable the trasmitter and receiver modules (see register U2STA) 
    // 4 – Enable UART2 (see register U2MODE)
    
    U1BRG = 10;   // Set Baud rate

    U1STA = 0;
    U1MODE = 0x8006;    // Enable UART for 8-bit data
                        // no parity, 1 stop bit
    U1STASET = 0x1400;   // Enable Transmit and Receive

    // Na função main() transmita, em ciclo infinito a cada 10 ms, o valor 0x5A, usando a função putc1()
    while(1)
    {
        putc1(0x5A);
        // wait 1 s
        delay(10);
    }
}

void putc1(char byte)
{
    // wait while UART2 UTXBF == 1 
    while(U2STAbits.UTXBF == 1);
    // Copy "byte" to the U2TXREG register
    U1TXREG = byte;
}

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}