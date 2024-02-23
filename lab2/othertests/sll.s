nop
    
test1:
    li x1, 0xf0
    li x2, 0x04
    xor x3,x1,x2
    li x4, 0x0f
    bne x3, x4, fail
    
pass:
    li x1, 10
    ecall
    
fail:
    li x1, 17
    ecall