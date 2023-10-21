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


results:
v5: .space 512
v6: .space 512
v7: .space 512


.text
Main:
    dadd    R1,     R0,     R0                  ; initializing iterator
    daddi   R2,     R0,     512                  ; setting max value for iteration
                                                ; 2
Loop:
    l.d     F1,     v1(R1)                      ; load vector to fp register
    l.d     F2,     v2(R1)                      ; load vector to fp register
    l.d     F3,     v3(R1)                      ; load vector to fp register
    l.d     F4,     v4(R1)                      ; load vector to fp register
                                                ; 4

    ; v5 = (v1*v2) + v3 + v4
    mul.d   F30,    F1,     F2                  ; adding a temp register letting the multiplication
    add.d   F5,     F3,     F4                  ; instruction to be parallelized

    add.d   F5,     F30,    F5
    s.d     F5,     v5(R1)                      ; save in memory the register
                                                ; 8+3+3+1

                                                ; v6 = v5/(v4+v1)
    add.d   F6,     F4,     F1
    div.d   F6,     F5,     F6
    s.d     F6,     v6(R1)
                                                ; 3+20+1

    ; v7 = v6 * (v2+v3)
    add.d   F7,     F2,     F3
    mul.d   F7,     F6,     F7
    s.d     F7,     v7(R1)

    beq     R1,     R2,     Ending              ; branching if iterator reach max iteration value
    daddi   R1,     R1,     8                   ; incrementing iterator
    j       Loop
                                                ; 3+8+4

Ending:
    nop     
    HALT                                        ; the end
                                                ; 1