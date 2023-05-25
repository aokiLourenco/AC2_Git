#include <detpic32.h>

volatile int value = 0;

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void confAll()
{

}

void send2displays(char value)
{
    static const char diplay[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    unsigned char dl = value >> 4;
    unsigned char dh = value & 0x0F;
}

void _int_(8) irs_T2(void)
{
    send2displays(value);

}

int main(void)
{
    while(1);
    return 0;
}