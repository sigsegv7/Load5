#include <efi.h>

int
efi_main(void *hand, EFI_SYSTEM_TABLE *systab) {
    systab->con_out->output_string(systab->con_out, (int16_t *)L"Hello, World!");
    for(;;);
    return 0;
}
