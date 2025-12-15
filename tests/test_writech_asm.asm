global main
default rel
extern lsa_writech
section .text
main:
    push rbp
    mov rbp, rsp
    
    ; writech('A')
    push 65
    pop rdi
    test rsp, 15
    jz L_aligned
    sub rsp, 8
    call lsa_writech
    add rsp, 8
    jmp L_end
L_aligned:
    call lsa_writech
L_end:

    push 0
    pop rax
    mov rsp, rbp
    pop rbp
    ret
