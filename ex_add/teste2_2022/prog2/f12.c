#include <detpic32.h>

#define N 4

volatile unsigned char voltage = 0;
volatile int average = 0;

void configureAll()
{
    // ADC
    AD1PCFGbits.PCFG4 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = N - 1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1;

    // Timer 2
    T2CONbits.TCKPS = 2;            // 1:4
    PR2 = 49999;                    // (20 MHz / 4[Prescaler 1:4] * 100[Hz]) - 1 = 49999
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

    // LEDs e cenas
    TRISE = TRISE & 0xFF00; // 1111 1111 0000 0000
    TRISD = TRISD & 0xFF9F;
    TRISB = TRISB & 0x80FF;

    AD1CON1bits.ASAM = 1;
}

void send2displays(unsigned char value)
{
    static const char display [] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
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


void delay(unsigned int ms)
{
    resetCoreTimer();
    while (readCoreTimer() < 20000 * ms);
}

void LEDComportamento(char nsei)
{
    switch (nsei)
    {
        case 1:
            LATE = (LATE & 0xFF00) | 0x0001;
            break;
        case 2:
            LATE = (LATE & 0xFF00) | 0x0003;
            break;
        case 3:
            LATE = (LATE & 0xFF00) | 0x0007;
            break;
        case 4:
            LATE = (LATE & 0xFF00) | 0x000F;
            break;
        case 5:
            LATE = (LATE & 0xFF00) | 0x001F;
            break;
        case 6:
            LATE = (LATE & 0xFF00) | 0x003F;
            break;
        case 7:
            LATE = (LATE & 0xFF00) | 0x007F;
            break;
        case 8:
            LATE = (LATE & 0xFF00) | 0x00FF;
            break;
        default:
            LATE = LATE & 0xFF00;
            break;
    }
}

int toBCD(char value)
{
    return (((value / 10) << 4) + (value % 10));
}

int voltageConversion(unsigned int VAL_AD)
{
    return (VAL_AD * 16 + 511) / 1023;
}

void _int_(8) isr_T2(void)
{
    send2displays(toBCD(voltage));
    IFS0bits.T2IF = 0;
}

void _int_(27) isr_AD(void)
{
    int *pointer = (int*) &ADC1BUF0;
    int i, average = 0;

    for (i = 0; i < 8; i++)
    {
        average += pointer[i * 4];
    }
    average = average / 4;

    voltage = toBCD(voltageConversion(average));

    IFS1bits.AD1IF = 0;
}

int main(void)
{
    configureAll();
    EnableInterrupts();
    while (1)
    {
        AD1CON1bits.ASAM = 1;

        LEDComportamento(voltage);

        delay(100);
    }
    return 0;
}
