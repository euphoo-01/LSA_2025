global main
default rel
extern lsa_writech, lsa_writeuint, lsa_writelogic, lsa_readch, lsa_pow, lsa_sqrt, lsa_isPrime, lsa_getMin, lsa_getMax, lsa_toUpper
section .text

; Ссылка на библиотеку: /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a
; Пример сборки: g++ -o program program.o /home/euphoo/02. University/КП/LSA_2025/LSA_StdLib/build/libLSA_StdLib.a -no-pie

section .bss
    waitKey_ch resq 1

section .text

waitKey:
    push rbp
    mov rbp, rsp
    call lsa_readch
    movzx rax, al
    push rax
    pop rax
    mov [waitKey_ch], rax
    push qword [waitKey_ch]
    pop rdi
    call lsa_writech
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
