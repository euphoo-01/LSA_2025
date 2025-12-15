global main
default rel
extern lsa_writech, lsa_writeuint, lsa_writelogic, lsa_readch, lsa_pow, lsa_sqrt, lsa_isPrime, lsa_getMin, lsa_getMax, lsa_toUpper
section .text

; Ссылка на библиотеку: /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a
; Пример сборки: g++ -o program program.o /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a -no-pie

section .bss
    main_hexVal resq 1
    main_hexVal2 resq 1
    main_zero resq 1

section .text

main:
    push rbp
    mov rbp, rsp
    push 255
    pop rax
    mov [main_hexVal], rax
    push 26
    pop rax
    mov [main_hexVal2], rax
    push 0
    pop rax
    mov [main_zero], rax
    push qword [main_hexVal]
    pop rdi
    test rsp, 15
    jz L_aligned_0
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_0
L_aligned_0:
    call lsa_writeuint
L_end_0:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_1
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_1
L_aligned_1:
    call lsa_writech
L_end_1:
    push qword [main_hexVal2]
    pop rdi
    test rsp, 15
    jz L_aligned_2
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_2
L_aligned_2:
    call lsa_writeuint
L_end_2:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_3
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_3
L_aligned_3:
    call lsa_writech
L_end_3:
    push qword [main_zero]
    pop rdi
    test rsp, 15
    jz L_aligned_4
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_4
L_aligned_4:
    call lsa_writeuint
L_end_4:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_5
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_5
L_aligned_5:
    call lsa_writech
L_end_5:
    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rbp
    ret
