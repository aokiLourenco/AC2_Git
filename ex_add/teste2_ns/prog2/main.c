#include <detpic32.h>

#define N 2

volatile int voltage = 0;

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

char toBCD(char value)
{
    return ((value / 10) << 4) + (value % 10);
}

int voltageConversion(char VAL_AD)
{
    return (VAL_AD * 50 +511) / 1023;
}

void configureAll()
{
    //ADC
    TRISBbits.TRISB4 = 1; 
    AD1PCFGbits.PCFG4= 0; 
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = N - 1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    //T2
    T2CONbits.TCKPS = 2;   // 1:4     // 65536 * 120Hz
    PR2 = 41666;           // (20000000/(4*120)) - 1 = PR2
    TMR2 = 0; 
    T2CONbits.TON = 1; 

    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
} 

int main()
{
    configureAll();
    EnableInterrupts();
    while(1)
    {
        AD1CON1bits.ASAM == 1;
        while(IFS1bits.AD1IF == 0);
        voltage = toBCD(voltageConversion(voltage + 15));
        IFS1bits.AD1IF = 0;
        delay(1000);
    }
    return 0;
}

void _int_(8) isr_T2()
{
    send2displays(voltage);
    IFS0bits.T2IF = 0;
}

// void _int_(27) isr_ADC()
// {
//     int *p = (int*) &ADC1BUF0;
//     int i, average = 0;
//     for (i = 0; i< 2; i++)
//     {
//         average += p[i * 4];
//     }
    
//     voltage = toBCD(voltageConversion(average / 2 + 15));

//     delay(1000);
//     IFS1bits.AD1IF = 0;
// }
