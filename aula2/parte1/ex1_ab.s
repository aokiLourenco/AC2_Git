# a) 100Hz ----> 200000/2
# b)

    .equ    RESET_CORE_TIMER,12
    .equ    PUT_CHAR,3
    .equ    PRINT_INT,6
    .equ    READ_CORE_TIMER,11
    .equ    K,2000
    .equ    ms,1

    .data
    .text
    .globl main

main:   li		$t0,0                       # counter=0
while:                                      # while (1) {
        li      $v0,RESET_CORE_TIMER        #
        syscall                             #

while2: li      $v0,READ_CORE_TIMER
        syscall
        blt      $v0,200000,while2
        
        move 	$a0, $t0		            # $a0 = $t0
        li		$a1, 0x0004000A		        # 
        addi	$t0, $t0, 1			        # $t0 = $t0 + 1
        
        li      $v0,PRINT_INT
        syscall
        
        li      $a0,'\r'
        li      $v0,PUT_CHAR
        syscall

        j		while				        # jump to while

# ex.2 --------------------------------

delay:   li  $v0,RESET_CORE_TIMER
        syscall

while3:  li  $v0,READ_CORE_TIMER
        syscall

        li  $t0,K
        la  $t1,ms

        mulou	$t0, $t1,20000			# $t0 * $t1 = Hi and Lo registers
        blt     $v0,$t0,while3
        
        jr		$ra					        # jump to $ra
               
