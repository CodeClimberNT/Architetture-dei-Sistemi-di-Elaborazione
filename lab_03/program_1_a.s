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
    daddi   R1, R0, 512     ; Initializing iterator
    daddi   R2, R0, 63      ; Initializing iterator
    daddi   R10, R0, 1      ; Odd/even check register

    ; Initialize m, k, p registers
    ld      R11, m(R0)      ; Load m
    mtc1    R0, F10         ; k
    mtc1    R0, F11         ; p

    mtc1    R0, F12         ; Prepare m result operation register

Loop:
    l.d     F1, v1(R1)      ; Load vector to FP register
    l.d     F2, v2(R1)      ; Load vector to FP register
    l.d     F3, v3(R1)      ; Load vector to FP register
    l.d     F4, v4(R1)      ; Load vector to FP register

    and     R30, R2, R10    ; Use R10 as a mask to retrieve the last bit of the iterator (0 for even, 1 for odd)
    beqz    R30, Even

Odd:
    dmul    R12, R11, R2    ; m * i
    mtc1    R12, F13         ; Move to FP register
    cvt.d.l F13, F13         ; Convert to FP format F13 = m * i
    div.d   F11, F1, F13    ; p = v1 / (m * i)

    ; 2^i
    dadd    R20, R0, R2     ; Copy iterator to R20
    daddi   R29, R0, 2      ; Load the value of power
    daddi   R30, R0, 1      ; Load neutral multiplication
    j       Pow

After_power:
    cvt.l.d F30, F4          ; (int) v4
    mfc1    R25, F4
    ddiv    R25, R25, R30   ; v4 / 2^i
    mtc1    R25, F10
    cvt.d.l F10, F10         ; Convert to float

    j       After_if

Even:
    dsllv   R12, R11, R2    ; m << i
    mtc1    R12, F13         ; Move to FP register
    cvt.d.l F13, F13         ; Convert to FP format
    mul.d   F11, F1, F13    ; p = v1 * m

    cvt.l.d F11, F11         ; Convert FP notation to binary
    mfc1    R11, F11         ; (int) p

    j       After_if

Pow:
    dmul    R30, R30, R29   ; Result *= 2
    daddi   R20, R20, -1
    bnez    R20, Pow
    j       After_power

After_if:
    ; Swap multiplication and addition to hide latency
    mul.d   F30, F11, F2     ; Multiply first
    add.d   F5, F3, F4       ; Then add
    s.d     F5, v5(R1)       ; Save the result in memory

    ; Swap multiplication and addition to hide latency
    mul.d   F6, F10, F1      ; Multiply first
    div.d   F6, F5, F6       ; Then divide
    s.d     F6, v6(R1)       ; Save the result in memory

    ; Swap multiplication and addition to hide latency
    mul.d   F7, F2, F3       ; Multiply first
    mul.d   F7, F6, F7       ; Then multiply again
    s.d     F7, v7(R1)       ; Save the result in memory

    daddi   R1, R1, -8       ; Decrement the iterator
    daddi   R2, R2, -1       ; Decrement the iterator
    bnez    R1, Loop         ; Loop until the iterator reaches zero

    HALT                    ; The end