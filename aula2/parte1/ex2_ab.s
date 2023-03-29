# a)  K=20000 -> t=10^-3
    
    .equ    RESET_CORE_TIMER,12
    .equ    READ_CORE_TIMER,11

    .data
    .text
    .globl delay

delay:   li  $v0,RESET_CORE_TIMER
        syscall

while3:  li  $v0,READ_CORE_TIMER
        syscall

        li  $t0,K
        li  $t1,ms

        mult	$t0, $t1			# $t0 * $t1 = Hi and Lo registers
        mflo	$t0					# copy Lo to $t0
        blt     $v0,$t0,while3

        jr      $ra
