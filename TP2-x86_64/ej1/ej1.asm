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
    mov     rdi, 16           ; tamaño de la estructura list
    call    malloc            ; allocate list
    cmp     rax, 0            ; si malloc devuelve NULL
    je      .malloc_err       ; saltar a error
    mov     qword [rax], NULL ; inicializar head = NULL
    mov     qword [rax + 8], NULL ; inicializar tail = NULL
    ret
.malloc_err:
    xor     rax, rax          ; retornar NULL
    ret

string_proc_node_create_asm:
    push    rbp               ; establecer marco de pila
    mov     rbp, rsp
    push    rbx               ; salvar callee-saved
    push    r12

    cmp     rsi, 0            ; verificar hash != NULL
    je      .node_null

    mov     r12, rsi          ; guardar puntero hash en r12
    mov     bl, dil           ; extraer type a registro byte

    mov     rdi, 32           ; tamaño de node
    call    malloc            ; allocate node struct
    cmp     rax, 0            ; comprobar fallo malloc
    je      .node_null

    mov     qword [rax], NULL ; node->next = NULL
    mov     qword [rax + 8], NULL ; node->prev = NULL
    mov     byte  [rax + 16], bl ; node->type = saved type
    mov     qword [rax + 24], r12 ; node->hash = hash ptr

    pop     r12               ; restaurar registros
    pop     rbx
    pop     rbp
    ret

.node_null:
    xor     rax, rax          ; devolver NULL en caso de error
    pop     r12
    pop     rbx
    pop     rbp
    ret

string_proc_list_add_node_asm:
    push    rbp
    mov     rbp, rsp
    push    rbx               ; salvar registro de lista
    push    r13               ; salvar type
    push    r14               ; salvar hash

    mov     rbx, rdi          ; rbx = puntero a list
    mov     r13, rsi          ; r13 = type
    mov     r14, rdx          ; r14 = hash ptr

    movzx   edi, r13b         ; preparar argumento type
    mov     rsi, r14          ; preparar argumento hash
    call    string_proc_node_create_asm
    cmp     rax, 0            ; ¿new_node == NULL?
    je      .end_add          ; si no, terminamos

    mov     rcx, rax          ; new_node ptr en rcx
    mov     rax, [rbx]        ; rax = list->first
    cmp     rax, 0
    jne     .list_not_empty   ; si lista no vacía, enlazar al final

    mov     [rbx], rcx        ; head = new_node (lista vacía)
    mov     [rbx + 8], rcx    ; tail = new_node
    jmp     .end_add

.list_not_empty:
    mov     rdx, [rbx + 8]    ; rdx = list->last
    mov     [rdx], rcx        ; last->next = new_node
    mov     [rcx + 8], rdx    ; new_node->prev = old last
    mov     [rbx + 8], rcx    ; tail = new_node

.end_add:
    pop     r14               ; restaurar registros
    pop     r13
    pop     rbx
    pop     rbp
    ret

string_proc_list_concat_asm:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32           ; espacio para registros locales
    push    rbx               ; salvar registers usados
    push    r12
    push    r13
    push    r14
    push    r15

    mov     rbx, rdi          ; rbx = list ptr
    movzx   r12d, sil         ; r12 = filter type
    mov     r13, rdx          ; r13 = extra hash ptr

    mov     rdi, 1            ; crear cadena vacía ("\0")
    call    malloc
    cmp     rax, 0            ; comprobar malloc
    je      .null_result
    mov     byte [rax], 0     ; inicializar terminador
    mov     r14, rax          ; r14 = acumulador de hashes

    mov     r15, [rbx]        ; r15 = primer nodo

.LoopConcat:
    test    r15, r15
    je      .AfterList        ; si fin de lista, continuar con extra

    mov     al, [r15 + 16]    ; cargar node->type
    cmp     al, r12b          ; ¿coincide con filter?
    jne     .NextNode         ; si no, siguiente

    mov     rdi, r14          ; cadena actual
    mov     rsi, [r15 + 24]   ; nodo->hash
    call    str_concat        ; concatenar
    test    rax, rax
    je      .cleanup          ; fallo en concatenación

    mov     rdi, r14          ; preparar free(old cadena)
    mov     r14, rax          ; actualizar acumulador
    call    free              ; liberar cadena anterior

.NextNode:
    mov     r15, [r15]        ; avanzar al siguiente nodo
    jmp     .LoopConcat

.AfterList:
    test    r13, r13          ; ¿hay extra hash?
    je      .AppendResult
    mov     rdi, r14          ; cadena actual
    mov     rsi, r13          ; extra hash
    call    str_concat        ; última concatenación
    test    rax, rax
    je      .cleanup

    mov     rdi, r14
    mov     r14, rax
    call    free              ; liberar antigua

.AppendResult:
    mov     rdi, rbx          ; args para append
    movzx   rsi, r12b
    mov     rdx, r14
    call    string_proc_list_add_node_asm ; añadir nodo con hash final

    mov     rax, r14          ; devolver puntero al string concatenado
    add     rsp, 32
    pop     r15               ; restaurar registros
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    ret

.cleanup:
    xor     eax, eax          ; error devuelve NULL
    add     rsp, 32
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    ret

.null_result:
    xor     eax, eax          ; fallo en malloc inicial
    add     rsp, 32
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    ret
