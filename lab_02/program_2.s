.data
inputs:
.double 1.1, 1.2, 1.3, 1.4, 1.5
.double 2.1, 2.2, 2.3, 2.4, 2.5
.double 3.1, 3.2, 3.3, 3.4, 3.5
.double 4.1, 4.2, 4.3, 4.4, 4.5
.double 5.1, 5.2, 5.3, 5.4, 5.5
.double 6.1, 6.2, 6.3, 6.4, 6.5

weights:
.double 1.1, 1.2, 1.3, 1.4, 1.5
.double 2.1, 2.2, 2.3, 2.4, 2.5
.double 3.1, 3.2, 3.3, 3.4, 3.5
.double 4.1, 4.2, 4.3, 4.4, 4.5
.double 5.1, 5.2, 5.3, 5.4, 5.5
.double 6.1, 6.2, 6.3, 6.4, 6.5

bias: .double 171                                                                                   ; decimal for 0xAB

check_value: .word16 2047                                                                           ; decimal for 0x7FF

y: .space 8                                                                                         ; result


.text
Main:
    dadd    R1,                         R0,                 R0                                      ; initialize iterator
    daddi   R2,                         R0,                 30                                      ; input vector length

    mtc1    R1,                         F3                                                          ; initialize x output to 0
    l.d     F4,                         bias(R0)                                                    ; load bias constant

Looping_inputs_and_weights:
    l.d     F1,                         inputs(R1)
    l.d     F2,                         weights(R1)
    mul.d   F30,                        F1,                 F2                                      ; temp register
    add.d   F3,                         F3,                 F30

    beq     R1,                         R2,                 Sum_bias_and_apply_activation_function
    daddi   R1,                         R1,                 1
    j       Looping_inputs_and_weights

Sum_bias_and_apply_activation_function:
    add.d   F3,                         F3,                 F4                                      ; add bias to result

    mfc1    R4,                         F3                                                          ; result fp moved to integer register

    lh      R5,                         check_value(R0)
    daddi   R30,                        R0,                 52                                      ; shift amout to have check_value in the equivalent position of the fp exponent
    dsllv   R5,                         R5,                 R30                                     ; value to not have loaded

    and     R4,                         R4,                 R5                                      ; use the R5 like a mask to get only the exponent bits
    beq     R4,                         R5,                 Ending
    j       Save_value

Save_value:
    s.d     F3,                         y(R0)                                                       ; save output value

Ending:
    HALT    