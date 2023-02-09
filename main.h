#pragma once 
#include "gnu-efi/inc/efi.h"
#include "gnu-efi/inc/efilib.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

void EndLine(EFI_STATUS Status)
{
    Status = ST->ConOut->OutputString(ST->ConOut, (CHAR16*)"\r\n");
}

CHAR16* itoa(int value, CHAR16* result, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    CHAR16* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

static inline void PlotPixel_32bpp(int x, int y, uint32_t pixel)
{
   *((uint32_t*)(gop->Mode->FrameBufferBase + (4 * gop->Mode->Info->PixelsPerScanLine * y) + (4 * x))) = pixel;
}
