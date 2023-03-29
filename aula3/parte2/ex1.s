    .equ    BASE, 0xBF88
    .equ    TRISE, 0x6100
    .equ    PORTE, 0x6110
    .equ    TRISB, 0x6040
    .equ    PORTB, 0x6050
    .equ    LATE, 0x6120
    .equ    RESET_CORE_TIMER,12
    .equ    READ_CORE_TIMER,11

    .data
    .text
    .globl main

main:   
    lui     $t0, BASE
    lw      $t1, TRISB($t0)
    ori     $t1, $t1, 0x0000     # 0000 0010
    sw      $t1, TRISB($t0)      # RB as ipnut

    lw      $t1, TRISE($t0)
    andi    $t1, $t1, 0x0000          # 1111 1111 1110 0001
    sw      $t1, TRISE($t0)      # RE as output

    lw      $t1, PORTB($t0)
    andi    $t1, $t1, 0x0000

if:
    lw      $t1, TRISB($t0)
    ori     $t1,$t1,0x0001
    
    li      $t2, 0          # counter = 0

loop:
    lw      $t3, LATE($t0)
    andi    $t3, $t3, 0x0000
    #sll		$t4, $t2, 1
    or      $t3, $t3, $t2 # -> $t4
    sw      $t3, LATE($t0)

    li      $v0, RESET_CORE_TIMER
    syscall

delay:
    li      $v0, READ_CORE_TIMER
    syscall
    blt     $v0, 20000000, delay

    addi	$t2, $t2, 1			# $t0 = $t1 + 0
    blt     $t2, 16, cont
    li      $t2, 0

    #sub     $t2, $t2, 1
    #bgt     $t2, 0, cont
    #li      $t2, 0

cont:
    j       loop
    jr      $ra