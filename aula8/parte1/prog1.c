#include <detpic32.h>

int main(void)
{
    // Configure Timer T3 (2Hz with interrupts disable)
    T3CONbits.TCKPS = 7;        // 1:256 prescaler (i.e. fout_presc = 625 KHz)
    PR3 = 39062;                // Fout = 20MHz / (256 * (x + 1)) =  2 Hz
    TMR3 = 0;                   // Clear timer T2 count register
    T3CONbits.TON = 1;          // Enable timer T2  (must be the last command of the timer configuration sequence)

    while(1)
    {
        while(IFS0bits.T3IF = 0);
        IFS0bits.T3IF = 0;
        putChar('.');
    }
    return 0;
}
