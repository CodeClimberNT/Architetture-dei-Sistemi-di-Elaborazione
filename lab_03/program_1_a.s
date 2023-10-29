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
v5: .space  512
v6: .space  512
v7: .space  512



.text
Main:
    daddi   R1,         R0,     512             ; Initializing iterator (for memory)
    daddi   R2,         R0,     64              ; Initializing the other iterator (for shifting and oddity)
    daddi   R10,        R0,     1               ; Odd/even check register
    ld      R20,        m(R0)                   ; Initialize m (R20)
    mtc1    R0,         F11                     ; Initialize p (F11)
    mtc1    R0,         F12                     ; Initialize k (F12)

; 5+5 (5 for first then 1 for the rest)

Loop:
    daddi   R1,         R1,     -8              ; Decrement iterator by 8 (assuming double elements)
    daddi   R2,         R2,     -1              ; Decrement iterator by 1

; preload for the branch
    l.d     F1,         v1(R1)                  ; Load v1[i]
    l.d     F4,         v4(R1)                  ; Load v4[i]

    and     R30,        R2,     R10             ; Use R10 as a mask to check last bit
    bnez    R30,        Odd                     ; if last bit is not 0 then value is Odd

; 6+1 (1 extra for the instruction following the branch condition)

Even:
    dsllv   R12,        R20,    R2              ; m << i
    mtc1    R12,        F19                     ; Convert R12 to double
    cvt.d.l F19,        F19                     ; Convert R12 to double
    mul.d   F11,        F1,     F19             ; p = v1[i] * (m << i)

    cvt.l.d F20,        F11                     ; Convert F11 (int)p
    mfc1    R20,        F20                     ; m = (int)p

    j       After_if

; 5 + 8 (dont count first dsllv as it s counted in the after branch clock)

Odd:
    dmul    R30,        R20,    R2              ; m*i
    daddi   R12,        R0,     2               ; Initialize starting value for pow
    cvt.l.d F10,        F4                      ; Convert v4 to int
    dsllv   R12,        R12,    R2              ; 2^i
    mfc1    R14,        F10                     ; Convert v4 to int
    ddiv    R14,        R14,    R12             ; v4[i] / 2^i

    mtc1    R30,        F11                     ; move to fp register(double)m*i
    cvt.d.l F11,        F11                     ; p=(double)m*i
    div.d   F11,        F1,     F11             ; p = v1/p

    mtc1    R14,        F12                     ; Move R14 to fp register (k)
    cvt.d.l F12,        F12                     ; Convert k to fp notation

; 8(all 1 EX) + 8 + 20 + 20

After_if:
    l.d     F2,         v2(R1)                  ; Load v2[i]
    l.d     F3,         v3(R1)                  ; Load v3[i]


    mul.d   F5,         F11,    F2              ; p * v2[i]
    add.d   F8,         F3,     F4              ; v3[i] + v4[i]

    add.d   F6,         F12,    F1              ; k + v1[i]
    add.d   F7,         F2,     F3              ; v2[i] + v3[i]


    add.d   F5,         F5,     F8              ; (p * v2[i]) + v3[i] + v4[i]
    s.d     F5,         v5(R1)                  ; Store result in v5[i]

    div.d   F6,         F5,     F6              ; v5[i] / (k + v1[i])
    s.d     F6,         v6(R1)                  ; Store result in v6[i]

    mul.d   F7,         F6,     F7              ; v6[i] * (v2[i] + v3[i])
    s.d     F7,         v7(R1)                  ; Store result in v7[i]


    bnez    R2,         Loop                    ; Loop until the iterator reaches 0

    HALT                                        ; End of program

; 7(1 exe) + 12(3 exe) + 16(8 exe) + 20 (20 exe) + 1


; Main:                   5+5=10 * 1
; Loop(Loop+after_if): 7 + 55=62 * 64
; Even:                   5+8=13 * 32
; Odd:              20+20+8+8=56 * 32
; Halt:                       +1
; Total:                        6187
;