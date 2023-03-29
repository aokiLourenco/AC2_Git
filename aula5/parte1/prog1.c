#include <detpic32.h>

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays (unsigned char value)
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
                                        0x71
                                        };
    unsigned char dh = value >> 4;
    unsigned char dl = value & 0x0F;

    dh = display7Scodes [dh];
    dl = display7Scodes [dl];

    static int flag = 0;

    if(flag == 0)
    {
        LATD = (LATD | 0x0040) & 0xFFDF;
        LATB = (LATB & 0x80FF) | ((unsigned int)(dh)) << 8;   
        flag = 1;
    }else
    {
        LATD = (LATD | 0x0020) & 0xFFBF;
        LATB = (LATB & 0x80FF) | ((unsigned int)(dl)) << 8;
        flag = 0;
    }

     

}



int main(void)
{
    TRISB = TRISB & 0x80FF;
    TRISD = TRISD & 0xFF9F;  // 1111 1111 1101 1111

    unsigned char cont = 0;
    int i = 0;

    while(1)
    {
        i = 0;
        do
        {
            cont = cont & 0xFF;
            send2displays(cont);
            delay(10);
        } while (++i < 100);
        cont++;
    }
    return 0;
}