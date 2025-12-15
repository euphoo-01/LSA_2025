global main
default rel
extern lsa_writech, lsa_writeuint, lsa_writelogic, lsa_readch, lsa_pow, lsa_sqrt, lsa_isPrime, lsa_getMin, lsa_getMax, lsa_toUpper
section .text

; Ссылка на библиотеку: /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a
; Пример сборки: g++ -o program program.o /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a -no-pie

section .bss
    factorial_n resq 1
    differ0_result resq 1
    differ0_i resq 1
    main_n resq 1
    main_fact resq 1

section .text

factorial:
    push rbp
    mov rbp, rsp
    mov rax, [rbp + 16]
    mov [factorial_n], rax
    push qword [factorial_n]
    push 0
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L0
    push 1
    pop rax
    mov rsp, rbp
    pop rbp
    ret 8
    jmp L1
L0:
    push 1
    pop rax
    mov [differ0_result], rax
    push 1
    pop rax
    mov [differ0_i], rax
    push 1
    pop rax
    mov [differ0_i], rax
L2:
    push qword [differ0_i]
    push qword [factorial_n]
    pop rbx
    pop rax
    cmp rax, rbx
    setle al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L3
    push qword [differ0_result]
    push qword [differ0_i]
    pop rbx
    pop rax
    imul rax, rbx
    push rax
    pop rax
    mov [differ0_result], rax
    push qword [differ0_i]
    pop rax
    inc qword [differ0_i]
    jmp L2
L3:
    push qword [differ0_result]
    pop rax
    mov rsp, rbp
    pop rbp
    ret 8
L1:
    pop rbp
    ret 8

main:
    push rbp
    mov rbp, rsp
    test rsp, 15
    jz L_aligned_0
    sub rsp, 8
    call lsa_readch
    add rsp, 8
    jmp L_end_0
L_aligned_0:
    call lsa_readch
L_end_0:
    movzx rax, al
    push rax
    pop rax
    mov [main_n], rax
    push qword [main_n]
    push 48
    pop rbx
    pop rax
    sub rax, rbx
    push rax
    pop rax
    mov [main_n], rax
    push qword [main_n]
    call factorial
    push rax
    pop rax
    mov [main_fact], rax
    push qword [main_fact]
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
    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rbp
    ret
