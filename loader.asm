; generate 64-bit code
bits 64

; contains the code that will run
section .text
incbin "BOOTX64.bin"

; allows the linker to see this symbol
global main

; see http://www.uefi.org/sites/default/files/resources/UEFI Spec 2_7_A Sept 6.pdf#G8.1001729
struc EFI_TABLE_HEADER
    .Signature    RESQ 1
    .Revision     RESD 1
    .HeaderSize   RESD 1
    .CRC32        RESD 1
    .Reserved     RESD 1
endstruc

; see http://www.uefi.org/sites/default/files/resources/UEFI Spec 2_7_A Sept 6.pdf#G8.1001773
struc EFI_SYSTEM_TABLE
    .Hdr                  RESB EFI_TABLE_HEADER_size
    .FirmwareVendor       RESQ 1
    .FirmwareRevision     RESD 1
    .ConsoleInHandle      RESQ 1
    .ConIn                RESQ 1
    .ConsoleOutHandle     RESQ 1
    .ConOut               RESQ 1
    .StandardErrorHandle  RESQ 1
    .StdErr               RESQ 1
    .RuntimeServices      RESQ 1
    .BootServices         RESQ 1
    .NumberOfTableEntries RESQ 1
    .ConfigurationTable   RESQ 1
endstruc

; see http://www.uefi.org/sites/default/files/resources/UEFI Spec 2_7_A Sept 6.pdf#G16.1016807
struc EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
    .Reset             RESQ 1
    .OutputString      RESQ 1
    .TestString	       RESQ 1
    .QueryMode	       RESQ 1
    .SetMode	       RESQ 1
    .SetAttribute      RESQ 1
    .ClearScreen       RESQ 1
    .SetCursorPosition RESQ 1
    .EnableCursor      RESQ 1
    .Mode              RESQ 1
endstruc

loopForever:
    jmp loopForever

main:
    ; reserve space for 4 arguments
    sub rsp, 4 * 8 + 8

    ; rdx points to the EFI_SYSTEM_TABLE structure
    ; which is the 2nd argument passed to us by the UEFI firmware
    ; adding 64 causes rcx to point to EFI_SYSTEM_TABLE.ConOut
    mov rcx, [rdx + 64]

    ; load the address of our string into rdx
    lea rdx, [rel strHello]

    ; EFI_SYSTEM_TABLE.ConOut points to EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
    ; call OutputString on the value in rdx
    call [rcx + EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL.OutputString]
    
    add rsp, 4*8 + 8
    ; loop forever so we can see the string before the UEFI application exits
    extern efi_main
    call efi_main
    ret
    jmp loopForever

codesize equ $ - $$

; contains nothing - but it is required by UEFI
section .reloc

; contains the data that will be displayed
section .data
    ; this must be a Unicode string
    strHello db __utf16__ `Hello World !\0`

datasize equ $ - $$