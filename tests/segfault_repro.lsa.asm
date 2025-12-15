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
    waitKey_ch resq 1
    main_x resq 1
    main_y resq 1
    main_symbol resq 1
    main_expr resq 1
    main_sum resq 1
    main_product resq 1
    main_divide resq 1
    main_fact resq 1
    main_power resq 1
    main_root resq 1
    main_isPrimeNum resq 1
    main_uppered resq 1
    main_maxVal resq 1
    main_minVal resq 1
    main_diff resq 1
    main_counter resq 1
    because1_i resq 1

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
    push 1
    pop rax
    mov rsp, rbp
    pop rbp
    ret 8
    pop rbp
    ret 8

waitKey:
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
    mov [waitKey_ch], rax
    push qword [waitKey_ch]
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
    push qword [waitKey_ch]
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rbp
    ret

main:
    push rbp
    mov rbp, rsp
    push 5
    pop rax
    mov [main_x], rax
    push 2
    pop rax
    mov [main_y], rax
    push 104
    pop rax
    mov [main_symbol], rax
    push 100
    push 2
    push 9
    pop rbx
    pop rax
    add rax, rbx
    push rax
    pop rbx
    pop rax
    imul rax, rbx
    push rax
    push qword [main_x]
    pop rbx
    pop rax
    xor rdx, rdx
    div rbx
    push rax
    push 9
    pop rbx
    pop rax
    xor rdx, rdx
    div rbx
    push rdx
    pop rax
    mov [main_expr], rax
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
    push qword [main_expr]
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
    push qword [main_x]
    push qword [main_y]
    pop rbx
    pop rax
    add rax, rbx
    push rax
    pop rax
    mov [main_sum], rax
    push qword [main_x]
    push qword [main_y]
    pop rbx
    pop rax
    imul rax, rbx
    push rax
    pop rax
    mov [main_product], rax
    push qword [main_x]
    push qword [main_y]
    pop rbx
    pop rax
    xor rdx, rdx
    div rbx
    push rax
    pop rax
    mov [main_divide], rax
    push 10
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
    push qword [main_sum]
    pop rdi
    test rsp, 15
    jz L_aligned_8
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_8
L_aligned_8:
    call lsa_writeuint
L_end_8:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_9
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_9
L_aligned_9:
    call lsa_writech
L_end_9:
    push qword [main_product]
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
    push 10
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
    push qword [main_divide]
    pop rdi
    test rsp, 15
    jz L_aligned_12
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_12
L_aligned_12:
    call lsa_writeuint
L_end_12:
    push qword [main_x]
    pop rax
    inc qword [main_x]
    pop rax ; clear stack
    push qword [main_y]
    pop rax
    dec qword [main_y]
    pop rax ; clear stack
    push qword [main_x]
    call factorial
    push rax
    pop rax
    mov [main_fact], rax
    push 10
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
    push qword [main_fact]
    pop rdi
    test rsp, 15
    jz L_aligned_14
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_14
L_aligned_14:
    call lsa_writeuint
L_end_14:
    push 2
    push 1
    pop rsi
    pop rdi
    call lsa_pow
    push rax
    pop rax
    mov [main_power], rax
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
    push qword [main_power]
    pop rdi
    test rsp, 15
    jz L_aligned_16
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_16
L_aligned_16:
    call lsa_writeuint
L_end_16:
    push 256
    pop rdi
    call lsa_sqrt
    push rax
    pop rax
    mov [main_root], rax
    push 10
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
    push qword [main_root]
    pop rdi
    test rsp, 15
    jz L_aligned_18
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_18
L_aligned_18:
    call lsa_writeuint
L_end_18:
    push 17
    pop rdi
    call lsa_isPrime
    push rax
    pop rax
    mov [main_isPrimeNum], rax
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
    push qword [main_isPrimeNum]
    pop rdi
    test rsp, 15
    jz L_aligned_20
    sub rsp, 8
    call lsa_writelogic
    add rsp, 8
    jmp L_end_20
L_aligned_20:
    call lsa_writelogic
L_end_20:
    push qword [main_symbol]
    pop rdi
    call lsa_toUpper
    movzx rax, al
    push rax
    pop rax
    mov [main_uppered], rax
    push qword [main_x]
    push qword [main_y]
    pop rsi
    pop rdi
    call lsa_getMax
    push rax
    pop rax
    mov [main_maxVal], rax
    push 10
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
    push qword [main_maxVal]
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
    push qword [main_x]
    push qword [main_y]
    pop rsi
    pop rdi
    call lsa_getMin
    push rax
    pop rax
    mov [main_minVal], rax
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
    push qword [main_minVal]
    pop rdi
    test rsp, 15
    jz L_aligned_24
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_24
L_aligned_24:
    call lsa_writeuint
L_end_24:
    push qword [main_diff]
    pop rax ; clear stack
    push qword [main_x]
    push qword [main_y]
    pop rbx
    pop rax
    cmp rax, rbx
    setg al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L25
    push qword [main_x]
    push qword [main_y]
    pop rbx
    pop rax
    sub rax, rbx
    push rax
    pop rax
    mov [main_diff], rax
    jmp L26
L25:
    push qword [main_y]
    push qword [main_x]
    pop rbx
    pop rax
    sub rax, rbx
    push rax
    pop rax
    mov [main_diff], rax
L26:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_27
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_27
L_aligned_27:
    call lsa_writech
L_end_27:
    push qword [main_diff]
    pop rdi
    test rsp, 15
    jz L_aligned_28
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_28
L_aligned_28:
    call lsa_writeuint
L_end_28:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_29
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_29
L_aligned_29:
    call lsa_writech
L_end_29:
    push 0
    pop rax
    mov [main_counter], rax
    push 0
    pop rax
    mov [because1_i], rax
L30:
    push qword [because1_i]
    push 10
    pop rbx
    pop rax
    cmp rax, rbx
    setl al
    movzx rax, al
    push rax
    pop rax
    cmp rax, 0
    je L31
    push qword [main_counter]
    push qword [because1_i]
    pop rbx
    pop rax
    add rax, rbx
    push rax
    pop rax
    mov [main_counter], rax
    push qword [main_counter]
    pop rdi
    test rsp, 15
    jz L_aligned_32
    sub rsp, 8
    call lsa_writeuint
    add rsp, 8
    jmp L_end_32
L_aligned_32:
    call lsa_writeuint
L_end_32:
    push 9
    pop rdi
    test rsp, 15
    jz L_aligned_33
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_33
L_aligned_33:
    call lsa_writech
L_end_33:
    push qword [because1_i]
    pop rax
    inc qword [because1_i]
    jmp L30
L31:
    push 10
    pop rdi
    test rsp, 15
    jz L_aligned_34
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end_34
L_aligned_34:
    call lsa_writech
L_end_34:
    call waitKey
    push rax
    pop rax ; clear stack
    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    pop rbp
    ret
