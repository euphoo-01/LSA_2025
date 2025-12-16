global main
default rel
extern lsa_writech, lsa_writeuint, lsa_writelogic, lsa_readch, lsa_pow, lsa_sqrt, lsa_isPrime, lsa_getMin, lsa_getMax, lsa_toUpper
section .text

; Ссылка на библиотеку: /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a
; Пример сборки: g++ -o program program.o /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a -no-pie

section .bss
    gcd_a resq 1
    gcd_b resq 1
    gcd_temp resq 1
    because0_i resq 1
    main_a resq 1
    main_b resq 1

section .text

gcd:
    push rbp
    mov rbp, rsp
    mov rax, [rbp + 16]
    mov [gcd_a], rax
    mov rax, [rbp + 24]
    mov [gcd_b], rax
    push qword [gcd_temp]
    pop rax ; clear stack
    push 0
    pop rax
    mov [because0_i], rax
L0:
    push qword [gcd_b]
    push 0
    pop rbx
    pop rax
    cmp rax, rbx
    setg al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L1
    push qword [gcd_b]
    pop rax
    mov [gcd_temp], rax
    push qword [gcd_a]
    push qword [gcd_b]
    pop rbx
    pop rax
    xor rdx, rdx
    div rbx
    push rdx
    pop rax
    mov [gcd_b], rax
    push qword [gcd_temp]
    pop rax
    mov [gcd_a], rax
    push qword [because0_i]
    pop rax
    inc qword [because0_i]
    jmp L0
L1:
    push qword [gcd_a]
    pop rax
    mov rsp, rbp
    pop rbp
    ret 16
    pop rbp
    ret 16

main:
    push rbp
    mov rbp, rsp
    push 48
    pop rax
    mov [main_a], rax
    push 18
    pop rax
    mov [main_b], rax
    push 205
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
    push 206
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
    push 196
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
    push 40
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
    push qword [main_a]
    pop rdi
    test rsp, 15
    jz L_aligned_6
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_6
L_aligned_6:
    call lsa_writeuint
L_end_6:
    push 44
    pop rdi
    test rsp, 15
    jz L_aligned_7
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_7
L_aligned_7:
    call lsa_writech
L_end_7:
    push 32
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
    push qword [main_b]
    pop rdi
    test rsp, 15
    jz L_aligned_9
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_9
L_aligned_9:
    call lsa_writeuint
L_end_9:
    push 41
    pop rdi
    test rsp, 15
    jz L_aligned_10
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_10
L_aligned_10:
    call lsa_writech
L_end_10:
    push 58
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
    push 32
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
    push qword [main_a]
    push qword [main_b]
    call gcd
    push rax
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
    push 10
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
    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rbp
    ret
