#include <detpic32.h>
// #define BAUDRATE 115200

// void delay(unsigned int ms)
// {
//     resetCoreTimer();
//     while(readCoreTimer() < 20000 * ms);
// }

// void send2displays(char value)
// {
//     static const char display7Scodes[] = {0x3F, //0
//                                           0x06, //1
//                                           0x5B, //2
//                                           0x4F, //3
//                                           0x66, //4
//                                           0x6D, //5
//                                           0x7D, //6
//                                           0x07, //7
//                                           0x7F, //8
//                                           0x6F, //9
//                                           0x77, //A
//                                           0x7C, //b
//                                           0x39, //C
//                                           0x5E, //d
//                                           0x79, //E
//                                           0x71  //F
//                                          };
//     static char displayFlag = 0;
//     unsigned char digit_low = value & 0x0F;
//     unsigned char digit_high = value >> 4;
//     if(displayFlag == 0)
//     {
//         LATDbits.LATD5 = 1;
//         LATDbits.LATD6 = 0;
//         LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
//     }
//     else
//     {
//         LATDbits.LATD5 = 0;
//         LATDbits.LATD6 = 1;
//         LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
//     }
//     displayFlag = !displayFlag;
// }

// void getc(char byte)
// {
//     while(U2STAbits.URXDA == 1);
//     U2RXREG = byte;
// }

// void configureall(void)
// {

//     T2CONbits.TCKPS = 2;
//     PR2 = 49999;
//     TMR2 = 0;
//     T2CONbits.TON = 1;

//     IPC2bits.T2IP = 2;
//     IEC0bits.T2IE = 1;

//     U2BRG = ((PBCLK + 8 * BAUDRATE) / (16 * BAUDRATE)) - 1;

//     U2MODEbits.BRGH = 0;
//     U2MODEbits.PDSEL = 0;
//     U2MODEbits.STSEL = 1;
    
//     U2STAbits.UTXEN = 1;
//     U2STAbits.URXEN = 1;

//     U2MODEbits.ON = 1;
    
//     TRISEbits.TRISE0 = 0;
//     TRISEbits.TRISE1 = 0;
//     TRISEbits.TRISE2 = 0;
//     TRISEbits.TRISE3 = 0;

//     TRISDbits.TRISD5 = 0;
//     TRISDbits.TRISD6 = 0;
// }

// void _int_(8) isr_T2(void)
// {
//     char c;
//     c = getc();
//     if(c == '0'){
//         LATEbits.LATE0 = 0;
//         send2displays(0x00);
//     }
//     else if(c == '1'){
//         LATEbits.LATE1 = 1;
//         send2displays(0x01);
//     }
//     else if(c == '2'){
//         LATEbits.LATE2 = 1;
//         send2displays(0x02);
//     }
//     else if(c == '3'){
//         LATEbits.LATE3 = 1;
//         send2displays(0x03);
//     }
//     else{
//         LATE = LATE & 0x000F;  // 0000 0000 0000 1111
//         send2displays(0xFF);
//         delay(1000);
//     }

//     IFS0bits.T2IF = 0;
     
// }

// int main(void)
// {
//     configureall();
//     EnableInterrupts();
//     while(1);
//     return 0;
// }


#include <detpic32.h>

#define BAUDRATE 115200

volatile static int displayValue = -1;
volatile char byte;

void configureall()
{
    U2BRG = ((PBCLK + 8 * BAUDRATE) / (16 * BAUDRATE)) -1;
    
    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 0;
    U2MODEbits.STSEL = 0;

    U2STAbits.UTXEN = 0;
    U2STAbits.URXEN = 1;

    U2MODEbits.ON = 1;

    IFS1bits.U2RXIF =0;
    IEC1bits.U2RXIE = 1;
    IPC8bits.U2IP = 2;

    TRISE = TRISE & 0xFFF0;
    LATE = LATE & 0xFFF0;

    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISB = TRISB & 0x80FF;

    T2CONbits.TCKPS = 2;     // k = 4
    PR2 = 49999;
    TMR2 = 0;
    T2CONbits.TON = 1;
    IPC2bits.T2IP = 3;
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;



    IFS1bits.U2TXIF = 0;

}

void delay(unsigned int ms)
{
    resetCoreTimer();
    while(readCoreTimer() < 20000 * ms);
}

void send2displays(char value)
{
    displayValue = value;
    static const char display[]  = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static char displayFlag = 0;
    unsigned char dl = value & 0x0F;
    unsigned char dh = value >> 4;

    if(displayFlag == 0)
    {
        LATEbits.LATE5 = 1;
        LATEbits.LATE6 = 0;
        LATB = (LATB & 0x80FF) | (display[dl] << 8); 
    } else{
        LATEbits.LATE6 = 1;
        LATEbits.LATE5 = 0;
        LATB = (LATB & 0x80FF) | (display[dh] << 8);
    }
    displayFlag = !displayFlag; 
}

void _int_(8) isr_T2(void)
{
    if(displayValue != 1)
    {
        send2displays(displayValue);
    } else {
        LATD = LATD & 0xFF9F;
    }

    IFS0bits.T2IF = 0;
}

void _int_(32) uart2(void)
{
    if(IFS1bits.U2RXIF == 1)
    {
        byte = U2RXREG;
        if(byte == '0')
        {
            LATEbits.LATE0 = 1;
            displayValue = 0x00;
        } else if(byte == '1'){
            LATEbits.LATE1 = 1;
            displayValue = 0x01;
        } else if ( byte == '2'){
            LATEbits.LATE2 = 1;
            displayValue = 0x02;
        } else if (byte == '3') {
            LATEbits.LATE3 = 1;
            displayValue = 0x03;
        } else {
            LATE = 0x000F;
            delay(1000);
            LATE = 0xFFF0;
            displayValue = 0xFF;
        }

        IFS1bits.U2RXIF = 0;

    }
}

int main(void)
{
    configureall();
    EnableInterrupts();
    while(1);
    return 0;
}