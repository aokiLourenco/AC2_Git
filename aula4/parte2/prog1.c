#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {
                                        0x3F, //0
                                        0x06, //1
                                        0x5B, //2
                                        0x4F, //3
                                        0x66, //4
                                        0x6D, //5
                                        0x7D, //6
                                        0x07, //7
                                        0x7F, //8
                                        0x6F, //9
                                        0x77, //A
                                        0x7C, //B
                                        0x39, //C
                                        0x5E, //D
                                        0x79, //E
                                        0x71  //F
                                        };
    unsigned char dh = value >> 4;      // Index of the decimal part
    unsigned char dl = value & 0x0F;    // Index of the unitary part
    
    // Get the correct hex code for the number
    dh = display7Scodes[dh];           
    dl = display7Scodes[dl];

    LATD = (LATD | 0x0040) & 0xFFDF;                            // Dipslay High active and Display Low OFF
    LATB = (LATB & 0x80FF) | ((unsigned int)(dh)) << 8;         // Clean the display and set the right value
    delay(20);                                                  // Debug (see the first display light up slightly)

    LATD = (LATD | 0x0020) & 0xFFBF;                            // Display High OFF and Display High active
    LATB = (LATB & 0x80FF) | ((unsigned int)(dl)) << 8;         // Clean the display and set the right value
}


int main(void)
{
    TRISB = TRISB & 0x80FF;     // 1000 0000 1111 1111
    TRISDbits.TRISD5 = 0;       // outputs
    TRISDbits.TRISD6 = 0;

    unsigned char cont = 0;

    while (1)
    {
       cont = cont & 0xFF;
       send2displays(cont);
       delay(200);
       cont++;
    }

}