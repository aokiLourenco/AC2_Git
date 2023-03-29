#include <detpic32.h>

int main(void)
{
    TRISBbits.TRISB4 = 1;               // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;              // RB4 configured as analog input
    AD1CON1bits.SSRC = 7;               // Convertion trigger selection bits: in this mode an internal counter ends sampling and starts conversion

    AD1CON1bits.CLRASAM = 1;            // Stop conversions when the 1st A/D converter interrupt is generated. At the same time hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;              // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 2 - 1;           // Interrupt is generated after 2 samples

    AD1CHSbits.CH0SA = 4;               // 4 is the desired input analog channel

    AD1CON1bits.ON = 1;                 // Enable A/D converter

    while(1)
    {
        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);
        putChar('\r');
        printInt(ADC1BUF0, 16 | 3 << 16);
        IFS1bits.AD1IF = 0;
    }

    return 0;
}