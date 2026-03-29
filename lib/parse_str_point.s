.section .data

.HELP_BUF_LEN = 64
.HELP_BUF:
    .rept .HELP_BUF_LEN
        .byte 0
    .endr

.section .rodata 

.PARSE_POINT_FAILED:
    .string "Parsowanie danych punktu się nie powiodło !\n"
.PARSE_POINT_FAILED_LEN = .-.PARSE_POINT_FAILED

.PARSE_POINT_WRONG_INPUT:
    .string "Podano nie prawidłowe dane !\n"
.PARSE_POINT_WRONG_INPUT_LEN = .-.PARSE_POINT_WRONG_INPUT

.PARSE_POINT_WRONG_SYMBOL:
    .string "Podano nie prawidłowy znak ! Funkcja przyjmuje tylko 'x' i 'y' !\n"
.PARSE_POINT_WRONG_SYMBOL_LEN = .-.PARSE_POINT_WRONG_SYMBOL

.macro WRITE buf len
    movq $1, %rax
    movq $1, %rdi
    leaq \buf(%rip), %rsi
    movq $\len, %rdx
    syscall
.endm 

.macro PARSE_POINT start_char end_char
    movq %r12, %rdi
    movq \start_char, %rsi #; char = '(' 40
    call get_char_pos

    cmp $-1, %rax
    je parse_str_point_failed_handler

    movq %rax, %r14
    addq $1, %r14

    movq %r12, %rdi
    movq \end_char, %rsi #; char = ';' 59
    call get_char_pos

    cmp $-1, %rax
    je parse_str_point_failed_handler

    movq %rax, %r15

    movq %r12, %rdi
    movq %r14, %rsi
    movq %r15, %rdx
    call get_substring

    movq %rax, %rdi
    call atof 

    jmp parse_str_point_clean
.endm 

.section .text 

.globl parse_str_point
.globl get_char_pos
.globl get_substring

#; Function return first position number of given char
.type get_char_pos, @function
get_char_pos:
    #; %rdi - string address
    #; %rsi - char to find

    #; %rax - first position of given char, -1 if not found

    #; Function prolog
    pushq %rbp
    movq %rsp, %rbp

    pushq %r12 #; string
    pushq %r13 #; char
    pushq %r14 #; current char

    movq %rdi, %r12  
    movq %rsi, %r13  
    movq $0, %rax 

    jmp get_char_pos_loop

get_char_pos_loop:
    addq $1, %rax 

    movzbq (%r12, %rax, 1), %r14  

    cmp $0, %r14
    je get_char_pos_not_found

    cmp %r14, %r13
    jne get_char_pos_loop

    jmp get_char_pos_end

get_char_pos_not_found:
    movq $-1, %rax
    jmp get_char_pos_end

get_char_pos_end:
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    ret

#; Function return substring of given string based on start and and position index
.type get_substring, @function
get_substring:
    #; %rdi - string address
    #; %rsi - start pos
    #; %rdx - end pos

    #; %rax - sub string address

    pushq %rbp
    movq %rsp, %rbp

    pushq %r12 #; string
    pushq %r13 #; start pos
    pushq %r14 #; end pos

    movq %rdi, %r12  
    movq %rsi, %r13  
    movq %rdx, %r14  

    subq %r13, %r14

    leaq .HELP_BUF(%rip), %rdi
    movq %r12, %rsi
    movq %r14, %rdx

    addq %r13, %rsi

    call strncpy

    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    ret

#; Function parse x and y of given point in string format
.type parse_str_point, @function
parse_str_point:
    #; %rdi - string address
    #; %rsi - char (x or y) (78 or 79)
    #; return %xmm0 - parsed value
    pushq %rbp
    movq %rsp, %rbp

    pushq %r12 #; string
    pushq %r13 #; char (x or y)
    pushq %r14 #; start pos helper
    pushq %r15 #; end pos helper

    movq %rdi, %r12  
    movq %rsi, %r13  

    cmp $0x78, %r13
    je parse_str_point_for_x
    cmp $0x79, %r13
    je parse_str_point_for_y

    WRITE .PARSE_POINT_WRONG_SYMBOL .PARSE_POINT_WRONG_SYMBOL_LEN

    jmp parse_str_point_failed_handler    

parse_str_point_for_x:
    PARSE_POINT $40 $59 #; '(' => 40    ';' => 59

parse_str_point_for_y:
    PARSE_POINT $59 $41 #; ';' => 59    ')' => 41   

parse_str_point_clean:
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    ret 
    
parse_str_point_failed_handler:
    WRITE .PARSE_POINT_WRONG_INPUT .PARSE_POINT_WRONG_INPUT_LEN
    je parse_str_point_clean
