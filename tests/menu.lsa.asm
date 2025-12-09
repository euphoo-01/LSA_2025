global main
default rel
extern writech, writeuint, readch, pow, sqrt, isPrime, getMin, getMax, toUpper
section .text

; Ссылка на библиотеку: /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/libLSA_StdLib.a
; Пример сборки: g++ -o program program.o /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/libLSA_StdLib.a -no-pie

section .bss
    main_input resq 1

section .text

main:
    push rbp
    mov rbp, rsp
    call readch
    movzx rax, al
    push rax
    pop rax
    mov [main_input], rax
    push qword [main_input]
    push 48
    pop rbx
    pop rax
    sub rax, rbx
    push rax
    pop rax
    mov [main_input], rax
    push qword [main_input]
    push 1
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L0
    push 97
    pop rdi
    call writech
L0:
    push qword [main_input]
    push 0
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L2
    push 98
    pop rdi
    call writech
L2:
    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rbp
    ret
