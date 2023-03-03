CC=clang
CFLAGS=--target=x86_64-unknown-windows -ffreestanding -fshort-wchar -mno-red-zone -Ignu-efi/inc -Ignu-efi/inc/x86_64 -Ignu-efi/inc/protocol
LDFLAGS=--target=x86_64-unknown-windows -nostdlib -Wl,-entry:efi_main -Wl,-subsystem:efi_application -fuse-ld=lld-link -Lgnu-efi/x86_64/lib/libefi.a -Lgnu-efi/x86_64/gnuefi/libgnuefi.a 
NASMLFLAGS=lld-link /subsystem:efi_application /entry:main /out:BOOTX64.EFI
NASM = nasm
AFlags = win64
SRC=$(wildcard *.c)
SRC2=$(wildcard *.asm)
FLAGSRC=$(wildcard *.obj)

OSRC=$(wildcard *.o)


main: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

compile: $(OSRC)
	$(CC) $(LDFLAGS) -o BOOTX64.EFI $(OSRC) $(FLAGSRC)
compiledata:
	clang $(CFLAGS) -c -o data.o gnu-efi/data.c
nasm: $(SRC2)
	$(NASM) -f $(AFlags) $(SRC2)
LinkNasm:
	$(NASMLFLAGS) $(FLAGSRC) $(OSRC)
