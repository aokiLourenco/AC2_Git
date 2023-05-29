#include <detpic32.h>

#define N 4

volatile int voltage = 0;

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays(char value)
{
    static const char display[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static int displayFlag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;

    if(displayFlag == 0)
    {
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (display[dl] << 8);
    } else {
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (display[dh] << 8);
    }

    displayFlag = !displayFlag;
}

void configureAll()
{
    // Configurar ADC
    TRISBbits.TRISB4 = 1;
    AD1PCFGbits.PCFG4= 0;
    AD1CON1bits.SSRC = 7; 
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = N - 1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;
    IPC6bits.AD1IP = 1;

    // Timer 5
    T5CONbits.TCKPS = 5;                
    PR5 = 624;                          
    TMR5 = 0;                           
    T5CONbits.ON = 1;

    IFS0bits.T5IF = 0;
    IPC5bits.T5IP = 2;
    IEC0bits.T5IE = 1;

    TRISD = TRISD & 0xFF9F;
    TRISB = TRISB & 0x80FF;
    TRISC = TRISC & 0xBFFF; // 1011 1111 1111 1111
}

int toBCD(char value)
{
    return ((value / 10) << 4) + (value % 10);
}

int voltageConversion(unsigned int value)
{
    return (value * 100 + 511) / 1023;
}

void setPWM(unsigned int dutycycle) {
    if(dutycycle <= 100 && dutycycle >= 0  ) {
        OC3RS = (625 * dutycycle) / 100;  // Ton constant
    }
}


void _int_(20) isr_T5(void)
{
    send2displays(toBCD(voltage));
    IFS0bits.T5IF = 0;
}


void _int_(27) isr_ADC(void)
{
    voltage = (ADC1BUF0 * 100 + 511) / 1023;
    setPWM(voltage);
    if(voltage == 100) {
        voltage = 0x00;
        LATCbits.LATC14 = 1;
    } else {
        LATCbits.LATC14 = 0;
    }
    delay(100);
    AD1CON1bits.ASAM = 1;
    IFS1bits.AD1IF = 0;
}

int main(void)
{
    configureAll();
    EnableInterrupts();
    AD1CON1bits.ASAM = 1;
    while(1)
    {
    }
    return 0;
}
