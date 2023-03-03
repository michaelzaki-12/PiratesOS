#include "gnu-efi/inc/efi.h"
#include "gnu-efi/inc/efilib.h"
#include "main.h"
extern EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_STATUS Status;
    EFI_INPUT_KEY Key;
    EFI_BOOT_SERVICES* BS;
    EFI_EXIT_BOOT_SERVICES     gOrigExitBootServices;

    /* Store the system table for future use in other functions */
    ST = SystemTable;
    BS = ST->BootServices;
    /* Say hi */
    BS->ExitBootServices = gOrigExitBootServices;

    /* Get the memory map */
    UINTN MemoryMapSize;
    EFI_MEMORY_DESCRIPTOR *MemoryMap;
    UINTN LocalMapKey;
    UINTN DescriptorSize;
    UINT32 DescriptorVersion;
    MemoryMap = NULL;
    MemoryMapSize = 0;
    Status = BS->GetMemoryMap(&MemoryMapSize, MemoryMap, &LocalMapKey, &DescriptorSize,&DescriptorVersion);
    Status = ST->ConOut->OutputString(ST->ConOut, L"Exit Boot services Didn't Work\n");
    gOrigExitBootServices(ImageHandle, LocalMapKey);

    while(1 == 1);

    return Status;
}
