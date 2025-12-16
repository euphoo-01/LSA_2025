global main
default rel
extern lsa_writech, lsa_writeuint, lsa_writelogic, lsa_readch, lsa_pow, lsa_sqrt, lsa_isPrime, lsa_getMin, lsa_getMax, lsa_toUpper
section .text

; Ссылка на библиотеку: /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a
; Пример сборки: g++ -o program program.o /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a -no-pie

section .bss
    isPerfect_n resq 1
    isPerfect_sum resq 1
    because0_i resq 1
    because1_k resq 1

section .text

isPerfect:
    push rbp
    mov rbp, rsp
    mov rax, [rbp + 16]
    mov [isPerfect_n], rax
    push 0
    pop rax
    mov [isPerfect_sum], rax
    push 1
    pop rax
    mov [because0_i], rax
L0:
    push qword [because0_i]
    push 2
    pop rbx
    pop rax
    imul rax, rbx
    push rax
    push qword [isPerfect_n]
    pop rbx
    pop rax
    cmp rax, rbx
    setle al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L1
    push qword [isPerfect_n]
    push qword [because0_i]
    pop rbx
    pop rax
    xor rdx, rdx
    div rbx
    push rdx
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
    push qword [isPerfect_sum]
    push qword [because0_i]
    pop rbx
    pop rax
    add rax, rbx
    push rax
    pop rax
    mov [isPerfect_sum], rax
L2:
    push qword [because0_i]
    pop rax
    inc qword [because0_i]
    jmp L0
L1:
    push qword [isPerfect_sum]
    push qword [isPerfect_n]
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L4
    push 1
    pop rax
    mov rsp, rbp
    pop rbp
    ret 8
L4:
    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret 8
    pop rbp
    ret 8

main:
    push rbp
    mov rbp, rsp
    push 1
    pop rax
    mov [because1_k], rax
L6:
    push qword [because1_k]
    push 1000
    pop rbx
    pop rax
    cmp rax, rbx
    setl al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L7
    push qword [because1_k]
    call isPerfect
    push rax
    push 1
    pop rbx
    pop rax
    cmp rax, rbx
    sete al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L8
    push qword [because1_k]
    pop rdi
    test rsp, 15
    jz L_aligned_10
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_10
L_aligned_10:
    call lsa_writeuint
L_end_10:
    push 32
    pop rdi
    test rsp, 15
    jz L_aligned_11
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_11
L_aligned_11:
    call lsa_writech
L_end_11:
    push 61
    pop rdi
    test rsp, 15
    jz L_aligned_12
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_12
L_aligned_12:
    call lsa_writech
L_end_12:
    push 32
    pop rdi
    test rsp, 15
    jz L_aligned_13
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_13
L_aligned_13:
    call lsa_writech
L_end_13:
    push 200
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
    push 228
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
    push 229
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
    push 235
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
    push 10
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
L8:
    push qword [because1_k]
    pop rax
    inc qword [because1_k]
    jmp L6
L7:
    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rbp
    ret
