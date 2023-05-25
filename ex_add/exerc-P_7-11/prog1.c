#include <detpic32.h>

#define BAUDRATE 115200

volatile static int displayValue = -1;
volatile char byte;

void configureall()
{
    U2BRG = ((PBCLK + 8 * BAUDRATE) / (16 * BAUDRATE)) -1;
    
    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 0;
    U2MODEbits.STSEL = 0;

    U2STAbits.UTXEN = 0;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;

    IFS1bits.U2RXIF =0;
    IEC1bits.U2RXIE = 1;
    IPC8bits.U2IP = 2;

    TRISE = TRISE & 0xFFF0;
    LATE = LATE & 0xFFF0;

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = TRISB & 0x80FF;

    T2CONbits.TCKPS = 2;     // k = 4
    PR2 = 49999;
    TMR2 = 0;
    T2CONbits.TON = 1;
    IPC2bits.T2IP = 2;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;



    IFS1bits.U2TXIF = 0;

}

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays(char value)
{
    displayValue = value;
    static const char display[]  = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;

    if(displayFlag == 0)
    {
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (display[dl] << 8); 
    } else{
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (display[dh] << 8);
    }
    displayFlag = !displayFlag; 
}

void _int_(8) isr_T2(void)
{
    if(displayValue != -1)
    {
        send2displays(displayValue);
    } else {
        LATD = LATD & 0xFF9F;
    }

    IFS0bits.T2IF = 0;
}

void _int_(32) uart2(void)
{
    if(IFS1bits.U2RXIF == 1)
    {
        byte = U2RXREG;
        if(byte == '0')
        {
            LATE = (LATE & 0xFFF0) | 0x0001;
            displayValue = 0x00;
        } else if(byte == '1'){
            LATE = (LATE & 0xFFF0) | 0x0002;
            displayValue = 0x01;
        } else if ( byte == '2'){
            LATE = (LATE & 0xFFF0) | 0x0004;
            displayValue = 0x02;
        } else if (byte == '3') {
            LATE = (LATE & 0xFFF0) | 0x0008;
            displayValue = 0x03;
        } else {
            displayValue = 0xFF;
            LATE = (LATE & 0xFFF0) | 0x000F;
            displayValue = 0xFF;
            delay(1000);
            LATE = LATE & 0xFFF0;
            displayValue = -1;
        }

        IFS1bits.U2RXIF = 0;

    }
}

int main(void)
{
    configureall();
    EnableInterrupts();
    while(1);
    return 0;
}
