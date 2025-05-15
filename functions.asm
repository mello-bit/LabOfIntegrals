section .note.GNU-stack noalloc noexec nowrite progbits
; чтобы executable stack не выдавал варнинги

section .rodata
    const_0_5: dd 0.5
    const_1:   dd 1.0
    const_3:   dd 3.0
    const_2_5: dd 2.5
    const_9_5: dd 9.5
    const_5:   dd 5.0

section .text
global f1, f2, f3
; f1(x) = 3(0.5/(x + 1) + 1)
f1:
    push ebp
    mov ebp, esp
    
    
    finit
    
    fld qword [ebp + 8]    ; ST0 = x
    fld1             ; ST0 = 1, ST1 = X
    faddp st1, st0   ; ST0 = x + 1
    
    fld dword [const_0_5]  ; ST0 = 0.5, ST1 = X + 1
    fdivrp st1, st0         ; ST0 = 0.5/(x + 1)
    
    fld1
    faddp st1, st0
    
    fld dword[const_3]   ;ST0 = 3, ST1 = (0.5/(X + 1) + 1)
    fmulp st1, st0  
    
    mov esp, ebp
    pop ebp
    ret 
 
;f2(x) = 2.5x - 9.5

f2:
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[ebp + 8]
    fld dword[const_2_5]        ; ST0=2.5 , ST1 = X
    fmulp st1, st0              ; ST0 = 2.5X
    
    fld dword[const_9_5]        ; ST0 = 9.5, ST1 = 2.5x
    fsubp st1, st0
    
    mov esp, ebp
    pop ebp
    ret


;f(x) = 5/x  (x > 0) видимо придется проверять на то, какой мне ввели аргумент в си
f3:
    push ebp
    mov ebp, esp
    
    finit
    
    fld qword[ebp + 8]
    fld dword[const_5]   ; ST0 = 5, ST1 = x
    
    fdivrp st1, st0      ; ST0 = 5/X
    
    mov esp, ebp
    pop ebp
    ret
    
    
    
    
    
    
    
    
    
    
    
    
    
    