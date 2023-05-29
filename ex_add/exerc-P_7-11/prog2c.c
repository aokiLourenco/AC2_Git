#include <detpic32.h>

#define N 2

volatile int counter = 0;
volatile int tecla = 0;
volatile int freq = 0;

unsigned char toBCD(int cenas)
{
    return (((cenas / 10) << 4) + (cenas % 10));
}

void send2displays(unsigned char value)
{
    static const char display[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
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

void configureAll()
{
    TRISBbits.TRISB14 = 1;
    AD1PCFGbits.PCFG4= 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = N-1;
    AD1CHSbits.CH0SA = 4;
    AD1CON1bits.ON = 1; 

    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;
    IPC6bits.AD1IP = 4;

    T1CONbits.TCKPS = 2; // 1:32
    PR1 = 62499;
    TMR1 = 0;
    T1CONbits.TON = 1;

    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 2;

    T2CONbits.TCKPS = 5; // 1:32
    PR2 = 12499;
    TMR2 = 0;
    T2CONbits.TON = 1;

    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 3;

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = TRISB & 0x80FF;
}

void command(unsigned int tecla)
{
    switch(tecla)
    {
        case '0':
                freq = 2 * (1 + 0);
                printf("Nova Frequência: ");
                printInt(freq, 10 | 2 << 4);
                putChar('\n');
                break;
        case '1':
                freq = 2 * (1 + 1);
                printf("Nova Frequência: ");
                printInt(freq, 10 | 2 << 4);
                putChar('\n');
                
                break;
        case '2':
                freq = 2 * (1 + 2);
                printf("Nova Frequência: ");
                printInt(freq, 10 | 2 << 4);
                putChar('\n');
                
                break;
        case '3':
                freq = 2 * (1 + 3);
                printf("Nova Frequência: ");
                printInt(freq, 10 | 2 << 4);
                putChar('\n');
                
                break;
        case '4':
                freq = 2 * (1 + 4);
                printf("Nova Frequência: ");
                printInt(freq, 10 | 2 << 4);
                putChar('\n');
                
                break;
        default:
                freq = 1;
                break;
    }
}

void _int_(4) irs_T1(void)
{
    counter += freq;

    if(counter >= 100)
    {
        counter = 0;
    }

    printInt(counter, 16 | 2 << 16);
    putChar('\r');

    IFS0bits.T1IF = 0;
}

void _int_(8) irs_T2(void)
{
    send2displays(toBCD(counter));
    IFS0bits.T2IF = 0;
}


int main(void)
{
    configureAll();
    EnableInterrupts();
    while(1)
    {
        tecla = inkey();
        if(tecla != '\0')
        {
            command(tecla);
        }

    }
    return 0;
}