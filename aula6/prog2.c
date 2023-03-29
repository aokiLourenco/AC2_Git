#include <detpic32.h> 

int main(void)
{
    TRISDbits.TRISD11 = 0;

    volatile int aux;
    
    TRISBbits.TRISB4 = 1; 
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;

    AD1CON1bits.CLRASAM = 1;

    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 1 - 1;

    AD1CHSbits.CH0SA = 4;

    AD1CON1bits.ON = 1;

    while(1)
    {
        AD1CON1bits.ASAM = 1;
        LATDbits.LATD11 = 1;
        while(IFS1bits.AD1IF == 0);
        LATDbits.LATD11 = 0;
        aux = ADC1BUF0;
        // putChar('\r');
        // printInt(ADC1BUF0, 16 | 3 << 16);
        IFS1bits.AD1IF = 0;
    }

    // Tempo de conversão 3.2 ns

    return 0;

}