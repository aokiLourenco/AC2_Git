#include <detpic32.h>


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
}