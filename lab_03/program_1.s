.data
v1:
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8
.double 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8
.double 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8
.double 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8
.double 6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8
.double 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8
.double 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8
v2:
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8
.double 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8
.double 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8
.double 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8
.double 6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8
.double 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8
.double 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8
v3:
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8
.double 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8
.double 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8
.double 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8
.double 6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8
.double 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8
.double 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8
v4:
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8
.double 3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8
.double 4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8
.double 5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8
.double 6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8
.double 7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8
.double 8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8

m:
.word 1

results:
v5: .space 512
v6: .space 512
v7: .space 512


.text
Main:
    daddi   R1,         R0,     512             ; initializing iterator
    daddi   R2,         R0,     63              ; initializing iterator
    daddi   R10,        R0,     1               ; odd/even check register

; initialize m,k,p registers
    ld      R11,        m(R0)                   ; load m
    mtc1    R0,         F10                     ; k
    mtc1    R0,         F11                     ; p


; 5+5 (5 for first then 1 for the rest)


Loop:

    l.d     F1,         v1(R1)                  ; load vector to fp register
    l.d     F2,         v2(R1)                  ; load vector to fp register
    l.d     F3,         v3(R1)                  ; load vector to fp register
    l.d     F4,         v4(R1)                  ; load vector to fp register

    and     R30,        R2,     R10             ; use R10 as mask to retrieve last bit of the iterator, if 0 is even, if 1 odd
    bnez    R30,        Odd

; 6+1(the "dsllv" under is always counted (32 times lost, 32 times used))


Even:
    dsllv   R12,        R11,    R2              ; m<<i
    mtc1    R12,        F13                     ; move to fp register
    cvt.d.l F13,        F13                     ; convert to fp format
    mul.d   F11,        F1,     F13             ; p=v1*m
    cvt.l.d F11,        F11                     ; convert fp notation in binary
    mfc1    R11,        F11                     ; m = (int)p
    j       After_if

; 5 + 8 (dont count first dsllv as it s counted in the after branch clock)



Odd:
    dmul    R12,        R11,    R2              ; m*i
    mtc1    R12,        F13                     ; move to fp register
    cvt.d.l F13,        F13                     ; convert to fp format F13=m*i
    div.d   F11,        F1,     F13             ; p=v1/m*i

    daddi   R29,        R0,     2               ; load value of pow base
    dsllv   R30,        R29,    R2              ; 2^i

    cvt.l.d F30,        F4                      ; (int) v4
    mfc1    R25,        F4                      ; R25 = (int)v4
    ddiv    R25,        R25,    R30             ; v4/2^i
    mtc1    R25,        F10
    cvt.d.l F10,        F10                     ; convert to float

; 8+20+20+8(this 8 are single clock execution)

; k:F10
; p:F11
; m:R11

After_if:
    mul.d   F30,        F11,    F2              ; adding a temp register letting the multiplication
    add.d   F5,         F3,     F4              ; instruction to be parallelized

    add.d   F5,         F5,     F30
    s.d     F5,         v5(R1)                  ; save in memory the register

    add.d   F6,         F10,    F1
    div.d   F6,         F5,     F6
    s.d     F6,         v6(R1)

    add.d   F7,         F2,     F3
    mul.d   F7,         F6,     F7
    s.d     F7,         v7(R1)

    daddi   R1,         R1,     -8              ; decrementing iterator
    daddi   R2,         R2,     -1              ; decrementing iterator
    bnez    R1,         Loop                    ; loop until iterator reach zero

    HALT                                        ; the end

; 8+3 + 3+1 + 3+20+1 + 3+8+1 + 3+1 (lost halt instruction for every iteration) + 1


; Main:                   5+5=10 * 1
; Loop(Loop+after_if): 7 + 55=62 * 64
; Even:                   5+8=13 * 32
; Odd:              20+20+8+8=56 * 32
; Halt:                       +1
; Total:                        6187
;