#include <detpic32.h>

volatile int value = 0;
volatile int key = 0;

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays(char value)
{
    static const char display[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayflag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;

    if(displayflag == 0)
    {
        LATD = (LATD & 0xFF9F) | 0xFFBF;
        LATB = (LATB & 0x80FF) | (display[dl] << 8);
    } else {
        LATD = (LATD & 0xFF9F) | 0xFFDF;
        LATB = (LATB & 0x80FF) | (display[dh] << 8);
    }

    displayflag = !displayflag;
}

void configureall()
{
    // Timer 2
    T2CONbits.TCKPS2 = 2;
    PR2 = 49999;
    TMR2 = 0;
    T2CONbits.ON = 1;

    // Configure priority
    IPC2bits.T2IP = 2;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;


    // LEDs & Display
    TRISE = TRISE & 0xFFF0;
    LATE = LATE & 0xFFF0;

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = TRISB & 0x80FF;
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 0;
}

void _int_(8) irs_T2(void)
{
    send2displays(value);
    IFS0bits.T2IF = 0;
}

void configureLED(unsigned int key)
{
    switch(key)
    {
        case '0':
                LATE = (LATE & 0xFFF0) | 0x0001;
                value = 0x00;
                break;
        case '1':
                send2displays(0x01);
                LATE = (LATE & 0xFFF0) | 0x0002;
                break;
        case '2':
                send2displays(0x02);
                LATE = (LATE & 0xFFF0) | 0x0004;
                break;
        case '3':
                send2displays(0x03);
                LATE = (LATE & 0xFFF0) | 0x0008;
                break;
        default:
                send2displays(0xFF);
                LATE = (LATE & 0xFFF0) | 0x000F;
                delay(1000);
                LATE = (LATE & 0xFFF0);
                send2displays(0xFF);
                break;
    }
}

int main(void)
{   
    configureall();
    EnableInterrupts();
    while(1)
    {
        key = inkey();
        if(key != '\0')
        {
            configureLED(key);
        }
    }
    return 0;
}
