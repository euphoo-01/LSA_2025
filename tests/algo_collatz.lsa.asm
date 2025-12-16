global main
default rel
extern lsa_writech, lsa_writeuint, lsa_writelogic, lsa_readch, lsa_pow, lsa_sqrt, lsa_isPrime, lsa_getMin, lsa_getMax, lsa_toUpper
section .text

; Ссылка на библиотеку: /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a
; Пример сборки: g++ -o program program.o /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a -no-pie

section .bss
    main_n resq 1
    main_steps resq 1
    because0_i resq 1
    because0_rem resq 1

section .text

main:
    push rbp
    mov rbp, rsp
    push 6
    pop rax
    mov [main_n], rax
    push 0
    pop rax
    mov [main_steps], rax
    push 209
    pop rdi
    test rsp, 15
    jz L_aligned_0
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_0
L_aligned_0:
    call lsa_writech
L_end_0:
    push 242
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
    push 224
    pop rdi
    test rsp, 15
    jz L_aligned_2
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_2
L_aligned_2:
    call lsa_writech
L_end_2:
    push 240
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
    push 242
    pop rdi
    test rsp, 15
    jz L_aligned_4
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_4
L_aligned_4:
    call lsa_writech
L_end_4:
    push 58
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
    push 32
    pop rdi
    test rsp, 15
    jz L_aligned_6
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_6
L_aligned_6:
    call lsa_writech
L_end_6:
    push qword [main_n]
    pop rdi
    test rsp, 15
    jz L_aligned_7
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_7
L_aligned_7:
    call lsa_writeuint
L_end_7:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_8
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_8
L_aligned_8:
    call lsa_writech
L_end_8:
    push 0
    pop rax
    mov [because0_i], rax
L9:
    push qword [main_n]
    push 1
    pop rbx
    pop rax
    cmp rax, rbx
    setg al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L10
    push qword [main_n]
    push 2
    pop rbx
    pop rax
    xor rdx, rdx
    div rbx
    push rdx
    pop rax
    mov [because0_rem], rax
    push qword [because0_rem]
    push 0
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L11
    push qword [main_n]
    push 2
    pop rbx
    pop rax
    xor rdx, rdx
    div rbx
    push rax
    pop rax
    mov [main_n], rax
    jmp L12
L11:
    push 3
    push qword [main_n]
    pop rbx
    pop rax
    imul rax, rbx
    push rax
    push 1
    pop rbx
    pop rax
    add rax, rbx
    push rax
    pop rax
    mov [main_n], rax
L12:
    push qword [main_n]
    pop rdi
    test rsp, 15
    jz L_aligned_13
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_13
L_aligned_13:
    call lsa_writeuint
L_end_13:
    push 32
    pop rdi
    test rsp, 15
    jz L_aligned_14
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_14
L_aligned_14:
    call lsa_writech
L_end_14:
    inc qword [main_steps]
    push qword [because0_i]
    pop rax
    inc qword [because0_i]
    jmp L9
L10:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_15
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_15
L_aligned_15:
    call lsa_writech
L_end_15:
    push 216
    pop rdi
    test rsp, 15
    jz L_aligned_16
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_16
L_aligned_16:
    call lsa_writech
L_end_16:
    push 224
    pop rdi
    test rsp, 15
    jz L_aligned_17
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_17
L_aligned_17:
    call lsa_writech
L_end_17:
    push 227
    pop rdi
    test rsp, 15
    jz L_aligned_18
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_18
L_aligned_18:
    call lsa_writech
L_end_18:
    push 232
    pop rdi
    test rsp, 15
    jz L_aligned_19
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_19
L_aligned_19:
    call lsa_writech
L_end_19:
    push 58
    pop rdi
    test rsp, 15
    jz L_aligned_20
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_20
L_aligned_20:
    call lsa_writech
L_end_20:
    push 32
    pop rdi
    test rsp, 15
    jz L_aligned_21
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_21
L_aligned_21:
    call lsa_writech
L_end_21:
    push qword [main_steps]
    pop rdi
    test rsp, 15
    jz L_aligned_22
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_22
L_aligned_22:
    call lsa_writeuint
L_end_22:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_23
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_23
L_aligned_23:
    call lsa_writech
L_end_23:
    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rbp
    ret
