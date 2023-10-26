#include <detpic32.h>

void setPWM(int duty_cycle)
{
    if(duty_cycle >= 0 && duty_cycle <= 100)
    {
        OC2RS = ((PR2 + 1) * duty_cycle) / 100;
    }
}

void configureAll()
{
    //Timer

    //OC3

    //ADC
}

int main()
{
    configureAll();
    while(1)
    {

    }
    return 0;
}