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
#include <cdefs.h>
#include <lfive/log.h>
#include <lfive/proto.h>
#include <machine/mmu.h>

#define SEEK_END 0xFFFFFFFFFFFFFFFF

static uintptr_t vas_pg;
static void *kern_img = NULL;
EFI_SYSTEM_TABLE *g_systab;
EFI_BOOT_SERVICES *g_bootsrv;
EFI_GRAPHICS_OUTPUT_PROTOCOL *g_gop;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *g_sfs;
EFI_FILE_PROTOCOL *g_fproto;
struct l5_proto g_lfive;

/*
 * Get the file size
 *
 * @file: File handle
 * @size_res: Result is written here
 *
 * Returns zero on success
 */
static efi_status_t
efi_get_fsize(EFI_FILE_PROTOCOL *file, uintn_t *size_res)
{
    efi_status_t status;
    uintn_t size;

    /* Seek to the end */
    status = file->set_position(
        file,
        SEEK_END
    );

    if (EFI_ERROR(status)) {
        puts(L"could not seek to end of file\n");
        return status;
    }

    /* The position should be the file size */
    status = file->get_position(
        file,
        &size
    );

    if (EFI_ERROR(status)) {
        puts(L"could not get file position\n");
        return status;
    }

    /* Seek to the start */
    status = file->set_position(
        file,
        0
    );

    if (EFI_ERROR(status)) {
        puts(L"could not seek to start of file\n");
        return status;
    }

    *size_res = size;
    return EFI_SUCCESS;
}

/*
 * Load the kernel via EFi
 */
static void
load_kernel(void)
{
    EFI_FILE_PROTOCOL *file;
    uintn_t file_size;
    efi_status_t status;
    void *bin_buf;

    /* Open the kernel image */
    status = g_fproto->open(
        g_fproto,
        &file,
        L"l5",
        EFI_FILE_MODE_READ,
        EFI_FILE_READ_ONLY | EFI_FILE_READ_ONLY | EFI_FILE_SYSTEM
    );

    if (EFI_ERROR(status)) {
        puts(L"could not load kernel\r\n");
        die();
    }

    /* Get the file size */
    status = efi_get_fsize(file, &file_size);
    if (EFI_ERROR(status)) {
        puts(L"get file size\r\n");
        die();
    }

    /* Allocate memory for the kernel */
    status = g_bootsrv->allocate_pool(
        EfiRuntimeServicesData,
        file_size,
        (void **)&kern_img
    );

    if (EFI_ERROR(status)) {
        puts(L"failed to allocate kernel buffer\r\n");
        die();
    }

    /* Load the kernel image into our buffer */
    status = file->read(
        file,
        &file_size,
        kern_img
    );

    if (EFI_ERROR(status)) {
        puts(L"failed to read kernel\r\n");
        die();
    }
}

/*
 * Initialize the EFI file protocol
 *
 * @image: Image handle
 * @res: File protocol
 *
 * Returns zero on success
 */
static int
init_efi_file(efi_handle_t image, EFI_FILE_PROTOCOL **res)
{
    efi_status_t status;
    EFI_LOADED_IMAGE_PROTOCOL *loaded_image;
    EFI_GUID sfs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;

    /* Get the loaded image interface */
    status = g_bootsrv->handle_protocol(
        image,
        &lip_guid,
        (void **)&loaded_image
    );

    if (EFI_ERROR(status)) {
        puts(L"could not handle loaded image protocol\r\n");
        return status;
    }

    /* Grab the I/O volume */
    status = g_bootsrv->handle_protocol(
        loaded_image->device_handle,
        &sfs_guid,
        (void **)&g_sfs
    );

    if (EFI_ERROR(status)) {
        puts(L"could not get I/O image\r\n");
        return status;
    }

    status = g_sfs->open_volume(g_sfs, res);
    if (EFI_ERROR(status)) {
        puts(L"could not open file protocol\r\n");
        return status;
    }

    return 0;
}

/*
 * Get the memory map from firmware
 */
static uintn_t
efi_get_mem(void)
{
    struct l5_mementry *l5_ent;
    EFI_MEMORY_DESCRIPTOR *map, *ent;
    efi_status_t status;
    uintn_t map_key;
    uintn_t map_size, l5_mapsize;
    uintn_t descriptor_size;
    uint32_t descriptor_version;
    uint8_t i = 0;

    /* Grab a hold of the memory map */
    map = NULL;
    map_size = 0;

    /* Get the pool size */
    status = g_bootsrv->get_memory_map(
        &map_size,
        map,
        NULL,
        &descriptor_size,
        NULL
    );

    if (status != EFI_BUFFER_TOO_SMALL) {
        puts(L"could not get memory map size\r\n");
        die();
    }

    /* Allocate a pool for the memory map */
    l5_mapsize = map_size;
    status = g_bootsrv->allocate_pool(
        EfiLoaderData,
        map_size,
        (void **)&g_lfive.memmap
    );

    /* Allocate a pool for the memory map */
    map_size += 2 * descriptor_size;
    status = g_bootsrv->allocate_pool(
        EfiLoaderData,
        map_size,
        (void **)&map
    );
    if (EFI_ERROR(status)) {
        puts(L"could not allocate memory map\r\n");
        die();
    }

    /* Actually get the memory map now */
    status = g_bootsrv->get_memory_map(
        &map_size,
        map,
        &map_key,
        &descriptor_size,
        &descriptor_version
    );
    if (EFI_ERROR(status)) {
        puts(L"could not load memory map\r\n");
        die();
    }

    ent = map;
    do {
        l5_ent = &g_lfive.memmap[i];
        l5_ent->base = ent->physical_start;
        l5_ent->npages = ent->number_of_pages;
        ent = ((void *)((uint8_t *)ent + descriptor_size));
    } while ((uint8_t *)ent < (uint8_t *)map + map_size);
    return map_key;
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

/*
 * Wait for a keystroke to boot the system
 */
static void
wait_key(void)
{
    EFI_INPUT_KEY input;

    /* Flush the console input */
    g_systab->con_in->reset(
        g_systab->con_in,
        0
    );

    for (;;) {
        g_systab->con_in->read_key_stroke(
            g_systab->con_in,
            &input
        );

        if (input.unicode_char == L'\r') {
            break;
        }
    }
}

int
efi_main(efi_handle_t *hand, EFI_SYSTEM_TABLE *systab)
{
    efi_status_t status;
    EFI_FILE_PROTOCOL file;
    uintn_t map_key = 0;
    int error;

    g_systab = systab;
    g_bootsrv = systab->boot_services;

    /* Grab the boot services */
    systab->boot_services->set_watchdog_timer(0, 0, 0, NULL);

    /* Clear the console */
    systab->con_out->reset(
        systab->con_out,
        0
    );

    puts(L"** l5 loader (uefi) **\r\n");
    if ((error = efi_get_gop()) < 0) {
        die();
    }

    /*
     * Initialize the address space, we don't want to
     * switch just yet! But take advantage of the boot
     * services before we exit them
     */
    mmu_init_vas(vas_pg);
    puts(L"** vas initialized\r\n");

    /* Wait for input */
    puts(L"[ press enter to boot ]\r\n");
    wait_key();
    puts(L"** booting...\r\n");

    /* Allocate a virtual address space */
    status = g_bootsrv->allocate_pages(
        AllocateAnyPages,
        EfiLoaderData,
        1,
        &vas_pg
    );

    if (EFI_ERROR(status)) {
        puts(L"failed to allocate VAS\r\n");
        die();
    }

    /* Load the kernel and L5 protocol */
    init_efi_file(hand, &g_fproto);
    load_kernel();
    map_key = efi_get_mem();

    /* Get the heck out of here! */
    status = g_bootsrv->exit_boot_services(
        hand,
        map_key
    );

    /* This would suck */
    if (status != EFI_SUCCESS) {
        puts(L"could not exit EFI boot services\r\n");
        die();
    }

    for (;;);
    return 0;
}
