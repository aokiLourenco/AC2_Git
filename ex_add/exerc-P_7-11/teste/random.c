#include <detpic32.h>
#define BAUDRATE 9600

volatile int voltage = 0;
volatile int duty_cycle = 0;
volatile char c;

void send2Displays(char value)
{
    static const char display[] = {};
    static char displayFlag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;

    if(displayFlag == 0)
    {
        LATE = (LATE & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (display[dl] << 8);
    } else {
        LATE = (LATE & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (display[dh] << 8);
    }

    displayFlag = !displayFlag;
}

void putc(char byte)
{
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte;
}

void puStr(char *str)
{
    int i;
    for(i = 0; str[i] != '\0'; i++)
    {
        putc(str[i]);
    }
}

int voltageConversion(char value)
{
    return((value * 8 + 511) / 1023);
}

char toBCD(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

char getc()
{
    while(U2STAbits.URXDA == 0);
    return U2RXREG;
}

void configureAll()
{
    // U2 9600,O,8,1
    U2BRG = ((PBCLK + 8 * BAUDRATE) / (16 * BAUDRATE)) - 1;

    U2MODEbits.PDSEL = 0b10;
    U2MODEbits.STSEL = 0;
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;

    IPC8bits.U2IP = 2;
    IFS1bits.U2RXIF = 0;
    IEC1bits.U2RXIE = 1;

    IFS1bits.U2TXIF = 0;
    IEC1bits.U2TXIE = 1;
    
    TRISE = TRISE & 0xFF9F;
    TRISB = TRISB & 0x80FF;

    // T2
    T2CONbits.TCKPS = 5; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 62499; // (20000000 / (32 * 10)) - 1 = PR2
    TMR2 = 0; // Reset timer T2 count register
    T2CONbits.TON = 1; // Enable timer T2 (must be the last command of the
   
    // OC1
    OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL = 0;// Use timer T2 as the time base for PWM generation
    OC1RS = 12500; // (62500 * duty_cycle) / 100
    OC1CONbits.ON = 1; // Enable OC1 module
}

void changeDC(unsigned int dutycycle)
{
    if(duty_cycle >= 0 && duty_cycle <= 100)
    {
        OC1RS = (62500 * duty_cycle) / 100; 
    }
}

int main()
{
    configureAll();
    EnableInterrupts();
    while(1)
    {
        if (c == 'u')
        {
            
        }
    }
    return 0;
}

void _int_(27) isr_ADC()
{
    int *p = (int*) &ADC1BUF0;
    int i, average = 0;

    for(i = 0; i < 8; i++)
    {
        average += p[i * 4];
    }

    voltage = toBCD(voltageConversion(average / 8));

    AD1CON1bits.ASAM = 1;

    IFS1bits.AD1IF = 0;
}

void _int_(32) isr_U2()
{
    if (IFS1bits.U2RXIF == 1)
    {
        c = getc();
        IFS1bits.U2RXIF = 0;
    }
}

void _int_(8) isr_T2()
{
    send2Displays(voltage);
    IFS0bits.T2IF = 0;
}