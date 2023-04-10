        .equ    BASE, 0xBF88
        .equ    TRISE, 0x6100
        .equ    LATE, 0x6120
        .equ    RESET_CORE_TIMER, 12
        .equ    READ_CORE_TIMER, 11
        .equ    printInt, 6
        .equ    putChar, 3
        .equ    K, 20000
        .equ    ms, 142 

        .data

        .text
        .globl main

main:   lui     $t0, BASE
        
        lw      $t1, TRISE($t0)
        andi    $t1, $t1, 0xFFF0      #1111 1111 1111 0000
        sw      $t1, TRISE($t0)

loop:   lw      $t1, LATE($t0)
        andi    $t1, $t1, 0xFFF0
        ori     $t1, $t1, 0x0009
        sw      $t1, LATE($t0)

        li      $v0, printInt
        li      $a1, 2
        li      $a0, 0x9
        syscall

        li      $v0, putChar
        li      $a0, '\n'
        syscall

        jal     delay

        lw      $t1, LATE($t0)
        andi    $t1, $t1, 0xFFF0
        ori     $t1, $t1, 0x0006
        sw      $t1, LATE($t0)

        li      $v0,printInt
        li      $a1, 2
        li      $a0, 0x6
        syscall

        li      $v0, putChar
        li      $a0, '\n'
        syscall

        jal     delay

        j       loop
        
        jr      $ra

delay:  li      $v0, RESET_CORE_TIMER
        syscall

while:  li      $v0, READ_CORE_TIMER
        syscall

        li      $t2, K
        la      $t3, ms

        mulou	$t2, $t3,K
        blt     $v0,$t2,while
