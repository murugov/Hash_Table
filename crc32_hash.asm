default rel
global crc32_hash_asm

section .text

crc32_hash_asm:
        mov     rdx, [rdi + 8]          ; word
        mov     eax, 0xFFFFFFFF         ; crc

        crc32   rax, qword [rdx]
        crc32   rax, qword [rdx + 8]
        crc32   rax, qword [rdx + 16]
        crc32   rax, qword [rdx + 24]

        not     eax 
        mov     [rdi], eax
        ret

section .note.GNU-stack noalloc noexec nowrite progbits