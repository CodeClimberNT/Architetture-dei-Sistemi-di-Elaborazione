;;;
;;;	morse.s
;;;

					AREA   BellissimiDati, DATA, READWRITE
result_ 			space 100

					AREA    translate_function, CODE, READONLY
					EXPORT translate_morse
translate_morse					
; Input parameters:
; r0: unsigned char* vett_input
; r1: int vet_input_length
; r2: unsigned char* vett_output
; r3: int vet_output_length
; r4: char change_symbol
; r5: char space
; r6: char sentence_end

				; save current SP for a faster access 
				; to parameters in the stack
				MOV   R12, SP
				; save volatile registers
				STMFD SP!,{R4-R8,R10-R11,LR}				
				; extract argument 4, 5 and 6 into R4, R5 and R6
				LDMFD R12!,{R4-R6}
				;Renaming registers
vett_input			RN  R0
vet_input_length	RN  R1
vett_output			RN  R2
vet_output_length	RN  R3
change_symbol		RN  R4
space_symbol		RN  R5
sentence_end		RN  R6
curr_morse			RN 	R7
morse_tree			RN 	R8
i					RN 	R9
j					RN 	R10
offset				RN 	R11
converted_char		RN 	R12

SPACE_ASCII			EQU 32

				MOV i, #0
				MOV j, #0
Resetting_new_char
				MOV offset, #16 ;first iteration offset of 16
				;resetting morse_tree starting position
				LDR morse_tree, =Morse_to_ASCII_tree
				ADD morse_tree, morse_tree, #31
Morse_binary_search
				LDRB curr_morse, [vett_input, i]
				ADD i, i, #1

				
				
				CMP curr_morse, #1
				ADDEQ morse_tree, morse_tree, offset	;if dash(1) go to the right with of offset
				SUBLT morse_tree, morse_tree, offset	;if dot(0) go to the left with offset
				
				LSR offset, #1 							;reduce the offset value to converge in the binary search (16->8->4->2->1)
				
				CMP curr_morse, #1
				BLE Morse_binary_search

				;here only when search finished
				LDRB converted_char, [morse_tree]
				STRB converted_char, [vett_output, j]
				ADD	 j, j, #1
				CMP  curr_morse, space_symbol
				BLT  Resetting_new_char	;lt means continue
				BGT  ending_funct		;gt means sentence_end character so exit
				;if space symbol add it to the output vector
				MOV  converted_char, #SPACE_ASCII
				STRB converted_char, [vett_output, j]
				ADD j, j, #1
				B    Resetting_new_char
				

ending_funct
				;R0 return value
				MOV R0, j
				
				; restore volatile registers and return from function
				LDMFD SP!,{R4-R8,R10-R11,PC}
				
				LTORG
Boundary		space 1024
	
Morse_to_ASCII_tree 	DCB 53,72,52,83,00,86,51,73,00,70,00,85,00,00,50,69,00,76,00,82,00,00,00,65,00,80,00,87,00,74,49
						DCB	00
						DCB	54,66,00,68,00,88,00,78,00,67,00,75,00,89,00,84,55,90,00,71,00,81,00,77,56,00,00,79,57,00,48

;Ordered morse alphabet (*=invalid char in this exercise)
;[5,H,4,S,*,V,3,I,*,F,*,U,*,*,2,E,*,L,*,R,*,*,*,A,*,P,*,W,*,J,1]
;START
;[6,B,*,D,*,X,*,N,*,C,*,K,*,Y,*,T,7,Z,*,G,*,Q,*,M,8,*,*,O,9,*,0]
;ASCII CONVERSION
;DOT LEFT
;[53,72,52,83,0,86,51,73,0,70,0,85,0,0,50,69,0,76,0,82,0,0,0,65,0,80,0,87,0,74,49]
;START
;[54,66,0,68,0,88,0,78,0,67,0,75,0,89,0,84,55,90,0,71,0,81,0,77,56,0,0,79,57,0,48]		
;DASH RIGHT		
				END 
				
