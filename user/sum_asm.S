.global main

.data
buf:
        .zero   23
.LC0:
        .string "Length limit"
.LC1:
        .string "Wrong number length"
.LC2:
        .string "No space between nums"
.LC3:
        .string "Wrong string"
.LC4:
        .string "%l\n"

.text
main:
        addi    sp,sp,-64
        sw      ra,60(sp)
        sw      s0,56(sp)
        addi    s0,sp,64
        sw      a0,-52(s0)
        sw      a1,-56(s0)
        sw      zero,-20(s0)
        lui     a5,%hi(buf)
        addi    a5,a5,%lo(buf)
        sw      a5,-24(s0)
        li      a1,23
        lui     a5,%hi(buf)
        addi    a0,a5,%lo(buf)
        call    gets
        lui     a5,%hi(buf)
        addi    a0,a5,%lo(buf)
        call    strlen
        mv      a5,a0
        sw      a5,-28(s0)
        lw      a4,-28(s0)
        li      a5,22
        ble     a4,a5,.L2
        lui     a5,%hi(.LC0)
        addi    a0,a5,%lo(.LC0)
        call    printf
        li      a0,0
        call    exit
.L2:
        lw      a0,-24(s0)
        call    atoi
        sw      a0,-32(s0)
        j       .L3
.L5:
        lw      a5,-24(s0)
        addi    a5,a5,1
        sw      a5,-24(s0)
        lw      a5,-20(s0)
        addi    a5,a5,1
        sw      a5,-20(s0)
.L3:
        lw      a5,-24(s0)
        lbu     a4,0(a5)
        li      a5,47
        bleu    a4,a5,.L4
        lw      a5,-24(s0)
        lbu     a4,0(a5)
        li      a5,57
        bleu    a4,a5,.L5
.L4:
        lw      a4,-20(s0)
        li      a5,10
        ble     a4,a5,.L6
        lui     a5,%hi(.LC1)
        addi    a0,a5,%lo(.LC1)
        call    printf
        li      a0,0
        call    exit
.L6:
        lw      a5,-24(s0)
        lbu     a4,0(a5)
        li      a5,32
        beq     a4,a5,.L7
        lui     a5,%hi(.LC2)
        addi    a0,a5,%lo(.LC2)
        call    printf
        li      a0,0
        call    exit
.L7:
        lw      a5,-24(s0)
        addi    a5,a5,1
        sw      a5,-24(s0)
        lw      a5,-20(s0)
        addi    a5,a5,1
        sw      a5,-20(s0)
        lw      a0,-24(s0)
        call    atoi
        sw      a0,-36(s0)
        j       .L8
.L10:
        lw      a5,-24(s0)
        addi    a5,a5,1
        sw      a5,-24(s0)
        lw      a5,-20(s0)
        addi    a5,a5,1
        sw      a5,-20(s0)
.L8:
        lw      a5,-24(s0)
        lbu     a4,0(a5)
        li      a5,47
        bleu    a4,a5,.L9
        lw      a5,-24(s0)
        lbu     a4,0(a5)
        li      a5,57
        bleu    a4,a5,.L10
.L9:
        lw      a4,-20(s0)
        li      a5,21
        ble     a4,a5,.L11
        lui     a5,%hi(.LC1)
        addi    a0,a5,%lo(.LC1)
        call    printf
        li      a0,0
        call    exit
.L11:
        lw      a5,-24(s0)
        lbu     a4,0(a5)
        li      a5,10
        beq     a4,a5,.L12
        lw      a5,-24(s0)
        lbu     a4,0(a5)
        li      a5,13
        bne     a4,a5,.L13
.L12:
        lw      a5,-20(s0)
        addi    a5,a5,1
        sw      a5,-20(s0)
.L13:
        lw      a4,-20(s0)
        lw      a5,-28(s0)
        beq     a4,a5,.L14
        lui     a5,%hi(.LC3)
        addi    a0,a5,%lo(.LC3)
        call    printf
        li      a0,0
        call    exit
.L14:
        lw      a4,-32(s0)
        lw      a5,-36(s0)
        add     a5,a4,a5
        mv      a1,a5
        lui     a5,%hi(.LC4)
        addi    a0,a5,%lo(.LC4)
        call    printf
        li      a0,0
        call    exit