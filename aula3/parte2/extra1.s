.equ BASE, 0xBF88        
        .equ TRISB, 0x6050
        .equ PORTB, 0x6060
        .equ TRISE, 0x6100
        .equ LATE, 0x6120


        .data
        .text
        .globl main

main:   
        lui $t0, BASE
        
        
        #Input
        lw $t1, TRISB($t0)          #READ
        ori $t1, $t1, 0x000F        #put RB0-RB3 as input
        sw $t1, TRISB($t0)          #SAVE


        #output
        lw $t1, TRISE($t0)          #READ
        andi $t1, $t1, 0xFFF0       #put RE0-RE3 as output
        sw $t1, TRISE($t0)          #SAVE


loop:   lw $t1, PORTB($t0)          #READ port values
        andi $t1, 0x000F            #isulate just the bits we want

        lw $t2, LATE($t0)           #READ leds values
        andi $t2, $t2, 0xFFF0       #put 0 where we want

        or $t2, $t2, $t1            #change the values

        sw $t2, LATE($t0)           #STORE
        j loop

        jr $ra