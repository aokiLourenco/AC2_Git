#include <detpic32.h>

#define VECTCOR 27
#define N 1

// Interrupt Handler 
void _int_(VECTOR) isr_adc(void)  // Replace VECTOR by the A/D vector number - see "PIC32 family data sheet" (pages 74-76) 
{ 
    putChar('.');
    printInt(ADC1BUF0, 16 | 3 << 16 );                        // Read conversion result (ADC1BUF0) and print it 
    AD1CON1bits.ASAM = 1;   // Start A/D conversion 
    IFS1bits.AD1IF = 0;     // Reset AD1IF flag 
}

int main(void)
{
    // Configure all (digital I/O, analog input, A/D module)

    TRISBbits.TRISB4 = 1;               // RB4 digital output disconnected
    AD1PCFGbits.PCFG4 = 0;              // RB4 configured as analog input
    AD1CON1bits.SSRC = 7;               // Convertion trigger selection bits: in this mode an internal counter ends sampling and starts conversion

    AD1CON1bits.CLRASAM = 1;            // Stop conversions when the 1st A/D converter interrupt is generated. At the same time hardware clears the ASAM bit

    AD1CON3bits.SAMC = 16;              // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = N - 1;           // Interrupt is generated after N samples

    AD1CHSbits.CH0SA = 4;               // 4 is the desired input analog channel

    AD1CON1bits.ON = 1;                 // Enable A/D converter

    IPC6bits.AD1IP = 2;                 // configure priority of A/D interrupts
    IFS1bits.AD1IF = 0;                 // clear A/D interrupt flag 
    IEC1bits.AD1IE = 1;                 // enable A/D interrupts
    EnableInterrupts();                 // Macro defined in "detpic32.h"

    AD1CON1bits.ASAM = 1;               // Start A/D conversion

    while(1);
    return 0;
}