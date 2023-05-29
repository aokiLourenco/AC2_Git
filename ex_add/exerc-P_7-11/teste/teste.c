#include <detpic32.h>

#define VT1 4
#define VT3 12
#define VT2 8
#define VAD1 27
#define N 1

volatile int count = 0;
volatile int freq = 10;
unsigned char toBCD(int counter)
{

    return (((counter / 10) << 4) + (counter % 10));
}

void send2Displays(unsigned char choice)
{

    static const char numb[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    static int flag = 0;

    if (flag == 0)
    {

        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (numb[choice & 0x0F] << 8);
    }
    else
    {
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (numb[choice >> 4] << 8);
    }

    flag = !flag;
}

void _int_(VT1) isr_T1(void)
{
    count += freq;
    if (count >= 100)
    {
        count = 0;
    }

    printInt(count, 0x0002000A); // 0000 0111 1111 1111
    putChar('\n');
    IFS0bits.T1IF = 0;
}

void _int_(VT2) isr_T2(void)
{
    send2Displays(toBCD(count));

    IFS0bits.T2IF = 0;
}
void _int_(VT3) isr_T3(void)
{
    AD1CON1bits.ASAM = 1;
    IFS0bits.T3IF = 0;
}


void _int_(VAD1) isr_adc(void){

    freq = 1 + (ADC1BUF0/127);

    IFS1bits.AD1IF = 0;
}


int main()
{

    // Display
    TRISD = TRISD & 0xFF9F;
    TRISB = TRISB & 0x80FF;

    // Timer 1
    T1CONbits.TCKPS = 2; // 1:64 prescaler (i.e Fout_presc = 625 KHz)
    PR1 = 31299;         // Fout = 20MHz / (64 * (62499 + 1)) = 10 Hz
    TMR1 = 0;            // Reset timer T2 count register
    T1CONbits.TON = 1;   // Enable timer T2 (must be the last command of the

    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 2;

    // TIMER 2

    T2CONbits.TCKPS = 5; // 1:64 prescaler (i.e Fout_presc = 625 KHz)
    PR2 = 12499;         // Fout = 20MHz / (64 * (62499 + 1)) = 10 Hz
    TMR2 = 0;            // Reset timer T2 count register
    T2CONbits.TON = 1;   // Enable timer T2 (must be the last command of the

    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 3;

    T3CONbits.TCKPS = 7; // 1:64 prescaler (i.e Fout_presc = 625 KHz)
    PR3 = 19530;         // Fout = 20MHz / (64 * (62499 + 1)) = 10 Hz
    TMR3 = 0;            // Reset timer T2 count register
    T3CONbits.TON = 1;   // Enable timer T2 (must be the last command of the

    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;
    IPC3bits.T3IP = 3;


    // ADC

    TRISBbits.TRISB4 = 1;  // RBx digital output disconnected
    AD1PCFGbits.PCFG4 = 0; // RBx configured as analog input
    AD1CON1bits.SSRC = 7;  // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    // interrupt is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16;    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = N - 1; // Interrupt is generated after N samples
    // (replace N by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
                        // This must the last command of the A/D
                        // configuration sequence

    IFS1bits.AD1IF = 0;
    IEC1bits.AD1IE = 1;
    IPC6bits.AD1IP = 3;


    EnableInterrupts();

    while (1)
    {

        PR1 = (20000000 / (64 * freq)) - 1;
    }

    return 0;
}