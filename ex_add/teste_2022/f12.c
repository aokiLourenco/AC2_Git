#include <detpic32.h>

#define N 2

volatile unsigned char voltage = 0;  // Global variable

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms)
}

int main(void)
{
    unsigned int cnt = 0;

    TRISEbits.TRISE1 = 0;

    // Configure all (digital I/O, analog input, A/D module)
    TRISBbits.TRISB4 = 1;               // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;              // RB4 configured as analog input
    AD1CON1bits.SSRC = 7;               // Convertion trigger selection bits: in this mode an internal counter ends sampling and starts conversion

    AD1CON1bits.CLRASAM = 1;            // Stop conversions when the 1st A/D converter interrupt is generated. At the same time hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;              // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = N - 1;           // Interrupt is generated after N samples

    AD1CHSbits.CH0SA = 4;               // 4 is the desired input analog channel

    AD1CON1bits.ON = 1;                 // Enable A/D converter

    while(1)
    {
        delay(200)
        AD1CON1bits.ASAM = 1;
        while(IFS1bits.AD1IF == 0);
        LATEbits.LATE1 = 0;
        int i;
        int media = 0;
        for( i = 0; i < 2; i++ ) {
            media = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3) / 4;
        }
        send2displays(media);
        printInt(media, 16 | 3 << 16);
        putChar('\r');
        IFS1bits.AD1IF = 0;
    }

    return 0;

}

void send2displays (unsigned char value)
{
    static const char display7Scodes[] = {
                                        0x3F, //0
                                        0x06, //1
                                        0x5B, //2
                                        0x4F, //3
                                        0x66, //4
                                        0x6D, //5
                                        0x7D, //6
                                        0x07, //7
                                        0x7F, //8
                                        0x6F, //9
                                        0x77, //A
                                        0x7C, //B
                                        0x39, //C
                                        0x5E, //D
                                        0x79, //E
                                        0x71
                                        };
    unsigned char dh = value >> 4;
    unsigned char dl = value & 0x0F;

    dh = display7Scodes [dh];
    dl = display7Scodes [dl];

    static int flag = 0;

    if(flag == 0)
    {
        LATD = (LATD | 0x0040) & 0xFFDF;
        LATB = (LATB & 0x80FF) | ((unsigned int)(dh)) << 8;   
        flag = 1;
    }else
    {
        LATD = (LATD | 0x0020) & 0xFFBF;
        LATB = (LATB & 0x80FF) | ((unsigned int)(dl)) << 8;
        flag = 0;
    }
}