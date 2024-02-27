nop

test1:
    lui x1, 0xf0f0f
    addi x1, x1, 0x0f0
    lui x2, 0xf0f0f
    addi x2, x2, 0x0f0
    bne x1, x2, fail
    
pass:
    li x1, 10
    ecall
    
fail:
    li x1, 17
    ecall
