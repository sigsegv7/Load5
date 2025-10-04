/*
 * Copyright (c) 2025 Ian Marco Moffett and L5 engineers
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <efi.h>
#include <lfive/log.h>
#include <lfive/proto.h>

EFI_SYSTEM_TABLE *g_systab;
EFI_BOOT_SERVICES *g_bootsrv;
EFI_GRAPHICS_OUTPUT_PROTOCOL *g_gop;
struct l5_proto g_lfive;

/*
 * Used on panics
 */
static void
die(void)
{
    puts(L"\r\n!! l5 panic !!\r\n");
    for (;;);
}

/*
 * Get the memory map from firmware
 */
static void
efi_get_mem(void)
{
    struct l5_mementry *l5_ent;
    EFI_MEMORY_DESCRIPTOR *map, *ent;
    efi_status_t status;
    uintn_t map_key;
    uintn_t map_size;
    uintn_t descriptor_size;
    uint32_t descriptor_version;
    uint8_t i = 0;

    /* Grab a hold of the memory map */
    map = NULL;
    map_size = 0;

#define get_memory_map()                        \
        g_bootsrv->get_memory_map(              \
            &map_size,                          \
            map,                                \
            &map_key,                           \
            &descriptor_size,                   \
            &descriptor_version                 \
        );

    status = get_memory_map();
    if (status != EFI_BUFFER_TOO_SMALL) {
        puts(L"could not get memory map size\r\n");
        die();
    }

    /* Allocate a pool for the memory map */
    status = g_bootsrv->allocate_pool(
        EfiLoaderData,
        map_size + 2 * descriptor_size,
        (void **)&map
    );
    if (EFI_ERROR(status)) {
        puts(L"could not allocate memory map\r\n");
        die();
    }

    /*
     * Some firmware is stupid so we have to call this
     * twice so it would work.
     */
    get_memory_map();
    status = get_memory_map();
    if (status == EFI_BUFFER_TOO_SMALL) {
        puts(L"could not load memory map\r\n");
        die();
    }
#undef get_memory_map

    /* Allocate some memory for the protocol */
    status = g_bootsrv->allocate_pool(
        EfiRuntimeServicesData,
        map_size + 2 * sizeof(struct l5_mementry),
        (void **)&g_lfive.memmap
    );

    if (EFI_ERROR(status)) {
        puts(L"failed to alloc protocol memmap\r\n");
        die();
    }

    puts(L"populating protocol memory map... ");
    ent = map;
    do {
        l5_ent = &g_lfive.memmap[i];
        l5_ent->base = ent->physical_start;
        l5_ent->npages = ent->number_of_pages;
        ent = ((void *)((uint8_t *)ent + descriptor_size));
    } while ((uint8_t *)ent < (uint8_t *)map + map_size);
    puts(L"OK\r\n");
}

/*
 * Acquire the handle for graphics output protocol
 *
 * Returns zero on success, otherwise a less than zero
 * value on failure.
 */
static int
efi_get_gop(void)
{
    struct l5_fbinfo *fbinfo;
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    efi_status_t status;

    /* Get the graphics output protocol */
    status = g_bootsrv->locate_protocol(&gopGuid, NULL, (void **)&gop);
    if (EFI_ERROR(status)) {
        puts(L"could not get graphics handle!\r\n");
    }

    fbinfo = &g_lfive.fbinfo;
    fbinfo->pitch = gop->mode->info->pixels_per_scan_line;
    fbinfo->width = gop->mode->info->horizontal_resolution;
    fbinfo->height = gop->mode->info->vertical_resolution;
    return 0;
}

int
efi_main(void *hand, EFI_SYSTEM_TABLE *systab)
{
    int error;

    g_systab = systab;
    g_bootsrv = systab->boot_services;

    /* Grab the boot services */
    systab->boot_services->set_watchdog_timer(0, 0, 0, NULL);
    puts(L"l5 loader - uefi\r\n");
    if ((error = efi_get_gop()) < 0) {
        die();
    }

    efi_get_mem();
    for (;;);
    return 0;
}
