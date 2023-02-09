#include "gnu-efi/inc/efi.h"
#include "gnu-efi/inc/efilib.h"
#include "main.h"
extern EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
    EFI_BOOT_SERVICES* BS;

    /* Store the system table for future use in other functions */
    ST = SystemTable;
    BS = ST->BootServices;
    /* Say hi */
    Status = ST->ConOut->OutputString(ST->ConOut, L"Hello World\r\n"); // EFI Applications use Unicode and CRLF, a la Windows
    if (EFI_ERROR(Status))
        return Status;
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
 
    Status = BS->LocateProtocol(&gopGuid, NULL, (void**)&gop);
    if(EFI_ERROR(Status))
      Status = ST->ConOut->OutputString(ST->ConOut, L"Unable To  alocate gop");
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo, numModes, nativeMode;
    
    Status = gop->QueryMode(gop, gop->Mode==NULL?0:gop->Mode->Mode, &SizeOfInfo, &info);
    // this is needed to get the current video mode
    if (Status == EFI_NOT_STARTED)
      Status = gop->SetMode(gop, 0);
    if(EFI_ERROR(Status)) {
      Status = ST->ConOut->OutputString(ST->ConOut, L"Unable to get native mode\n");
    } else {
      nativeMode = gop->Mode->Mode;
      numModes = gop->Mode->MaxMode;
    }
    for (int i = 0; i < numModes; i++) {
      CHAR16 X[33];
      Status = gop->QueryMode(gop, i, &SizeOfInfo, &info);
      Status = ST->ConOut->OutputString(ST->ConOut, L"mode");
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(i, X, 10));
      EndLine(Status);
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(20, X, 10));
      EndLine(Status);
      Status = ST->ConOut->OutputString(ST->ConOut, L"width");
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(info->HorizontalResolution, X, 10));
      EndLine(Status);
      Status = ST->ConOut->OutputString(ST->ConOut, L"Height");
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(info->VerticalResolution, X, 10));
      EndLine(Status);
      Status = ST->ConOut->OutputString(ST->ConOut, L"Format");
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(info->PixelFormat, X, 10));
      EndLine(Status);
      if(i == nativeMode){
          Status = ST->ConOut->OutputString(ST->ConOut, L"(Current)");
      }else
          Status = ST->ConOut->OutputString(ST->ConOut, L"...");


    }

    // SIMPLE_TEXT_OUTPUT_INTERFACE* this;
    Status = gop->SetMode(gop, 0);

    if(EFI_ERROR(Status)) {
        Status = ST->ConOut->OutputString(ST->ConOut, L"Unable to set mode 0");
    }else{
      CHAR16 R[100];
      Status = ST->ConOut->OutputString(ST->ConOut, L"Framebuffer Address");
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(gop->Mode->FrameBufferBase, R, 10));
      EndLine(Status);
      Status = ST->ConOut->OutputString(ST->ConOut, L"Framebuffer Size");
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(gop->Mode->FrameBufferSize, R, 10));
      EndLine(Status);
      Status = ST->ConOut->OutputString(ST->ConOut, L"Width");
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(gop->Mode->Info->HorizontalResolution, R, 10));
      EndLine(Status);
      Status = ST->ConOut->OutputString(ST->ConOut, L"Height");
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(gop->Mode->Info->VerticalResolution, R, 10));
      EndLine(Status);
      Status = ST->ConOut->OutputString(ST->ConOut, L"PixelPerLine");
      Status = ST->ConOut->OutputString(ST->ConOut, itoa(gop->Mode->Info->PixelsPerScanLine, R, 10));
      EndLine(Status);
    }
    
    PlotPixel_32bpp(1920, 1080, (uint32_t)0xFF0000);
    while(1 == 1);

    return Status;
}