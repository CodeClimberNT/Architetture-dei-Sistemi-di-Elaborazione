.data
v1:
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.9
v2:
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.9
v3:
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.9
v4:
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8
.double 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.9


results:
v5: .space 512
v6: .space 512
v7: .space 512


.text
Main:
    dadd    R1,     R0,     R0                  ; initializing iterator
    daddi   R2,     R0,     64                  ; setting max value for iteration

Loop:
    l.d     F1,     v1(R1)                      ; load vector to fp register
    l.d     F2,     v2(R1)                      ; load vector to fp register
    l.d     F3,     v3(R1)                      ; load vector to fp register
    l.d     F4,     v4(R1)                      ; load vector to fp register

    mul.d   F5,     F1,     F2                  ; from the equation following the
    add.d   F5,     F1,     F3                  ; arithmetic logic order to save the result in F5
    add.d   F5,     F1,     F4
    s.d     F5,     v5(R1)                      ; save in memory the register

    add.d   F6,     F4,     F1
    div.d   F6,     F5,     F6
    s.d     F6,     v6(R1)

    add.d   F7,     F2,     F3
    mul.d   F7,     F6,     F7
    s.d     F7,     v7(R1)


    beq     R1,     R2,     Ending              ; branching if iterator reach max iteration value
    daddi   R1,     R1,     1                   ; incrementing iterator
    j       Loop

Ending:
    HALT                                        ; the end