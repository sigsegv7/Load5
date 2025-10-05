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

#include <machine/mmu.h>
#include <lfive/mem.h>
#include <cdefs.h>
#include <efi.h>

/* Page table flags */
#define PTE_ADDR_MASK   0x000FFFFFFFFFF000
#define PTE_P           BIT(0)        /* Present */
#define PTE_RW          BIT(1)        /* Writable */
#define PTE_PS          BIT(7)        /* Page size */
#define PTE_NX          BIT(63)       /* Execute-disable */

/* Default page size */
#define PAGE_SIZE 4096

typedef enum {
    PMAP_OFFSET,
    PMAP_TBL,
    PMAP_PD,
    PMAP_PDPT,
    PMAP_PML4
} pmap_lvt_t;

/*
 * Convert MI protection flags to MD
 * page table flags
 */
static uint32_t
prot_to_pte(uint32_t prot)
{
    uint32_t pte_flags = 0;

    if (ISSET(prot, PROT_READ))
        pte_flags |= PTE_P;
    if (ISSET(prot, PROT_WRITE))
        pte_flags |= PTE_RW;

    return pte_flags;
}

/*
 * Invalidate a page in the TLB. We use this to prevent
 * stale entries when remapping or changing attributes
 * of pages.
 *
 * @ptr: Page base to invalidate from the TLB
 */
static inline void
__invlpg(void *ptr)
{
    uintptr_t v = (uintptr_t)ptr;

    v = ALIGN_UP(v, PAGE_SIZE);
    __ASMV(
        "invlpg (%0)"
        :
        : "r" (v)
        : "memory"
    );
}

/*
 * Extract a specific level index from the virtual
 * address
 *
 * @va: Virtual address to extract from
 * @level: Level index to extract
 */
static uintn_t
mmu_level_index(vaddr_t va, pmap_lvt_t level)
{
    switch (level) {
    case PMAP_OFFSET:
        return va & 0xFFF;
    case PMAP_TBL:
        return (va >> 12) & 0x1FF;
    case PMAP_PD:
        return (va >> 21) & 0x1FF;
    case PMAP_PDPT:
        return (va >> 30) & 0x1FF;
    case PMAP_PML4:
        return (va >> 39) & 0x1FF;
    }

    return (uintn_t)-1;
}

/*
 * Extract a specific pagemap level using a virtual
 * address and a specific address space
 *
 * @vas: Current address space
 * @va: Virtual address to map
 * @lvl: Pagemap level to extract
 * @res: Resulting level pointer is written here
 *
 * Returns zero on success
 */
static int
mmu_get_level(struct mmu_vas *vas, vaddr_t va, pmap_lvt_t lvl, uintptr_t **res)
{
    efi_status_t status;
    uintptr_t *cur, tmp_va, addr;
    size_t index;
    pmap_lvt_t cur_level = PMAP_PML4;

    if (vas == NULL || lvl > PMAP_PML4) {
        return -1;
    }

    if (res == NULL) {
        return -1;
    }

    /*
     * We'll do a recursive descent style algorithm
     * to get the page table that we want. Keep going
     * down levels [lvl, MMU_TBL) until we hit the
     * bottom.
     */
    cur = (void *)(vas->pml4 & PTE_ADDR_MASK);
    while (cur_level > lvl) {
        index = mmu_level_index(va, cur_level);
        addr = cur[index];

        /* Is this present? */
        if (ISSET(addr, PTE_P)) {
            cur = (void *)(cur[index] & PTE_ADDR_MASK);
            --cur_level;
            continue;
        }

        /* Allocate new frame */
        status = g_bootsrv->allocate_pages(
            AllocateAnyPages,
            EfiLoaderData,
            1,
            &addr
        );
        if (EFI_ERROR(status)) {
            puts(L"out of memory\r\n");
            die();
        }

        /* Write the new entry */
        cur[index] = (addr | PTE_P | PTE_RW);
        cur = (void *)addr;

        /*
         * To be certain that we will see every change
         * per every level, we must invalidate its
         * corresponding entry.
         */
        __invlpg(cur);
        --cur_level;
    }
    *res = cur;
    return 0;
}

int
mmu_get_vas(struct mmu_vas *res_p)
{
    uint64_t cr3;

    if (res_p == NULL) {
        return -1;
    }

    /* Read the CR3 value */
    __ASMV(
        "mov %%cr3, %0"
        : "=r" (cr3)
        :
        : "memory"
    );

    res_p->pml4 = cr3 & PTE_ADDR_MASK;
    return 0;
}

int
mmu_set_vas(struct mmu_vas *vas)
{
    if (vas == NULL) {
        return -1;
    }

    __ASMV(
        "mov %0, %%cr3"
        :
        : "r" (vas->pml4)
        : "memory"
    );

    return 0;
}

int
mmu_map(struct mmu_vas *vas, vaddr_t va, paddr_t pa, int prot, int size)
{
    uintptr_t *tbl;
    uint32_t pte_flags = 0;
    uintn_t index;
    int error;

    if (vas == NULL) {
        return -1;
    }

    /* Grab the page table */
    switch (size) {
    case MAP_HUGE_2MIB:
        error = mmu_get_level(
            vas,
            va,
            PMAP_PD,
            &tbl
        );

        pte_flags |= PTE_PS;
        index = mmu_level_index(va, PMAP_PD);
        break;
    case MAP_HUGE_1GIB:
        error = mmu_get_level(
            vas,
            va,
            PMAP_PDPT,
            &tbl
        );

        pte_flags |= PTE_PS;
        index = mmu_level_index(va, PMAP_PDPT);
        break;
    default:
        error = mmu_get_level(
            vas,
            va,
            PMAP_TBL,
            &tbl
        );
        index = mmu_level_index(va, PMAP_TBL);
        break;
    }

    if (error != 0) {
        puts(L"mmu_map page table fetch failure\r\n");
        return -1;
    }

    /* Grab the MD flags and PTE index */
    pte_flags |= prot_to_pte(prot);

    /* Create the mapping */
    tbl[index] = pa | pte_flags;
    __invlpg((void *)va);
    return 0;
}

int
mmu_init_vas(uintptr_t pg)
{
    struct mmu_vas vas;
    uint64_t *pml4;
    int error, prot = PROT_READ | PROT_WRITE;

    if (pg == NULL) {
        return -1;
    }

    pml4 = (void *)pg;
    vas.pml4 = pg;

    /*
     * Zero everything initially
     */
    for (int i = 0; i < 512; ++i) {
        pml4[i] = 0;
    }

    /* Identity map the lower 4 GiB */
    for (uintn_t i = 0; i < MEM_1GIB * 4; i += MAP_HUGE_1GIB) {
        error = mmu_map(&vas, i, i, prot, MAP_HUGE_1GIB);
        if (error != 0) {
            puts(L"failed to map lower 4 GiB\r\n");
            die();
        }
    }

    return 0;
}
