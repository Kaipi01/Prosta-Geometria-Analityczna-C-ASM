.section .rodata

.macro CLOSE_FD fd
    movq $3, %rax #; 3 - close
    movq \fd, %rdi
    syscall
.endm

.macro WRITE buf len
    movq $1, %rax
    movq $1, %rdi
    leaq \buf(%rip), %rsi
    movq $\len, %rdx
    syscall
.endm 

.SAVE_FILE_SUCCESS:
    .string "\n\nDane zostały zapisane do pliku: %s\nPoprawnie zapisano: %d bajtów\n"
.SAVE_FILE_SUCCESS_LEN = .-.SAVE_FILE_SUCCESS

.CREATE_FILE_FAILED:
    .string "Utworzenie pliku się nie powiodło !\n"
.CREATE_FILE_FAILED_LEN = .-.CREATE_FILE_FAILED

.WRITE_FAILED:
    .string "Zapis danych się nie powiódł !\n"
.WRITE_FAILED_LEN = .-.WRITE_FAILED 

.section .text 

.globl save_file

#; Function creates .txt file and save data to this file
.type save_file, @function
save_file:
    #; %rdi - file name
    #; %rsi - file content buffer
    #; %rdx - content length
    pushq %rbp
    movq %rsp, %rbp

    pushq %r12 #; %r12 file name
    pushq %r13 #; %r13 file content
    pushq %r14 #; %r14 file fd
    pushq %r15 #; %r15 readed bytes

    movq %rdi, %r12  
    movq %rsi, %r13  
    movq %rdx, %r15  

    #; create and open file
    movq $2, %rax #; 2 - open
    movq %r12, %rdi #; file name
    movq $(1 | 0x40 | 0x200), %rsi #; flags: O_WRONLY | O_CREAT | O_TRUNC
    movq $0x1FF, %rdx #; full access mode
    syscall #; return fd in %rax

    movq %rax, %r14

    test %rax, %rax #; check fd value
    js create_file_failed_handler #; if SF=1 (%rax < 0)
 
    #; write content to file
    movq $1, %rax #; 1 - write
    movq %r14, %rdi
    movq %r13, %rsi
    movq %r15, %rdx
    syscall

    test %rax, %rax #; check write return value
    js write_failed_handler
    CLOSE_FD %r14 
    je save_file_clean

save_file_clean:
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbp
    ret

create_file_failed_handler:
    WRITE .CREATE_FILE_FAILED .CREATE_FILE_FAILED_LEN
    je save_file_clean
    
write_failed_handler:
    WRITE .WRITE_FAILED .WRITE_FAILED_LEN
    CLOSE_FD %r14
    je save_file_clean
