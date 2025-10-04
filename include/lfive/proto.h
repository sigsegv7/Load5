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

#ifndef _LFIVE_PROTO_H_
#define _LFIVE_PROTO_H_ 1

#include <stdint.h>

/*
 * Framebuffer information
 *
 * @io: Framebuffer base
 * @pitch: Framebuffer pitch
 * @width: Framebuffer height
 * @height: Framebuffer height
 */
struct l5_fbinfo {
    uint32_t *io;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
};

/*
 * Describes a memory map entry
 *
 * @base: Base address
 * @npages: Number of pages in this region
 */
struct l5_mementry {
    uintptr_t base;
    size_t npages;
};

/*
 * Describes the main L5 protocol handle from where
 * the bootloader can pass information to the OS
 * environment
 */
struct l5_proto {
    struct l5_fbinfo fbinfo;
    struct l5_mementry *memmap;
};

#endif  /* !_LFIVE_PROTO_H_ */
