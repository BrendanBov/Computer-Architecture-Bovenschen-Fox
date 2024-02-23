nop

test1:
    li x1, 0x00
    li x2, 0x00
    xor x3,x1,x2
    li x4, 0x00
    bne x3, x4, fail

test2:
    li x1, 0xff
    li x2, 0x00
    xor x3,x1,x2
    li x4, 0xff
    bne x3, x4, fail

test3:
    li x1, 0x00
    li x2, 0xff
    xor x3,x1,x2
    li x4, 0xff
    bne x3, x4, fail

test4:
    li x1, 0xff
    li x2, 0xff
    xor x3,x1,x2
    li x4, 0x00
    bne x3, x4, fail

pass:
    li x1, 10
    ecall

fail:
    li x1, 17
    ecall