// #include <detpic32.h>

// void delay(unsigned int ms)
// {
//     resetCoreTimer();
//     while(readCoreTimer() < 20000 * ms);
// }

// void send2LEDs(char counter)
// {
//     LATE = (LATE & 0xFF00) | counter;
// }

// void configureAll()
// {
//     TRISE = TRISE & 0xFF00;   // 1111 1111 0000 0000
//     TRISB = TRISB | 0x000F;
// }

// int main(void)
// {
//     configureAll();
//     int counter = (PORTB = PORTB & 0x000F);
//     send2LEDs(counter);

//     while(1)
//     {
//         if(LATE == 0b11111111)
//         {
//             send2LEDs(counter);
//             counter = (PORTB & 0x000F);
//         }   
//         counter = ((counter << 1) + 1);
//         send2LEDs(counter);
//         delay(1000);
//     }
//     return 0;
// }

#include <detpic32.h>


void delay(int ms){

    resetCoreTimer();
    while(readCoreTimer()< 20000 * ms);

}


void send2Leds(char counter){

    LATE = (LATE & 0xFF00) | counter;

}


int main()
{

    //leds
    TRISE = TRISE & 0xFF00;
    TRISB = TRISB | 0x000F;

    //counter
    int counter;
    counter = (PORTB & 0x000F);

    while (1)
    {
        if(LATE == 0b11111111){
            send2Leds(counter);
            counter = (PORTB & 0x000F);
        }
        send2Leds(counter); 
        counter = (counter << 1) + 1;
        delay(1000);
    }


    return 0;
}

