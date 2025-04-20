%define NULL 0
%define TRUE 1
%define FALSE 0

section .data

section .text

global string_proc_list_create_asm
global string_proc_node_create_asm
global string_proc_list_add_node_asm
global string_proc_list_concat_asm

extern malloc
extern free
extern str_concat


string_proc_list_create_asm:
    mov rdi, 16
    call malloc
    cmp rax, 0
    je .malloc_err
    mov qword [rax], NULL
    mov qword [rax + 8], NULL
    ret
.malloc_err:
    xor rax, rax
    ret


string_proc_node_create_asm:
    push rbp
    mov rbp, rsp
    push rbx
    push r12

    cmp rsi, 0
    je .node_null

    mov r12, rsi
    mov bl, dil

    mov rdi, 32
    call malloc
    cmp rax, 0
    je .node_null

    mov qword [rax], NULL
    mov qword [rax + 8], NULL
    mov byte  [rax + 16], bl
    mov qword [rax + 24], r12

    pop r12
    pop rbx
    pop rbp
    ret

.node_null:
    xor rax, rax
    pop r12
    pop rbx
    pop rbp
    ret

string_proc_list_add_node_asm:
    push rbp
    mov rbp, rsp
    push rbx
    push r13
    push r14

    mov rbx, rdi       
    mov r13, rsi        
    mov r14, rdx       

    movzx edi, r13b
    mov rsi, r14
    call string_proc_node_create_asm
    cmp rax, 0
    je .end

    mov rcx, rax       

    mov rax, [rbx]
    cmp rax, 0
    jne .no_vacio

    mov [rbx], rcx
    mov [rbx + 8], rcx
    jmp .end

.no_vacio:
    mov rdx, [rbx + 8]  
    mov [rdx], rcx     
    mov [rcx + 8], rdx 
    mov [rbx + 8], rcx  

.end:
    pop r14
    pop r13
    pop rbx
    pop rbp
    ret

;===========================================================
; Concatenar todos los hashes del tipo y agregar nuevo nodo
;===========================================================
string_proc_list_concat_asm:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    push rbx
    push r12
    push r13
    push r14
    push r15

    mov rbx, rdi       
    movzx r12d, sil     
    mov r13, rdx      

    mov rdi, 1
    call malloc
    cmp rax, 0
    je .nulo
    mov byte [rax], 0
    mov r14, rax        

    mov r15, [rbx]     

.loop:
    cmp r15, 0
    je .siguiente

    mov al, byte [r15 + 16]
    cmp al, r12b
    jne .salto

    mov rdi, r14
    mov rsi, [r15 + 24]
    call str_concat
    test rax, rax
    je .fallo

    mov rdi, r14
    mov r14, rax
    call free

.salto:
    mov r15, [r15]
    jmp .loop

.siguiente:
    cmp r13, 0
    je .agregar

    mov rdi, r13
    mov rsi, r14
    call str_concat
    cmp rax, 0
    je .fallo

    mov rdi, r14
    mov r14, rax
    call free

.agregar:
    mov rdi, rbx
    movzx rsi, r12b
    mov rdx, r14
    call string_proc_list_add_node_asm

    mov rax, r14
    add rsp, 32
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

.fallo:
    test r14, r14
    je .nulo
    mov rdi, r14
    call free

.nulo:
    xor rax, rax
    add rsp, 32
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret