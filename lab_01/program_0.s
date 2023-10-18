; ---------------------------
; Program: program_0.s
; Sum of palindrome vectors
; ---------------------------


.data
vectors:                                                                       ;vectors to check if palindrome
    v1:     .byte       2, -2, 6, -3, 11, 1, 2, 9, 2, 1, 11, -3, 6, -2, 2
    v2:     .byte       4, 7, -10, 3, 11, 9, 7, 6, 4, 7, 20, 15, 9, -8, 2 
    v3:     .byte       10,9, 22, 5, -1, 21, 9, 5, 9, 21, -1, 5, 22, 9,10

flags:
    f1:     .space      1                                                           ; flags to check if palindrome
    f2:     .space      1                                                           ; flags to check if palindrome
    f3:     .space      1                                                           ; flags to check if palindrome
result:
v4:     .space      15                                                          ; final array for results

.text
Main:
    ld      R2,     v1(R0)
    daddi   R1,     R0,                             7                           ; iterator with length of floor(length of vectors/2)
    dadd    R2,     R0,                             R0                          ; pointer left-right register
    daddi   R3,     R0,                             14                          ; pointer right-left register


Check_v1:
    lb      R4,     v1(R2)                                                      ; load first byte to R2
    lb      R5,     v1(R3)                                                      ; load last byte to R3

    bne     R4,     R5,                             Check_v2                    ; branch if not equal (palindrome)

    daddi   R2,     R2,                             1                           ; move pointer across the vector
    daddi   R3,     R3,                             -1                          ; move pointer across the vector
    daddi   R1,     R1,                             -1                          ; decrement iterator

    bnez    R1,     Check_v1                                                    ; jump back to loop if iterator not zero
    daddi   R1,     R0,                             1                           ; add value to prepare for flag store
    sb      R1,     f1(R0)                                                      ; set flag if vector is palindrome

Reset_v1_to_v2:
    ld      R2,     v2(R0)
    daddi   R1,     R0,                             7                           ; iterator with length of floor(length of vectors/2)
    dadd    R2,     R0,                             R0                          ; pointer left-right register
    daddi   R3,     R0,                             14                          ; pointer right-left register


Check_v2:
    lb      R4,     v2(R2)                                                      ; load first byte to R2
    lb      R5,     v2(R3)                                                      ; load last byte to R3

    bne     R4,     R5,                             Check_v3                    ; branch if not equal (palindrome)

    daddi   R2,     R2,                             1                           ; move pointer across the vector
    daddi   R3,     R3,                             -1                          ; move pointer across the vector
    daddi   R1,     R1,                             -1                          ; decrement iterator

    bnez    R1,     Check_v2                                                    ; jump back to loop if iterator not zero
    daddi   R1,     R0,                             1                           ; add value to prepare for flag store
    sb      R1,     f2(R0)                                                      ; set flag if vector is palindrome

Reset_v2_to_v3:
    ld      R2,     v3(R0)
    daddi   R1,     R0,                             7                           ; iterator with length of floor(length of vectors/2)
    dadd    R2,     R0,                             R0                          ; pointer left-right register
    daddi   R3,     R0,                             14                          ; pointer right-left register

Check_v3:
    lb      R4,     v3(R2)                                                      ; load first byte to R2
    lb      R5,     v3(R3)                                                      ; load last byte to R3

    bne     R4,     R5,                             Check_where_to_start_sum    ; branch if not equal (palindrome)

    daddi   R2,     R2,                             1                           ; move pointer across the vector
    daddi   R3,     R3,                             -1                          ; move pointer across the vector
    daddi   R1,     R1,                             -1                          ; decrement iterator

    bnez    R1,     Check_v3                                                    ; jump back to loop if iterator not zero
    daddi   R1,     R0,                             1                           ; add value to prepare for flag store
    sb      R1,     f3(R0)                                                      ; set flag if vector is palindrome



Check_where_to_start_sum:
    lb      R6,     f1(R0)
    bnez    R6,     Reset_register_for_first_sum
    lb      R6,     f2(R0)
    bnez    R6,     Reset_register_for_second_sum
    lb      R6,     f3(R0)
    bnez    R6,     Reset_register_for_third_sum
    j       Ending                                                              ; if no flag is set, end the program


Reset_register_for_first_sum:
    daddi   R1,     R0,                             15                          ; length of result vector
    dadd    R2,     R0,                             R0                          ; pointer register

Sum_first_vector:
    lb      R3,     v1(R2)                                                      ; load origin vector register
    lb      R4,     v4(R2)                                                      ; load result vector register

    dadd    R3,     R3,                             R4                          ; sum ith byte from vector
    sb      R3,     v4(R2)                                                      ; store to result vector

    daddi   R2,     R2,                             1                           ; increment pointer
    bne     R1,     R2,                             Sum_first_vector

Check_sum_after_first:
    lb      R6,     f2(R0)
    bnez    R6,     Reset_register_for_second_sum
    lb      R6,     f3(R0)
    bnez    R6,     Reset_register_for_third_sum
    j       Ending                                                              ; if no flag is set, end the program


Reset_register_for_second_sum:
    daddi   R1,     R0,                             15                          ; length of result vector
    dadd    R2,     R0,                             R0                          ; pointer register

Sum_second_vector:
    lb      R3,     v2(R2)                                                      ; load origin vector register
    lb      R4,     v4(R2)                                                      ; load result vector register

    dadd    R3,     R3,                             R4                          ; sum ith byte from vector
    sb      R3,     v4(R2)                                                      ; store to result vector

    daddi   R2,     R2,                             1                           ; increment pointer
    bne     R1,     R2,                             Sum_second_vector

Check_sum_after_second:
    lb      R6,     f3(R0)
    bnez    R6,     Reset_register_for_third_sum
    j       Ending                                                              ; if no flag is set, end the program


Reset_register_for_third_sum:
    daddi   R1,     R0,                             15                          ; length of result vector
    dadd    R2,     R0,                             R0                          ; pointer register

Sum_third_vector:
    lb      R3,     v3(R2)                                                      ; load origin vector register
    lb      R4,     v4(R2)                                                      ; load result vector register

    dadd    R3,     R3,                             R4                          ; sum ith byte from vector
    sb      R3,     v4(R2)                                                      ; store to result vector

    daddi   R2,     R2,                             1                           ; increment pointer
    bne     R1,     R2,                             Sum_third_vector


Ending:
    HALT    