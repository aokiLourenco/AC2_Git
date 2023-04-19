#include <detpic32.h>

void configureAll();

volatile int voltage = 0;       // Global variable

int main(void)
{
    configureAll();             // Function to configure all (digital I/O, analog                           
                                // input, A/D module, timers T1 and T3, interrupts)
    
    IFS0bits.T3IF = 0;          // Reset timer T3 interrupt flag
    IFS0bits.T1IF = 0;          // Reset timer T1 interrupt flag
    IFS1bits.AD1IF = 0;         // Reset A/D interrupt flag

    EnableInterrupts();

    int ds;
    while (1)
    {
        ds = PORTB & 0x03;
        if (ds == 1)
            IEC0bits.T1IE = 0;          // Disable timer T1 interrupts
        else
            IEC0bits.T1IE = 1;          // Enable timer T1 interrupts
    }    

    return 0;
}

void configureAll()
{
    // Timer3
    T3CONbits.TCKPS = 2;        // 1:4 prescaler Fout_presc = 5Mhz;
    PR3 = 49999;                // Fout = 20Mhz / 4(49999+1) = 100Hz
    TMR3 = 0;                   // Reset timer T3 count register
    T3CONbits.TON = 1;          // Enable timer T3
    // Timer1
    T1CONbits.TCKPS = 3;        // 1:256 prescaler Fout_presc = 78125;
    PR1 = 39062;                // Fout = 20Mhz / 256(19530 + 1) = 4,00005Hz
    TMR1 = 0;                   // Reset timer T1 count register
    T1CONbits.TON = 1;          // Enable timer T1 

    TRISEbits.RE1 = 0;
    TRISEbits.RE3 = 0;

    IPC1bits.T1IP = 1;          // Interrupt priority 1
    IEC0bits.T1IE = 1;          // Enable timer T1 interrupts
    IPC3bits.T3IP = 3;          // Interrupt priority 2
    IEC0bits.T3IE = 1;          // Enable timer T3 interrupts

}

void _int_(4) isr_T1(void)
{
    IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{  
    IFS0bits.T3IF = 0;
}
