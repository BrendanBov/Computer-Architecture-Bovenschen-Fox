loadandstore:
    li s0, 1
    li s1, 2
    li s2, 3
    li s3, 4
    li sp, 0
    
    sb s0, 0(sp)
    sb s1, 1(sp)
    sh s2, 2(sp)
    sw s3, 4(sp)
    
    lb a0, 0(sp)
    lb a1, 1(sp)
    lh a2, 2(sp)
    lw a3, 4(sp)
    
    bne s0, a0, fail
    bne s1, a1, fail
    bne s2, a2, fail
    bne s3, a3, fail
    
    pass:
        li a0, 10
        ecall
    
    fail:
        li a0, 17
        ecall
