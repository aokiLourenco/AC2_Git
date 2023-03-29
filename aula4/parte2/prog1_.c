#include <detpic32.h>

int main (void)
{
    TRISB = TRISB & 0x80FF;  //Bit 15 - 8
    TRISD = TRISD & 0xFF9F;
    LATDbits.LATD5 = 1; //Display L on
    LATDbits.LATD6 = 0; //Display H off
    LATB = LATB & 0x80FF;

    while (1)
    {
        char in = getChar();
        

       if (in >= 'a' && in <= 'g') in -= 0x20;  // Convert to uppercase  
       if (in >= 'A' && in <= 'G')
       {
           in -= 'A';                           // Get the index of the leter A->0, G->6
           int val = (0x100 << in);             // 0001 0000 0000
           LATB = (LATB & 0x00FF) | val;

       } else {
           LATB = (LATB & 0x00FF);              // Clear display
       }
    }
    
    return 0;
}