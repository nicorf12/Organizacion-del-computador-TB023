
global strClone
global strPrint
global strCmp
global strLen
global strDelete

global arrayNew
global arrayDelete
global arrayPrint
global arrayGetSize
global arrayAddLast
global arrayGet
global arrayRemove
global arraySwap

global cardCmp
global cardClone
global cardAddStacked
global cardDelete
global cardGetSuit
global cardGetNumber
global cardGetStacked
global cardPrint
global cardNew

section .data
    ARRAY_OFF_TYPE EQU 0
    ARRAY_OFF_SIZE EQU 4
    ARRAY_OFF_CAPACITY EQU 5
    ARRAY_OFF_DATA EQU 8
    NULL_MSG db 'NULL', 0
    OPEN_BRACKET db '[', 0
    CLOSE_BRACKET db ']', 0
    COMMA db ",", 0
    ABRIR_LLAVE db '{', 0
    CERRAR_LLAVE db '}', 0
    GUION db '-', 0
    STACKED_VACIO db '[]', 0

section .bss

section .text
    extern malloc
    extern free
    extern fprintf
    extern getCloneFunction
    extern getDeleteFunction
    extern getPrintFunction
    extern intCmp
    extern intDelete
    extern listAddFirst
    extern listNew
    extern listDelete
    extern listPrint
    extern listClone
    extern strDelete

; ** String **
;char* strClone(char* a);
strClone:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    sub rsp, 8

    xor r12, r12
    xor r13, r13
    mov r12,rdi

    call strLen
    mov r13, rax

    inc r13
    mov rdi, r13
    call malloc
        
    xor r11,r11 ; contador
    xor r10,r10 ; aux
    .copyLoop:
        mov r10b, [r12+r11]
        mov [rax+r11], r10b
        cmp byte [r12+r11], 0
        je .end
        inc r11
        jmp .copyLoop

    .end:
        add rsp, 8
        pop r13
        pop r12
        pop rbp
        ret

;void strPrint(char* a, FILE* pFile)
strPrint:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    ; Argumentos:
    ; rdi: char* a
    ; rsi: FILE* pFile
    
    ; Verificar si los punteros son NULL
    cmp byte [rdi], 0
    jz .imprimirNULL     ; Si a es NULL, salir de la función
    cmp byte [rsi], 0
    jz .end     ; Si pFile es NULL, salir de la funció
    
    mov rdx, rdi
    mov rdi, rsi
    xor al, al
    mov rsi, rdx 
    call fprintf

    jmp .end

    .imprimirNULL:
        mov rdx, NULL_MSG
        mov rdi, rsi
        xor al, al
        mov rsi, rdx 
       call fprintf

    .end:
        add rsp, 16  ; Restaurar la pila antes de salir de la función
        pop rbp      ; Restaurar el puntero de pila base
        ret          ; Retornar de la función

;uint32_t strLen(char* a);
strLen:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    ;STACK FRAME
    xor r11,r11

    .lenLoop:
        cmp byte [rdi + r11],0
        je .fin
        inc r11
        jmp .lenLoop

    .fin:
        mov rax,r11
    ;STACK
    add rsp, 8
    pop rbp
    ret

;int32_t strCmp(char* a, char* b);
strCmp:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    ;STACK FRAME

    .loop:
        mov al, [rdi]
        mov bl, [rsi]
        cmp al, bl
        je .checkEnd
        jl .less
        jg .greater

    .checkEnd:
        cmp al, 0
        je .equal

        inc rdi
        inc rsi
        jmp .loop

    .equal:
        mov rax,0
        jmp .fin
    .less:
        mov rax, 1
        jmp .fin
    .greater:
        mov rax,-1
        jmp .fin

    .fin:
        ;STACK
        add rsp, 8
        pop rbp
        ret



;void strDelete(char* a);
strDelete:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    ;STACK FRAME

    call free

    ;STACK
    add rsp, 8
    pop rbp
    ret
; ** Array **
; uint8_t arrayGetSize(array_t* a)
arrayGetSize:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    ;STACK FRAME

    xor rax , rax
    mov al, [rdi+4]

    ;STACK
    add rsp, 8
    pop rbp
    ret

; void arrayAddLast(array_t* a, void* data)
arrayAddLast:
    push rbp ;98
    mov rbp, rsp
    push r12 ;96
    push r13 ;94
    push r14 ; 92
    push r15 ; 90
    ;STACK FRAME
    xor r12,r12
    xor r13,r13

    mov r15, rsi ; void* data
    mov r12b, [rdi+4] ;size
    mov r14, [rdi+8] ;data
    cmp r12b, [rdi+5]
    je .fin
    
    mov r13d, edi
    ;copio el dato y lo meto en el arreglo de punteros
    xor rdi,rdi
    mov dword edi, [r13d]
    call getCloneFunction ; rdi -> type
    mov rdi, r15
    call rax
    
    ;multiplico size por 8
    shl r12,3
    mov [r14+r12], rax
 
    ;incremento el size
    inc byte [r13+4]

    .fin:
        ;STACK
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret

; void* arrayGet(array_t* a, uint8_t i)
arrayGet:
    push rbp
    mov rbp, rsp
    sub rsp, 8
    ;STACK FRAME
    
    movzx rsi, sil
    movzx r8, byte [rdi+4]
    cmp rsi, r8

    jl .iCorrect
    xor rax, rax
    jmp .end

    .iCorrect:
        mov r8, [rdi+8]
        xor r9, r9
        movzx rsi, sil
        lea rax, [r8 + rsi*8]
        mov rax, [rax]

    .end:
        ;STACK
        add rsp, 8
        pop rbp
        ret

; array_t* arrayNew(type_t t, uint8_t capacity)
arrayNew:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    ;STACK FRAME

    xor r13, r13
    xor r12, r12

    mov r12d, edi     ; type_t t
    movzx r13, sil   ; uint8_t capacity

    mov rdi, 16      ; Tamaño de la estructura array_t
    call malloc      ; Reservar memoria para array_t
    mov r14, rax     ; Guardar el puntero a la estructura en r14

    mov [r14], r12d         ; type
    mov byte [r14+4], 0    ; size (inicializado en 0)
    mov [r14+5], r13b  ; capacity

    shl r13, 3        ; Multiplicar capacity por 8 (tamaño de puntero)
    mov rdi, r13      ; Tamaño necesario para data
    call malloc       ; Reservar memoria para data
    mov [r14+8], rax  ; Guardar el puntero a data en la estructura array_t
    
    mov rax, r14
    ;STACK 
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

; void* arrayRemove(array_t* a, uint8_t i)
arrayRemove:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    push r15
    sub rsp, 8
    ;STACK FRAME
    mov r12, rdi
    mov r14, rsi ; R14 -> i

    call arrayGet
    cmp rax,0
    je .iNotCorrect

    xor r13,r13 
    sub byte [r12+4], 1
    movzx r13, byte [r12+4] ;size
    mov r15, [r12+8] ; R15 APUNTA A PRIMERA CASILLA DE ARR PTR
    mov r8,r15 ; R8 APUNTA A PRIMERA CASILLA DE ARR PTR
    mov r15, [r15 + 8*r14] ; R15 APUNTA CASILLA ELEMENTO

    .loop:
        cmp r14, r13
        je .end

        mov r9, [r8+8*r14+8] ; R9 caja siguiente
        mov [r8+8*r14], r9 ; POS actual, guardo caja siguiente
        
        inc r14
        jmp .loop
    
    mov rax, r15
    jmp .end

    .iNotCorrect:
        xor rax,rax
    
    .end:
        ;STACK 
        add rsp, 8
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret

; void arraySwap(array_t* a, uint8_t i, uint8_t j)
arraySwap:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    push r15

    xor r13,r13
    xor r14,r14

    mov r12, rdi              ; puntero al array
    movzx r13, sil             ; indice I
    movzx r14, dl              ; indice J

    mov rdi, r12
    mov rsi, r13
    call arrayGet             ; arrayGet(a, i);
    mov r15, rax
    cmp rax, 0
    je .end             ; asegurarse que el indice i era valido 

    mov rdi, r12
    mov rsi, r14
    call arrayGet       ; arrayGet(a, j);
    cmp rax, 0
    je .end             ; asegurarse que el indice j era valido 

    ; En caso de que ambas posiciones sean validas, intercambia los elementos de posicion.
    .swap:      
        mov rcx, [r12 + ARRAY_OFF_DATA]
        shl r13, 3
        shl r14, 3

        mov [rcx + r13], rax
        mov [rcx + r14], r15

    .end:
        pop r15
        pop r14
        pop r13
        pop r12
        pop rbp
        ret


; void arrayDelete(array_t* a)
arrayDelete:
    push rbp ;98
    mov rbp, rsp
    push r12 ;96
    push r13 ;94
    push r14 ;92
    ;STACK FRAME

    mov r12, rdi

    xor rdi,rdi
    mov dword edi, [r12d]
    call getDeleteFunction ; rdi -> type
    mov r15, rax ; func que borra

    xor r14,r14
    xor r13,r13 
    movzx r13, byte [r12+4] ;size
    .loop:
        cmp r14, r13
        je .end 
        mov rdi, [r12 + 8]
        mov rdi, [rdi+ 8*r14]
        call r15
        inc r14
        jmp .loop

    .end:
        mov rdi, [r12+8]
        call free
        mov rdi, r12
        call free
    
        ;STACK 
        pop r14
        pop r13
        pop r12
        pop rbp
        ret

;void arrayPrint(array_t* a, FILE* pFile)
arrayPrint:
    push rbp
    mov rbp, rsp
    sub rsp, 16                 ; Alinear la pila a 16 bytes

    push r12
    push r13
    push r14
    push r15
    ; STACK FRAME
    mov r12, rdi                ; r12 -> a
    mov r13, rsi                ; r13 -> pFile
    
    ; Obtener el tipo de dato del array y la función de impresión
                   ; Guardar la dirección de la función de impresión en r14

    ; Imprimir [
    mov rdx, OPEN_BRACKET
    mov rdi, r13
    xor eax, eax
    mov rsi, rdx 
    call fprintf
    
    ; Inicializar contadores
    xor r14, r14
    xor r15, r15 
    movzx r15, byte [r12 + ARRAY_OFF_SIZE] ; r15 -> size

    ; Manejar el caso de array vacío
    cmp r15, 0
    je .print_close_bracket     ; Si el tamaño es cero, imprimir solo el cierre del bracket

.loop:
    ; Comparación
    cmp r14, r15
    je .print_close_bracket

    ; Imprimir el dato
    mov edi, [r12 + ARRAY_OFF_TYPE]
    call getPrintFunction       ; rax -> dirección de la función de impresión

    mov rdi, [r12 + ARRAY_OFF_DATA] ; Obtene data
    mov rdi, [rdi+ 8*r14]
    mov rsi, r13
    call rax

    ; Decrementar el índice
    inc r14

    ; Si no es el último elemento, imprimir la coma
    cmp r14, r15
    jz .print_close_bracket

    ; Imprimir la coma
    mov rdx, COMMA
    mov rdi, r13
    xor eax, eax
    mov rsi, rdx 
    call fprintf

    jmp .loop

.print_close_bracket:
    ; Imprimir ]
    mov rdx, CLOSE_BRACKET
    mov rdi, r13
    xor eax, eax
    mov rsi, rdx 
    call fprintf

    ; Restaurar la pila
    pop r15
    pop r14
    pop r13
    pop r12
    add rsp, 16                 ; Restaurar la alineación de la pila
    pop rbp
    ret

; ** Card **

; card_t* cardNew(char* suit, int32_t* number)
cardNew:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    push r15
    sub rsp, 8

    mov r12, rdi
    mov r13, rsi

    xor rdi, rdi
    mov rdi, 24
    call malloc
    mov r14, rax

    xor rdi, rdi
    mov dword edi, 2
    call getCloneFunction
    mov rdi, r12 
    call rax
    mov r15, rax

    xor rdi, rdi
    mov dword edi, 1
    call getCloneFunction
    mov rdi, r13
    call rax
    mov rsi, rax

    mov [r14], r15 ; le pongo el string
    mov [r14 + 8], rsi ; le pongo el numero

    xor rdi, rdi
    mov dil, 3
    call listNew
    mov [r14 + 16], rax ; le pongo la lista
    
    mov rax, r14 ; devuelvo la direccion de memoria de mi struct

    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbp
    ret

;char* cardGetSuit(card_t* c)
cardGetSuit:
    push rbp
    mov rbp, rsp
    mov rax, [rdi]
    pop rbp

ret

;int32_t* cardGetNumber(card_t* c)
cardGetNumber:
    push rbp
    mov rbp, rsp
    mov rax, [rdi + 8]
    pop rbp
ret

;list_t* cardGetStacked(card_t* c)
cardGetStacked:
    push rbp
    mov rbp, rsp
    mov rax, [rdi + 16]
    pop rbp
ret

;void cardPrint(card_t* c, FILE* pFile)
cardPrint:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    sub rsp, 16
    xor r12, r12
    xor r13, r13

    mov r12, rdi ; card
    mov r13, rsi ; pFIle

    mov rdx, ABRIR_LLAVE
    mov rdi, r13
    xor eax, eax
    mov rsi, rdx
    call fprintf
    
    mov rdi, 2
    call getPrintFunction
    mov rdi, [r12]
    mov rsi, r13
    call rax

    mov rdx, GUION
    mov rdi, r13
    xor eax, eax
    mov rsi, rdx
    call fprintf
    
    mov rdi, 1
    call getPrintFunction
    mov rdi, [r12+8]
    mov rsi, r13
    call rax

    mov rdx, GUION
    mov rdi, r13
    xor eax, eax
    mov rsi, rdx
    call fprintf
    
    mov rdi, [r12 + 16]
    mov rsi, r13
    cmp rdi, 0
    jz .stack_vacio
    call listPrint
    jmp .cierre

    .stack_vacio:
    mov rdi, STACKED_VACIO
    mov rsi, 13
    xor eax, eax
    mov rsi, rdx
    call fprintf
    jmp .cierre

    .cierre
    mov rdx, CERRAR_LLAVE
    mov rdi, r13
    xor eax, eax
    mov rsi, rdx
    call fprintf

    add rsp, 16
    pop r13
    pop r12
    pop rbp
    ret

;int32_t cardCmp(card_t* a, card_t* b)
cardCmp:
    push rbp
    mov rbp,rsp
    push r12
    push r13
    xor r12,r12
    xor r13,r13
    sub rsp, 8
   
    mov r12, rdi
    mov r13, rsi
    mov rdi, [r12]
    mov rsi, [r13]
    call strCmp
    cmp rax, 0
    je .stringEqual
    jmp .end

    .stringEqual:
        mov rdi, [r12+8]
        mov rsi, [r13+8]
        call intCmp

    .end:
        add rsp, 8
        pop r13
        pop r12
        pop rbp
        ret

;card_t* cardClone(card_t* c)
cardClone:
    push rbp
    mov rbp, rsp
    push r12
    push r13

    mov r12, rdi

    mov rdi, [r12]
    mov rsi, [r12 + 8]
    call cardNew
    mov r13, rax

    mov rdi, [r13+16]
    call listDelete

    mov rdi, [r12 + 16]
    call listClone
    mov [r13 + 16], rax

    mov rax,r13

    pop r13
    pop r12
    pop rbp
    ret

;void cardAddStacked(card_t* c, card_t* card)
cardAddStacked:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    
    mov rdi, [rdi + 16]
    call listAddFirst

    pop r13
    pop r12
    pop rbp
    ret

;void cardDelete(card_t* c)
cardDelete:
    push rbp
    mov rbp, rsp
    push r12
    sub rsp, 8

    mov r12, rdi

    mov rdi, [r12]
    call strDelete

    mov rdi, [r12+8]
    call intDelete

    mov rdi, [r12+16]
    call listDelete

    mov rdi, r12
    call free

    add rsp, 8
    pop r12
    pop rbp
    ret