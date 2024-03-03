				AREA asm_functions, CODE, READONLY	
	
;definizione di costanti e register renaming
;MY_CONST EQU 36
;my_register RN 0
	
calcola_somma_prodotto		PROC
				EXPORT calcola_somma_prodotto
				;R0 VETT
				;R1 n
				;R2 *alarm
				
				PUSH {r4-r12,lr}

vett RN 0
dim RN 1
alarm RN 2

i RN 3
result RN 4

curri RN 5
nexti RN 6
				mov i, #0
loop			
				
				LDR curri, [vett, i, lsl#2]
				add i, i, #1
				LDR nexti, [vett, i, lsl#2]
				add i,i
				
				mul curri, curri, nexti
				add result, curri
				
				cmp i, dim
				blt loop
				
				cmp result, #255
				movlt r7, #-1
				movge r7, #1
				STRB r7, [alarm]
				
the_end
				mov r0, result
				
				POP {r4-r12,pc} ;restore di tutti i registri utilizzati nel mio codice
				ENDP				
				
				END