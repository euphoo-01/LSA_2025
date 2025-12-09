global main
default rel
extern lsa_writech, lsa_writeuint, lsa_readch, lsa_pow, lsa_sqrt, lsa_isPrime, lsa_getMin, lsa_getMax, lsa_toUpper
section .text

; Ссылка на библиотеку: /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/libLSA_StdLib.a
; Пример сборки: g++ -o program program.o /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/libLSA_StdLib.a -no-pie

section .bss
    main_x resq 1
    main_y resq 1
    main_a resq 1
    main_b resq 1
    main_p resq 1

section .text

main:
    push rbp
    mov rbp, rsp
    push 100
    pop rax
    mov [main_x], rax
    push qword [main_x]
    pop rdi
    call lsa_sqrt
    push rax
    pop rax
    mov [main_y], rax
    push qword [main_y]
    pop rdi
    call lsa_writeuint
    push 2
    pop rax
    mov [main_a], rax
    push 3
    pop rax
    mov [main_b], rax
    push qword [main_a]
    push qword [main_b]
    pop rsi
    pop rdi
    call lsa_pow
    push rax
    pop rax
    mov [main_p], rax
    push qword [main_p]
    pop rdi
    call lsa_writeuint
    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rbp
    ret
