    .equ BASE,  0xBF88                    
    .equ TRISE, 0x6100                                    
    .equ LATE, 0x6120                                         
    .equ TRISD, 0x60C0
    .equ PORTD, 0x60D0

    .data
    .text
    .globl main

main:
    lui     $t0, BASE                                           # $t0=0xBF880000
    lw      $t1, TRISD($t0)
    ori	    $t1, $t1, 0x0100 # 0000 0001 0000 0000
    sw		$t1, TRISD($t0)                                     # RD8 as input

    lw      $t1, TRISE($t0)
    andi	$t1, $t1, 0xFFFE            
    sw      $t1, TRISE($t0)                                     # RE0 as an output

loop:
    lw      $t1, PORTD($t0)
    not     $t1, $t1
    andi    $t1, $t1, 0x0100
    srl		$t1, 8			
    
    lw      $t2, LATE($t0)
    andi    $t2, $t2, 0xFFFE
    or		$t2, $t2, $t1
    sw      $t2, LATE($t0)

    j	    loop

    jr		$ra