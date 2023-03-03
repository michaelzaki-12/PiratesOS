; generate 64-bit code
bits 64
%include "header.inc"
; contains the code that will run
section .text

; allows the linker to see this symbol
global main

; see http://www.uefi.org/sites/default/files/resources/UEFI Spec 2_7_A Sept 6.pdf#G8.1001729
loopForever:
    jmp loopForever

main:
    sub rsp, 4 * 8 + 8 
    mov rcx, [rdx + 64]
    lea rdx, [rel strHello]
    call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]
    add rsp, 64
    
    extern efi_main
    call efi_main
    jmp loopForever
codesize equ $ - $$

; contains nothing - but it is required by UEFI
section .reloc

; contains the data that will be displayed
section .data
    ; this must be a Unicode string
    strHello db __utf16__ `Hello World !\0`

datasize equ $ - $$
