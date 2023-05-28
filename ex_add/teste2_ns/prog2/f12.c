#include <detpic32.h>

#define N 2

volatile int voltage = 0;

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays(char value)
{
    static const char display[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static int displayflag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;
    
    if(displayflag == 0)
    {
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (display[dl] << 8);
    } else {
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (display[dh] << 8);
    }

    displayflag = !displayflag;
}

int toBCD(char value)
{
    return ((value / 10) << 4) + (value % 10);
}

int voltageConversion(unsigned int value)
{
    return ((value * 50 + 511) / 1023);
}

void configureAll()
{
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4= 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = N - 1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;


    // Timer 2
    T2CONbits.TCKPS = 2;            // 1:4
    PR2 = 41666;                    // (20 MHz / 4[Prescaler 1:4] * 120[Hz]) - 1 = PR2
    TMR2 = 0;
    T2CONbits.ON = 1;

    // T2 PRIO
    IPC2bits.T2IP = 2;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;

    // ADC PRIO
    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;
    IPC6bits.AD1IP = 3;
    
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;
}

void _int_(8) isr_T2(void)
{
    send2displays(toBCD(voltage));
    IFS0bits.T2IF = 0;
}

void _int_(27) isr_ADC(void)
{
    int *p = (int*) &ADC1BUF0;
    int i, average = 0;

    for (i = 0; i < 2; i++)
    {
       average +=  p[i];
    }

    average = average / 2;

    voltage = toBCD(voltageConversion(average + 15));

    delay(100);

    AD1CON1bits.ASAM = 1;

    IFS1bits.AD1IF = 0;
}


int main(void)
{
    configureAll();
    EnableInterrupts();
    while(1)
    {
        AD1CON1bits.ASAM = 1;

        delay(100);
    }
    return 0;
}