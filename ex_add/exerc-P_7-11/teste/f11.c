#include <detpic32.h>

volatile unsigned int counter = 0;

void send2Displays(unsigned char value)
{
    static const char display[] =  {};
    static char displyFlag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh  = value >> 4;

    if(displyFlag == 0)
    {
        LATE = (LATE & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (display[dl] << 8);
    } else {
        LATE = (LATE & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (display[dh] << 8);
    }
    displyFlag = !displyFlag;
}

void configureAll()
{
    //Timer

}

int main()
{
    configureAll();
    while(1)
    {
        if(counter > 9)
        {
            counter = 0;
        }
        counter++;
    }
    return 0;
}

void _int_(8) isr_T2()
{
    send2Displays(counter);
    IFS0bits.T2IF = 0;
}