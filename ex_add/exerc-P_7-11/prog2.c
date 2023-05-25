#include <detpic32.h>

volatile int counter = 0;
volatile char tecla = 0;

unsigned char toBCD(int cenas)
{
    return (((cenas / 10) << 4) + (cenas % 10));
}

void send2displays(unsigned char value)
{
    static const char display[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;

    if(displayFlag == 0)
    {
        LATD = (LATD & 0xFF9F) | 0x0020;
        LATB = (LATB & 0x80FF) | (display[dl] << 8);
    } else {
        LATD = (LATD & 0xFF9F) | 0x0040;
        LATB = (LATB & 0x80FF) | (display[dh] << 8); 
    }

    displayFlag = !displayFlag;
}

void configureAll()
{
    T1CONbits.TCKPS = 2; // 1:32
    PR1 = 62499;
    TMR1 = 0;
    T1CONbits.TON = 1;

    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 2;

    T2CONbits.TCKPS = 5; // 1:32
    PR2 = 12499;
    TMR2 = 0;
    T2CONbits.TON = 1;

    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    IPC2bits.T2IP = 3;

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = TRISB & 0x80FF;
}

void _int_(4) irs_T1(void)
{
    switch(tecla)
    {
        case '0':
                counter += 2 * (1 + 0);
                break;
        case '1':
                counter += 2 * (1 + 1);
                break;
        case '2':
                counter += 2 * (1 + 2);
                break;
        case '3':
                counter += 2 * (1 + 3);
                break;
        case '4':
                counter += 2 * (1 + 4);
                break;
        default:
                counter++;
                break;
    }

    if(counter >= 100)
    {
        counter = 0;
    }

    printInt(counter, 16 | 2 << 16);
    putChar('\r');

    IFS0bits.T1IF = 0;
}

void _int_(8) irs_T2(void)
{
    send2displays(toBCD(counter));
    IFS0bits.T2IF = 0;
}


int main(void)
{
    configureAll();
    EnableInterrupts();
    while(1)
    {
        tecla = inkey();

    }
    return 0;
}


// #include <detpic32.h>

// #define VT1 4
// #define VT2 8



// volatile int count = 0;

// unsigned char toBCD(int counter){

//     return (((counter / 10) << 4) + (counter % 10)); 

// }


// void send2Displays(unsigned char choice){

//     static const char  numb[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

//     static int flag = 0;

//     if (flag == 0){

//         LATD = (LATD & 0xFF9F) | 0x0020;
//         LATB = (LATB & 0x80FF) | (numb[choice & 0x0F] << 8);


//     }else{
//         LATD = (LATD & 0xFF9F) | 0x0040;
//         LATB = (LATB & 0x80FF) | (numb[choice >> 4] << 8);
//     }

//     flag = !flag;

// }





// void _int_(VT1) isr_T1(void)
// {

//     if(count++ >= 100){
//         count = 0;
//     }
//     printInt(count, 0x0002000A); //0000 0111 1111 1111
//     putChar('\n');
//     IFS0bits.T1IF = 0;

// }


// void _int_(VT2) isr_T2(void)
// {   
//     send2Displays(toBCD(count));

//     IFS0bits.T2IF = 0;

// }



// int main()
// {

//     //Display
//     TRISD = TRISD & 0xFF9F;
//     TRISB = TRISB & 0x80FF;



//     //Timer 1
//     T1CONbits.TCKPS = 2; // 1:64 prescaler (i.e Fout_presc = 625 KHz)
//     PR1 = 31299;         // Fout = 20MHz / (64 * (62499 + 1)) = 10 Hz
//     TMR1 = 0;            // Reset timer T2 count register
//     T1CONbits.TON = 1;   // Enable timer T2 (must be the last command of the

//     IFS0bits.T1IF = 0;
//     IEC0bits.T1IE = 1;
//     IPC1bits.T1IP = 2;


//     //TIMER 2

//     T2CONbits.TCKPS = 5; // 1:64 prescaler (i.e Fout_presc = 625 KHz)
//     PR2 = 12499;         // Fout = 20MHz / (64 * (62499 + 1)) = 10 Hz
//     TMR2 = 0;            // Reset timer T2 count register
//     T2CONbits.TON = 1;   // Enable timer T2 (must be the last command of the

//     IFS0bits.T2IF = 0;
//     IEC0bits.T2IE = 1;
//     IPC2bits.T2IP = 3;

//     EnableInterrupts();

//     while (1)
//     {
//     }

//     return 0;
// }
