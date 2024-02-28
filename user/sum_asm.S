.global main

.data
buf:
        .zero   23
.LC0:
        .string "Length limit\n"
.LC1:
        .string "Wrong number length\n"
.LC2:
        .string "No space between nums\n"
.LC3:
        .string "Wrong string\n"
.LC4:
        .string "%l\n"

.text
main:
        # кусочек для выделения памяти на стеке, сохранения адреса возврата,
        # и параметров мейна
        addi    sp,sp,-64
        sw      ra,60(sp)
        sw      s0,56(sp)
        addi    s0,sp,64
        sw      a0,-52(s0)
        sw      a1,-56(s0)

        # обнуление переменной cnt
        sw      zero,-20(s0)

        # сохранение адреса buf в tmp
        la      a5, buf
        sw      a5,-24(s0)

        # передал параметры (длину и адрес buf) и вызвал gets
        li      a1,23
        la      a0, buf
        call    gets

        # передал параметры и вызвал
        la      a0, buf
        call    strlen

        # получил и сохранил вычисленную длину
        mv      a5,a0
        sw      a5,-28(s0)

        # иф на проверку с максимальной длиной строки
        lw      a4,-28(s0)
        li      a5,22
        ble     a4,a5,.L2

        # загрузил адрес строки и stderr
        la      a1, .LC1
        li      a0,2
        call    fprintf

        li      a0,-1
        call    exit
.L2:
        # переход сюда, если условие выше не выполнилось

        # вызов atoi и сохранение значения
        lw      a0,-24(s0)
        call    atoi
        sw      a0,-32(s0)

        j       .L3
.L5:
        # тело while
        lw      a5,-24(s0)
        addi    a5,a5,1
        sw      a5,-24(s0)
        lw      a5,-20(s0)
        addi    a5,a5,1
        sw      a5,-20(s0)
.L3:
        # проверка условия while
        lw      a5,-24(s0)
        # загружаю первый байтик из tmp
        lbu     a4,0(a5)
        # загружаю чиселко
        li      a5,47
        # иф <=
        bleu    a4,a5,.L4
        # аналогично
        lw      a5,-24(s0)
        lbu     a4,0(a5)
        li      a5,57
        bleu    a4,a5,.L5
.L4:
        lw      a4,-20(s0)
        li      a5,10
        ble     a4,a5,.L6
        la      a1, .LC1
        li      a0,2
        call    fprintf
        li      a0,-1
        call    exit
.L6:
        lw      a5,-24(s0)
        lbu     a4,0(a5)
        li      a5,32
        beq     a4,a5,.L7
        la      a1, .LC2
        li      a0,2
        call    fprintf
        li      a0,-1
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
        la      a1, .LC1
        li      a0,2
        call    fprintf
        li      a0,-1
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
        la      a1, .LC3
        li      a0,2
        call    fprintf
        li      a0,-1
        call    exit
.L14:
        lw      a4,-32(s0)
        lw      a5,-36(s0)
        add     a5,a4,a5
        mv      a1,a5
        la      a0, .LC4
        call    printf
        li      a0,0
        call    exit