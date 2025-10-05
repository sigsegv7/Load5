/*
 * Copyright (c) 2023-2025 Ian Marco Moffett and the Osmora Team.
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
 * 3. Neither the name of Hyra nor the names of its
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

#ifndef _EFI_H_
#define _EFI_H_ 1

// PE32+ Subsystem type for EFI images
#define EFI_IMAGE_SUBSYSTEM_EFI_APPLICATION         10
#define EFI_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER 11
#define EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER      12

// PE32+ Machine type for EFI images
#define EFI_IMAGE_MACHINE_IA32                      0x014c
#define EFI_IMAGE_MACHINE_IA64                      0x0200
#define EFI_IMAGE_MACHINE_EBC                       0x0EBC
#define EFI_IMAGE_MACHINE_x64                       0x8664
#define EFI_IMAGE_MACHINE_ARMTHUMB_MIXED            0x01C2
#define EFI_IMAGE_MACHINE_AARCH64                   0xAA64
#define EFI_IMAGE_MACHINE_RISCV32                   0x5032
#define EFI_IMAGE_MACHINE_RISCV64                   0x5064
#define EFI_IMAGE_MACHINE_RISCV128                  0x5128

/******************/
/*  Status_Codes  */
/******************/
#define EFI_SUCCESS                 0x0000000000000000

#define EFI_WARN_UNKNOWN_GLYPH      0x0000000000000001
#define EFI_WARN_DELETE_FAILURE     0x0000000000000002
#define EFI_WARN_WRITE_FAILURE      0x0000000000000003
#define EFI_WARN_BUFFER_TOO_SMALL   0x0000000000000004
#define EFI_WARN_STALE_DATA         0x0000000000000005
#define EFI_WARN_FILE_SYSTEM        0x0000000000000006
#define EFI_WARN_RESET_REQUIRED     0x0000000000000007

#define EFI_LOAD_ERROR              0x8000000000000001
#define EFI_INVALID_PARAMETER       0x8000000000000002
#define EFI_UNSUPPORTED             0x8000000000000003
#define EFI_BAD_BUFFER_SIZE         0x8000000000000004
#define EFI_BUFFER_TOO_SMALL        0x8000000000000005
#define EFI_NOT_READY               0x8000000000000006
#define EFI_DEVICE_ERROR            0x8000000000000007
#define EFI_WRITE_PROTECTED         0x8000000000000008
#define EFI_OUT_OF_RESOURCES        0x8000000000000009
#define EFI_VOLUME_CORRUPTED        0x800000000000000a
#define EFI_VOLUME_FULL             0x800000000000000b
#define EFI_NO_MEDIA                0x800000000000000c
#define EFI_MEDIA_CHANGED           0x800000000000000d
#define EFI_NOT_FOUND               0x800000000000000e
#define EFI_ACCESS_DENIED           0x800000000000000f
#define EFI_NO_RESPONSE             0x8000000000000010
#define EFI_NO_MAPPING              0x8000000000000011
#define EFI_TIMEOUT                 0x8000000000000012
#define EFI_NOT_STARTED             0x8000000000000013
#define EFI_ALREADY_STARTED         0x8000000000000014
#define EFI_ABORTED                 0x8000000000000015
#define EFI_ICMP_ERROR              0x8000000000000016
#define EFI_TFTP_ERROR              0x8000000000000017
#define EFI_PROTOCOL_ERROR          0x8000000000000018
#define EFI_INCOMPATIBLE_VERSION    0x8000000000000019
#define EFI_SECURITY_VIOLATION      0x800000000000001a
#define EFI_CRC_ERROR               0x800000000000001b
#define EFI_END_OF_MEDIA            0x800000000000001c
#define EFI_END_OF_FILE             0x800000000000001f
#define EFI_INVALID_LANGUAGE        0x8000000000000020
#define EFI_COMPROMISED_DATA        0x8000000000000021
#define EFI_IP_ADDRESS_CONFLICT     0x8000000000000022
#define EFI_HTTP_ERROR              0x8000000000000023

#define EFI_ERROR(status) ((status) & 0x8000000000000000)

#include <stdint.h>

// GCC( and clang) has support for 128 bit integers,
//      for other compilers don't define them;
#ifdef __GNUC__
typedef unsigned __int128 uint128_t;
typedef __int128 int128_t;
#endif // __GNUC__

typedef uint8_t BOOLEAN;
#define TRUE  1
#define FALSE 0

#define NULL 0

typedef uintmax_t uintn_t;
typedef intmax_t  intn_t;

typedef uintn_t efi_status_t;

typedef void *efi_handle_t;
typedef void *efi_event_t;
typedef uint64_t efi_lba_t;
typedef uintn_t  efi_tpl_t;

typedef uint64_t efi_phys_addr_t;

typedef uint8_t efi_mac_addr_t[4];
typedef uint8_t efi_ipv4_addr_t[4];
typedef uint8_t efi_ipv6_address_t[16];
typedef uint8_t __attribute__ ((aligned(4))) EFI_IP_ADDRESS[16];
typedef uintn_t efi_tpl_t;

#define IN
#define OUT
#define OPTIONAL
#define CONST const

// Only true for x64(as far as I know)
#define __efiapi __attribute__((ms_abi))

/*******************/
/*  Boot_Services  */
/*******************/

//
// CreateEvent
//
typedef void *EFI_EVENT;
// These types can be “ORed” together as needed – for example,
// EVT_TIMER might be “ORed” with EVT_NOTIFY_WAIT or
// EVT_NOTIFY_SIGNAL. For actual definitions check the specification.
#define EVT_TIMER                          0x80000000
#define EVT_RUNTIME                        0x40000000
#define EVT_NOTIFY_WAIT                    0x00000100

#define EVT_NOTIFY_SIGNAL                  0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES      0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE  0x60000202

typedef
    void
    (__efiapi *EFI_EVENT_NOTIFY) (
        IN EFI_EVENT Event,
        IN void    *Context
        );

typedef
    efi_status_t
    (__efiapi *EFI_CREATE_EVENT) (
        IN uint32_t             Type,
        IN efi_tpl_t            NotifyTpl,
        IN EFI_EVENT_NOTIFY   NotifyFunction OPTIONAL,
        IN void               *NotifyContext OPTIONAL,
        OUT EFI_EVENT         *Event
        );


//
// CreateEventEx
//
typedef struct {
    uint32_t data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t  data4[8];
} EFI_GUID;
typedef
    efi_status_t
    (__efiapi *EFI_CREATE_EVENT_EX) (
        IN uint32_t    Type,
        IN efi_tpl_t NotifyTpl,
        IN EFI_EVENT_NOTIFY   NotifyFunction OPTIONAL,
        IN CONST void        *NotifyContext OPTIONAL,
        IN CONST EFI_GUID    *EventGroup OPTIONAL,
        OUT EFI_EVENT        *Event
        );
#define EFI_EVENT_GROUP_EXIT_BOOT_SERVICES \
    {0x27abf055, 0xb1b8, 0x4c26,\
    {0x80,0x48,0x74,0x8f,0x37,0xba,0xa2,0xdf}}

#define EFI_EVENT_GROUP_VIRTUAL_ADDRESS_CHANGE \
    {0x13fa7698,0xc831,0x49c7,\
    {0x87,0xea,0x8f,0x43,0xfc,0xc2,0x51,0x96}}

#define EFI_EVENT_GROUP_MEMORY_MAP_CHANGE \
    {0x78bee926, 0x692f, 0x48fd,\
    {0x9e,0xdb,0x1,0x42,0x2e,0xf0,0xd7,0xab}}

#define EFI_EVENT_GROUP_READY_TO_BOOT \
    {0x7ce88fb3, 0x4bd7, 0x4679,\
    {0x87,0xa8,0xa8,0xd8,0xde,0xe5,0xd,0x2b}}

#define EFI_EVENT_GROUP_RESET_SYSTEM \
    {0x62da6a56,0x13fb,0x485a,\
    {0xa8,0xda,0xa3,0xdd,0x79,0x12,0xcb,0x6b}}


//
// CloseEvent
//
typedef
    efi_status_t
    (__efiapi *EFI_CLOSE_EVENT) (
        IN EFI_EVENT Event
        );



//
// SignalEvent
//
typedef
    efi_status_t
    (__efiapi *EFI_SIGNAL_EVENT) (
        IN EFI_EVENT Event
        );


//
// WaitForEvent
//
typedef
    efi_status_t
    (__efiapi *EFI_WAIT_FOR_EVENT) (
        IN uintn_t       NumberOfEvents,
        IN EFI_EVENT   *Event,
        OUT uintn_t      *Index
        );



//
// CheckEvent
//
typedef
    efi_status_t
    (__efiapi *EFI_CHECK_EVENT) (
        IN EFI_EVENT Event
        );


//
// SetTimer
//
typedef enum {
    TimerCancel,
     TimerPeriodic,
     TimerRelative
} EFI_TIMER_DELAY;
typedef
    efi_status_t
    (__efiapi *EFI_SET_TIMER) (
        IN EFI_EVENT           Event,
        IN EFI_TIMER_DELAY     Type,
        IN uint64_t              TriggerTime
        );

//
// RaiseTPL
//
typedef
    efi_tpl_t
    (__efiapi *EFI_RAISE_TPL) (
        IN efi_tpl_t NewTpl
        );
typedef uintn_t efi_tpl_t;
#define TPL_APPLICATION    4
#define TPL_CALLBACK       8
#define TPL_NOTIFY         16
#define TPL_HIGH_LEVEL     31


//
// RestoreTPL
//
typedef
    void
    (__efiapi *EFI_RESTORE_TPL) (
        IN efi_tpl_t OldTpl
        );


//
// AllocatePages
//
typedef enum {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} EFI_ALLOCATE_TYPE;
typedef enum {
    EfiReservedMemoryType = 0,
    EfiLoaderCode = 1,
    EfiLoaderData = 2,
    EfiBootServicesCode = 3,
    EfiBootServicesData = 4,
    EfiRuntimeServicesCode = 5,
    EfiRuntimeServicesData = 6,
    EfiConventionalMemory = 7,
    EfiUnusableMemory = 8,
    EfiACPIReclaimMemory = 9,
    EfiACPIMemoryNVS = 10,
    EfiMemoryMappedIO = 11,
    EfiMemoryMappedIOPortSpace = 12,
    EfiPalCode = 13,
    EfiPersistentMemory = 14,
    EfiMaxMemoryType = 15
} EFI_MEMORY_TYPE;
typedef
    efi_status_t
    (__efiapi *EFI_ALLOCATE_PAGES) (
        IN EFI_ALLOCATE_TYPE        type,
        IN EFI_MEMORY_TYPE          memory_type,
        IN uintn_t                    pages,
        IN OUT efi_phys_addr_t *memory
        );

//
// FreePages
//
typedef
    efi_status_t
    (__efiapi *EFI_FREE_PAGES) (
        IN efi_phys_addr_t memory,
        IN uintn_t         pages);

//
// GetMemoryMap
//
typedef uint64_t  EFI_VIRTUAL_ADDRESS;
typedef struct {
    uint32_t        type;
    efi_phys_addr_t   physical_start;
    EFI_VIRTUAL_ADDRESS    virtual_start;
    uint64_t                 number_of_pages;
    uint64_t                 attribute;
} EFI_MEMORY_DESCRIPTOR;
typedef
    efi_status_t
    (__efiapi *EFI_GET_MEMORY_MAP) (
        IN OUT uintn_t                 *memory_map_size,
        IN OUT EFI_MEMORY_DESCRIPTOR *memory_map,
        OUT uintn_t                    *map_key,
        OUT uintn_t                    *descriptor_size,
        OUT uint32_t                   *descriptor_version
        );
#define EFI_MEMORY_UC            0x0000000000000001
#define EFI_MEMORY_WC            0x0000000000000002
#define EFI_MEMORY_WT            0x0000000000000004
#define EFI_MEMORY_WB            0x0000000000000008
#define EFI_MEMORY_UCE           0x0000000000000010
#define EFI_MEMORY_WP            0x0000000000001000
#define EFI_MEMORY_RP            0x0000000000002000
#define EFI_MEMORY_XP            0x0000000000004000
#define EFI_MEMORY_NV            0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_RO            0x0000000000020000
#define EFI_MEMORY_SP            0x0000000000040000
#define EFI_MEMORY_CPU_CRYPTO    0x0000000000080000
#define EFI_MEMORY_RUNTIME       0x8000000000000000
#define EFI_MEMORY_DESCRIPTOR_VERSION 1

//
// AllocatePool
//
typedef
    efi_status_t
    (__efiapi *EFI_ALLOCATE_POOL) (
        IN EFI_MEMORY_TYPE   PoolType,
        IN uintn_t             Size,
        OUT void             **Buffer
        );

//
// FreePool
//
typedef
    efi_status_t
    (__efiapi *EFI_FREE_POOL) (
        IN void   *Buffer
        );

//
// InstallProtocolInterface
//
typedef enum {
    EFI_NATIVE_INTERFACE
} EFI_INTERFACE_TYPE;
typedef
    efi_status_t
    (__efiapi *EFI_INSTALL_PROTOCOL_INTERFACE) (
        IN OUT efi_handle_t     *Handle,
        IN EFI_GUID           *Protocol,
        IN EFI_INTERFACE_TYPE InterfaceType,
        IN void               *Interface
        );

//
// UninstallProtocolInterface
//
typedef
    efi_status_t
    (__efiapi *EFI_UNINSTALL_PROTOCOL_INTERFACE) (
        IN efi_handle_t    Handle,
        IN EFI_GUID      *Protocol,
        IN void          *Interface
        );

//
// ReinstallProtocolInterface
//
typedef
    efi_status_t
    (__efiapi *EFI_REINSTALL_PROTOCOL_INTERFACE) (
        IN efi_handle_t Handle,
        IN EFI_GUID   *Protocol,
        IN void       *OldInterface,
        IN void       *NewInterface
        );

//
// RegisterProtocolNotify
//
typedef
    efi_status_t
    (__efiapi *EFI_REGISTER_PROTOCOL_NOTIFY) (
        IN EFI_GUID  *Protocol,
        IN efi_event_t Event,
        OUT void   **Registration
        );

//
// LocateHandle
//
typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;
typedef
    efi_status_t
    (__efiapi *EFI_LOCATE_HANDLE) (
        IN EFI_LOCATE_SEARCH_TYPE  SearchType,
        IN EFI_GUID                *Protocol OPTIONAL,
        IN void                    *SearchKey OPTIONAL,
        IN OUT uintn_t               *BufferSize,
        OUT efi_handle_t             *Buffer
        );

//
// HandleProtocol
//
typedef
    efi_status_t
    (__efiapi *efi_handle_t_PROTOCOL) (
        IN efi_handle_t    Handle,
        IN EFI_GUID      *Protocol,
        OUT void         **Interface
        );

//
// LocateDevicePath
//
typedef struct {
    uint8_t type;
    uint8_t sub_type;
    uint8_t length[2];
} EFI_DEVICE_PATH_PROTOCOL;
typedef
    efi_status_t
    (__efiapi *EFI_LOCATE_DEVICE_PATH) (
        IN EFI_GUID                      *Protocol,
        IN OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
        OUT efi_handle_t                   *Device
        );

//
// OpenProtocol
//
typedef
    efi_status_t
    (__efiapi *EFI_OPEN_PROTOCOL) (
        IN efi_handle_t      Handle,
        IN EFI_GUID        *Protocol,
        OUT void           **InterfaceOPTIONAL,
        IN efi_handle_t      AgentHandle,
        IN efi_handle_t      ControllerHandle,
        IN uint32_t          Attributes
        );
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL        0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL       0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER 0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER           0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE           0x00000020

//
// CloseProtocol
//
typedef
    efi_status_t
    (__efiapi *EFI_CLOSE_PROTOCOL) (
        IN efi_handle_t      Handle,
        IN EFI_GUID        *Protocol,
        IN efi_handle_t      AgentHandle,
        IN efi_handle_t      ControllerHandle
        );

//
// OpenProtocolInformation
//
typedef struct {
    efi_handle_t       agent_handle;
    efi_handle_t       controller_handle;
    uint32_t           attributes;
    uint32_t           open_count;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;
typedef
    efi_status_t
    (__efiapi *EFI_OPEN_PROTOCOL_INFORMATION) (
        IN efi_handle_t                            Handle,
        IN EFI_GUID                              *Protocol,
        OUT EFI_OPEN_PROTOCOL_INFORMATION_ENTRY  **EntryBuffer,
        OUT uintn_t                                *EntryCount
        );

//
// ConnectController
//
typedef
    efi_status_t
    (__efiapi *EFI_CONNECT_CONTROLLER) (
        IN efi_handle_t                ControllerHandle,
        IN efi_handle_t                *DriverImageHandle  OPTIONAL,
        IN EFI_DEVICE_PATH_PROTOCOL  *RemainingDevicePath OPTIONAL,
        IN BOOLEAN                   Recursive
        );

//
// DisconnectController
//
typedef
    efi_status_t
    (__efiapi *EFI_DISCONNECT_CONTROLLER) (
        IN efi_handle_t ControllerHandle,
        IN efi_handle_t DriverImageHandle OPTIONAL,
        IN efi_handle_t ChildHandle    OPTIONAL
        );

//
// ProtocolsPerHandle
//
typedef
    efi_status_t
    (__efiapi *EFI_PROTOCOLS_PER_HANDLE) (
        IN efi_handle_t Handle,
        OUT EFI_GUID  ***ProtocolBuffer,
        OUT uintn_t     *ProtocolBufferCount
        );

//
// LocateHandleBuffer
//
typedef
    efi_status_t
    (__efiapi *EFI_LOCATE_HANDLE_BUFFER) (
        IN EFI_LOCATE_SEARCH_TYPE  SearchType,
        IN EFI_GUID                *Protocol OPTIONAL,
        IN void                    *SearchKeyOPTIONAL,
        IN OUT uintn_t               *NoHandles,
        OUT efi_handle_t             **Buffer
        );

//
// LocateProtocol
//
typedef
    efi_status_t
    (__efiapi *EFI_LOCATE_PROTOCOL) (
        IN EFI_GUID     *Protocol,
        IN void         *Registration OPTIONAL,
        OUT void        **Interface
        );

//
// InstallMultipleProtocolInterfaces
//
typedef
    efi_status_t
    (__efiapi *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
        IN OUT efi_handle_t *Handle,
        ...
        );

//
// UninstallMultipleProtocolInterfaces
//
typedef
    efi_status_t
    (__efiapi *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
        IN efi_handle_t Handle,
        ...
        );

//
// LoadImage
//
typedef
    efi_status_t
    (__efiapi *EFI_IMAGE_LOAD) (
        IN BOOLEAN                    BootPolicy,
        IN efi_handle_t                 ParentImageHandle,
        IN EFI_DEVICE_PATH_PROTOCOL   *DevicePath,
        IN void                       *SourceBuffer OPTIONAL,
        IN uintn_t                      SourceSize,
        OUT efi_handle_t                *ImageHandle
        );

//
// StartImage
//
typedef
    efi_status_t
    (__efiapi *EFI_IMAGE_START) (
        IN efi_handle_t       ImageHandle,
        OUT uintn_t           *ExitDataSize,
        OUT uint16_t          **ExitData OPTIONAL
        );

//
// UnloadImage
//
typedef
    efi_status_t
    (__efiapi *EFI_IMAGE_UNLOAD) (
        IN efi_handle_t       ImageHandle
        );

//
// Exit
//
typedef
    efi_status_t
    (__efiapi *EFI_EXIT) (
        IN efi_handle_t       ImageHandle,
        IN efi_status_t       ExitStatus,
        IN uintn_t            ExitDataSize,
        IN uint16_t           *ExitData OPTIONAL
        );

//
// ExitBootServices
//
typedef
    efi_status_t
    (__efiapi *EFI_EXIT_BOOT_SERVICES) (
        IN efi_handle_t   ImageHandle,
        IN uintn_t        MapKey
        );

//
// SetWatchdogTimer
//
typedef
    efi_status_t
    (__efiapi *EFI_SET_WATCHDOG_TIMER) (
        IN uintn_t        Timeout,
        IN uint64_t       WatchdogCode,
        IN uintn_t        DataSize,
        IN uint16_t       *WatchdogData  OPTIONAL
        );

//
// Stall
//
typedef
    efi_status_t
    (__efiapi *EFI_STALL) (
        IN uintn_t  Microseconds
        );

//
// CopyMem
//
typedef
    void
    (__efiapi *EFI_COPY_MEM) (
        IN void   *Destination,
        IN void   *Source,
        IN uintn_t  Length
        );

//
// SetMem
//
typedef
    void
    (__efiapi *EFI_SET_MEM) (
        IN void   *Buffer,
        IN uintn_t  Size,
        IN uint8_t  Value
        );

//
// GetNextMonotonicCount
//
typedef
    efi_status_t
    (__efiapi *EFI_GET_NEXT_MONOTONIC_COUNT) (
        OUT uint64_t  *Count
        );

//
// InstallConfigurationTable
//
typedef
    efi_status_t
    (__efiapi *EFI_INSTALL_CONFIGURATION_TABLE) (
        IN EFI_GUID  *Guid,
        IN void      *Table
        );

//
// CalculateCrc32
//
typedef
    efi_status_t
    (__efiapi *EFI_CALCULATE_CRC32) (
        IN void     *Data,
        IN uintn_t    DataSize,
        OUT uint32_t  *Crc32
        );

/**********************/
/*  Runtime_Services  */
/**********************/
#define EFI_RT_SUPPORTED_GET_TIME                       0x0001
#define EFI_RT_SUPPORTED_SET_TIME                       0x0002
#define EFI_RT_SUPPORTED_GET_WAKEUP_TIME                0x0004
#define EFI_RT_SUPPORTED_SET_WAKEUP_TIME                0x0008
#define EFI_RT_SUPPORTED_GET_VARIABLE                   0x0010
#define EFI_RT_SUPPORTED_GET_NEXT_VARIABLE_NAME         0x0020
#define EFI_RT_SUPPORTED_SET_VARIABLE                   0x0040
#define EFI_RT_SUPPORTED_SET_VIRTUAL_ADDRESS_MAP        0x0080
#define EFI_RT_SUPPORTED_CONVERT_POINTER                0x0100
#define EFI_RT_SUPPORTED_GET_NEXT_HIGH_MONOTONIC_COUNT  0x0200
#define EFI_RT_SUPPORTED_RESET_SYSTEM                   0x0400
#define EFI_RT_SUPPORTED_UPDATE_CAPSULE                 0x0800
#define EFI_RT_SUPPORTED_QUERY_CAPSULE_CAPABILITIES     0x1000
#define EFI_RT_SUPPORTED_QUERY_VARIABLE_INFO            0x2000

//
// GetVariable
//
typedef
    efi_status_t
    (__efiapi *EFI_GET_VARIABLE) (
        IN uint16_t*VariableName,
        IN EFI_GUID*VendorGuid,
        OUT uint32_t*Attributes OPTIONAL,
        IN OUT uintn_t*DataSize,
        OUT void*Data OPTIONAL
        );
#define EFI_VARIABLE_NON_VOLATILE                            0x00000001
#define EFI_VARIABLE_BOOTSERVICE_ACCESS                      0x00000002
#define EFI_VARIABLE_RUNTIME_ACCESS                          0x00000004
#define EFI_VARIABLE_HARDWARE_ERROR_RECORD                   0x00000008
#define EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS              0x00000010
// NOTE: EFI_VARIABLE_AUTHENTICATED_WRITE_ACCESS is deprecated
// and should be considered reserved.
#define EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS   0x00000020
#define EFI_VARIABLE_APPEND_WRITE                            0x00000040
#define EFI_VARIABLE_ENHANCED_AUTHENTICATED_ACCESS           0x00000080
#define EFI_VARIABLE_AUTHENTICATION_3_CERT_ID_SHA256 1
typedef struct {
    uint8_t         type;
    uint32_t        id_size;
    // uint8_t      Id[IdSize];
} EFI_VARIABLE_AUTHENTICATION_3_CERT_ID;

//
// GetNextVariableName
//
typedef
    efi_status_t
    (__efiapi *EFI_GET_NEXT_VARIABLE_NAME) (
        IN OUT uintn_t      *VariableNameSize,
        IN OUT uint16_t     *VariableName,
        IN OUT EFI_GUID   *VendorGuid
        );

//
// SetVariable
//
typedef
    efi_status_t
    (__efiapi *EFI_SET_VARIABLE) (
        IN uint16_t      *VariableName,
        IN EFI_GUID    *VendorGuid,
        IN uint32_t      Attributes,
        IN uintn_t       DataSize,
        IN void        *Data
        );

//
// QueryVariableInfo
//
typedef
    efi_status_t
    (__efiapi *EFI_QUERY_VARIABLE_INFO) (
        IN uint32_t     Attributes,
        OUT uint64_t    *MaximumVariableStorageSize,
        OUT uint64_t    *RemainingVariableStorageSize,
        OUT uint64_t    *MaximumVariableSize
        );

//
// GetTime
//
// This represents the current time information
typedef struct {
    uint16_t   year;              // 1900 – 9999
    uint8_t   month;              // 1 – 12
    uint8_t   day;                // 1 – 31
    uint8_t   hour;               // 0 – 23
    uint8_t   minute;             // 0 – 59
    uint8_t   second;             // 0 – 59
    uint8_t   pad1; uint32_t   nanosecond;  // 0 – 999,999,999
    int16_t   timezone;           // -1440 to 1440 or 2047
    uint8_t   daylist; uint8_t   pad2;
} EFI_TIME;
// Bit Definitions for EFI_TIME.Daylight
#define EFI_TIME_ADJUST_DAYLIGHT  0x01
#define EFI_TIME_IN_DAYLIGHT      0x02
// Value Definition for EFI_TIME.TimeZone
#define EFI_UNSPECIFIED_TIMEZONE  0x07FF
// This provides the capabilities of the
// real time clock device as exposed through the EFI interfaces.
typedef struct {
    uint32_t   resolution;
    uint32_t   accuracy;
    BOOLEAN  sets_to_zero;
} EFI_TIME_CAPABILITIES;
typedef
    efi_status_t
    (__efiapi *EFI_GET_TIME) (
        OUT EFI_TIME              *Time,
        OUT EFI_TIME_CAPABILITIES *Capabilities OPTIONAL
        );

//
// SetTime
//
typedef
    efi_status_t
    (__efiapi *EFI_SET_TIME) (
        IN EFI_TIME  *Time
        );

//
// GetWakeupTime
//
typedef
    efi_status_t
    (__efiapi *EFI_GET_WAKEUP_TIME) (
        OUT BOOLEAN  *Enabled,
        OUT BOOLEAN  *Pending,
        OUT EFI_TIME *Time
        );

//
// SetWakeupTime
//
typedef
    efi_status_t
    (__efiapi *EFI_SET_WAKEUP_TIME) (
        IN BOOLEAN  Enable,
        IN EFI_TIME *Time OPTIONAL
        );

//
// SetVirtualAddressMap
//
typedef
    efi_status_t
    (__efiapi *EFI_SET_VIRTUAL_ADDRESS_MAP) (
        IN uintn_t                  MemoryMapSize,
        IN uintn_t                  DescriptorSize,
        IN uint32_t                 DescriptorVersion,
        IN EFI_MEMORY_DESCRIPTOR *VirtualMap
        );

//
// ConvertPointer
//
typedef
    efi_status_t
    (__efiapi *EFI_CONVERT_POINTER) (
        IN uintn_t   DebugDisposition,
        IN void    **Address
        );
#define EFI_OPTIONAL_PTR      0x00000001

//
// ResetSystem
//
typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;
typedef
    void
    (__efiapi *EFI_RESET_SYSTEM) (
        IN EFI_RESET_TYPE ResetType,
        IN efi_status_t     ResetStatus,
        IN uintn_t          DataSize,
        IN void           *ResetData OPTIONAL
        );

//
// GetNextHighMonotonicCount
//
typedef
    efi_status_t
    (__efiapi *EFI_GET_NEXT_HIGH_MONOTONIC_COUNT) (
        OUT uint32_t  *HighCount
        );

//
// UpdateCapsule
//
typedef struct {
    uint64_t                   length;
    union {
        efi_phys_addr_t   data_block;
        efi_phys_addr_t   continuation_pointer;
    } Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;
typedef struct {
    EFI_GUID   capsule_guid;
    uint32_t     header_size;
    uint32_t     flags;
    uint32_t     capsule_image_size;
} EFI_CAPSULE_HEADER;
typedef
    efi_status_t
    (__efiapi *EFI_UPDATE_CAPSULE) (
        IN EFI_CAPSULE_HEADER   **CapsuleHeaderArray,
        IN uintn_t                CapsuleCount,
        IN efi_phys_addr_t ScatterGatherList OPTIONAL
        );

//
// QueryCapsuleCapabilities
//
typedef
    efi_status_t
    (__efiapi *EFI_QUERY_CAPSULE_CAPABILITIES) (
        IN EFI_CAPSULE_HEADER    **CapsuleHeaderArray,
        IN uintn_t                 CapsuleCount,
        OUT uint64_t               *MaximumCapsuleSize,
        OUT EFI_RESET_TYPE       *ResetType
        );

/***********************/
/*  Simple_Text_Stuff  */
/***********************/

//
//
// Input stuff
//
//
#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID \
    {0x387477c1,0x69c7,0x11d2,\
    {0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

//
// InputReset
//
struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef
    efi_status_t
    (__efiapi *EFI_INPUT_RESET) (
        IN struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *This,
        IN BOOLEAN                         ExtendedVerification
        );

//
// ReadKeyStroke
//
typedef struct {
    uint16_t   scan_code;
    uint16_t   unicode_char;
} EFI_INPUT_KEY;
typedef
    efi_status_t
    (__efiapi *EFI_INPUT_READ_KEY) (
        IN struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL *This,
        OUT EFI_INPUT_KEY                 *Key
        );

typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_INPUT_RESET     reset;
    EFI_INPUT_READ_KEY  read_key_stroke;
    efi_event_t           wait_for_key;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

//
//
// Output stuff
//
//

//*******************************************************
// UNICODE DRAWING CHARACTERS
//*******************************************************
#define BOXDRAW_HORIZONTAL                 0x2500
#define BOXDRAW_VERTICAL                   0x2502
#define BOXDRAW_DOWN_RIGHT                 0x250c
#define BOXDRAW_DOWN_LEFT                  0x2510
#define BOXDRAW_UP_RIGHT                   0x2514
#define BOXDRAW_UP_LEFT                    0x2518
#define BOXDRAW_VERTICAL_RIGHT             0x251c
#define BOXDRAW_VERTICAL_LEFT              0x2524
#define BOXDRAW_DOWN_HORIZONTAL            0x252c
#define BOXDRAW_UP_HORIZONTAL              0x2534
#define BOXDRAW_VERTICAL_HORIZONTAL        0x253c
#define BOXDRAW_DOUBLE_HORIZONTAL          0x2550
#define BOXDRAW_DOUBLE_VERTICAL            0x2551
#define BOXDRAW_DOWN_RIGHT_DOUBLE          0x2552
#define BOXDRAW_DOWN_DOUBLE_RIGHT          0x2553
#define BOXDRAW_DOUBLE_DOWN_RIGHT          0x2554
#define BOXDRAW_DOWN_LEFT_DOUBLE           0x2555
#define BOXDRAW_DOWN_DOUBLE_LEFT           0x2556
#define BOXDRAW_DOUBLE_DOWN_LEFT           0x2557
#define BOXDRAW_UP_RIGHT_DOUBLE            0x2558
#define BOXDRAW_UP_DOUBLE_RIGHT            0x2559
#define BOXDRAW_DOUBLE_UP_RIGHT            0x255a
#define BOXDRAW_UP_LEFT_DOUBLE             0x255b
#define BOXDRAW_UP_DOUBLE_LEFT             0x255c
#define BOXDRAW_DOUBLE_UP_LEFT             0x255d
#define BOXDRAW_VERTICAL_RIGHT_DOUBLE      0x255e
#define BOXDRAW_VERTICAL_DOUBLE_RIGHT      0x255f
#define BOXDRAW_DOUBLE_VERTICAL_RIGHT      0x2560
#define BOXDRAW_VERTICAL_LEFT_DOUBLE       0x2561
#define BOXDRAW_VERTICAL_DOUBLE_LEFT       0x2562
#define BOXDRAW_DOUBLE_VERTICAL_LEFT       0x2563
#define BOXDRAW_DOWN_HORIZONTAL_DOUBLE     0x2564
#define BOXDRAW_DOWN_DOUBLE_HORIZONTAL     0x2565
#define BOXDRAW_DOUBLE_DOWN_HORIZONTAL     0x2566
#define BOXDRAW_UP_HORIZONTAL_DOUBLE       0x2567
#define BOXDRAW_UP_DOUBLE_HORIZONTAL       0x2568
#define BOXDRAW_DOUBLE_UP_HORIZONTAL       0x2569
#define BOXDRAW_VERTICAL_HORIZONTAL_DOUBLE 0x256a
#define BOXDRAW_VERTICAL_DOUBLE_HORIZONTAL 0x256b
#define BOXDRAW_DOUBLE_VERTICAL_HORIZONTAL 0x256c
//*******************************************************
// EFI Required Block Elements Code Chart
//*******************************************************
#define BLOCKELEMENT_FULL_BLOCK            0x2588
#define BLOCKELEMENT_LIGHT_SHADE           0x2591
//*******************************************************
// EFI Required Geometric Shapes Code Chart
//*******************************************************
#define GEOMETRICSHAPE_UP_TRIANGLE         0x25b2
#define GEOMETRICSHAPE_RIGHT_TRIANGLE      0x25ba
#define GEOMETRICSHAPE_DOWN_TRIANGLE       0x25bc
#define GEOMETRICSHAPE_LEFT_TRIANGLE       0x25c4
//*******************************************************
// EFI Required Arrow shapes
//*******************************************************
#define ARROW_UP                           0x2191
#define ARROW_DOWN                         0x2193

#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID \
    {0x387477c2,0x69c7,0x11d2,\
    {0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

//
// TestReset
//
struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef
    efi_status_t
    (__efiapi *EFI_TEXT_RESET) (
        IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this,
        IN BOOLEAN                         extendedVerification
        );

//
// OutputString
//
typedef
    efi_status_t
    (__efiapi *EFI_TEXT_STRING) (
        IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        IN uint16_t                          *String
        );

//
// TestString
//
typedef
    efi_status_t
    (__efiapi *EFI_TEXT_TEST_STRING) (
        IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        IN uint16_t              *String
        );

//
// QueryMode
//
typedef
    efi_status_t
    (__efiapi *EFI_TEXT_QUERY_MODE) (
        IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        IN uintn_t              ModeNumber,
        OUT uintn_t              *Columns,
        OUT uintn_t              *Rows
        );

//
// SetMode
//
typedef
    efi_status_t
    (* __efiapi EFI_TEXT_SET_MODE) (
        IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        IN uintn_t                           ModeNumber
        );

//
// SetAttribute
//
typedef
    efi_status_t
    (__efiapi *EFI_TEXT_SET_ATTRIBUTE) (
        IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        IN uintn_t                           Attribute
        );
//*******************************************************
// Attributes
//*******************************************************
#define EFI_BLACK                            0x00
#define EFI_BLUE                             0x01
#define EFI_GREEN                            0x02
#define EFI_CYAN                             0x03
#define EFI_RED                              0x04
#define EFI_MAGENTA                          0x05
#define EFI_BROWN                            0x06
#define EFI_LIGHTGRAY                        0x07
#define EFI_BRIGHT                           0x08
#define EFI_DARKGRAY                         0x08
#define EFI_LIGHTBLUE                        0x09
#define EFI_LIGHTGREEN                       0x0A
#define EFI_LIGHTCYAN                        0x0B
#define EFI_LIGHTRED                         0x0C
#define EFI_LIGHTMAGENTA                     0x0D
#define EFI_YELLOW                           0x0E
#define EFI_WHITE                            0x0F

#define EFI_BACKGROUND_BLACK                 0x00
#define EFI_BACKGROUND_BLUE                  0x10
#define EFI_BACKGROUND_GREEN                 0x20
#define EFI_BACKGROUND_CYAN                  0x30
#define EFI_BACKGROUND_RED                   0x40
#define EFI_BACKGROUND_MAGENTA               0x50
#define EFI_BACKGROUND_BROWN                 0x60
#define EFI_BACKGROUND_LIGHTGRAY             0x70
// The foreground color and background color can
//  be OR-ed(|) together to set them both in one call.

//
// ClearScreen
//
typedef
    efi_status_t
    (__efiapi *EFI_TEXT_CLEAR_SCREEN) (
        IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL  *This
        );

//
// SetCursorPosition
//
typedef
    efi_status_t
    (__efiapi *EFI_TEXT_SET_CURSOR_POSITION) (
        IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        IN uintn_t                           Column,
        IN uintn_t                           Row
        );

//
// EnableCursor
//
typedef
    efi_status_t
    (__efiapi *EFI_TEXT_ENABLE_CURSOR) (
        IN struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
        IN BOOLEAN                         Visible
        );

typedef struct {
    int32_t               max_mode; // current settings
    int32_t               mode;
    int32_t               attribute;
    int32_t               cursor_column;
    int32_t               cursor_row;
    BOOLEAN             cursor_visible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET                  reset;
    EFI_TEXT_STRING                 output_string;
    EFI_TEXT_TEST_STRING            test_string;
    EFI_TEXT_QUERY_MODE             query_mode;
    EFI_TEXT_SET_MODE               set_mode;
    EFI_TEXT_SET_ATTRIBUTE          set_attribute;
    EFI_TEXT_CLEAR_SCREEN           clear_screen;
    EFI_TEXT_SET_CURSOR_POSITION    set_cursor_position;
    EFI_TEXT_ENABLE_CURSOR          enable_cursor;
    SIMPLE_TEXT_OUTPUT_MODE         *node;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

/***********************/
/*  Table_Definitions  */
/***********************/

#define EFI_SPECIFICATION_VERSION   EFI_SYSTEM_TABLE_REVISION
#define EFI_SYSTEM_TABLE_REVISION EFI_2_8_SYSTEM_TABLE_REVISION

typedef struct {
    uint64_t  signature;
    uint32_t  revision;
    uint32_t  headerSize;
    uint32_t  crc_32;
    uint32_t  reserved;
} EFI_TABLE_HEADER;

#define EFI_BOOT_SERVICES_SIGNATURE   0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION   EFI_SPECIFICATION_VERSION
typedef struct {
    EFI_TABLE_HEADER hdr;

    //
    // Task Priority Services
    //
    EFI_RAISE_TPL raise_tpl; // EFI 1.0+
    EFI_RESTORE_TPL restore_tpl; // EFI 1.0+

    //
    // Memory Services
    //
    EFI_ALLOCATE_PAGES allocate_pages; // EFI 1.0+
    EFI_FREE_PAGES free_pages; // EFI 1.0+
    EFI_GET_MEMORY_MAP get_memory_map; // EFI 1.0+
    EFI_ALLOCATE_POOL allocate_pool; // EFI 1.0+
    EFI_FREE_POOL free_pool; // EFI 1.0+

    //
    // Event & Timer Services
    //
    EFI_CREATE_EVENT create_event; // EFI 1.0+
    EFI_SET_TIMER set_timer; // EFI 1.0+
    EFI_WAIT_FOR_EVENT wait_for_event; // EFI 1.0+
    EFI_SIGNAL_EVENT signal_event; // EFI 1.0+
    EFI_CLOSE_EVENT close_event; // EFI 1.0+
    EFI_CHECK_EVENT check_event; // EFI 1.0+

    //
    // Protocol Handler Services
    //
    EFI_INSTALL_PROTOCOL_INTERFACE install_protocol_interface; // EFI 1.0+
    EFI_REINSTALL_PROTOCOL_INTERFACE reinstall_protocol_interface; // EFI 1.0+
    EFI_UNINSTALL_PROTOCOL_INTERFACE uninstall_protocol_interface; // EFI 1.0+
    efi_handle_t_PROTOCOL handle_protocol; // EFI 1.0+
    void* Reserved; // EFI 1.0+
    EFI_REGISTER_PROTOCOL_NOTIFY register_protocol_notify; // EFI 1.0+
    EFI_LOCATE_HANDLE locate_handle; // EFI 1.0+
    EFI_LOCATE_DEVICE_PATH locate_device_path; // EFI 1.0+
    EFI_INSTALL_CONFIGURATION_TABLE install_configuration_table; // EFI 1.0+

    //
    // Image Services
    //
    EFI_IMAGE_LOAD load_image; // EFI 1.0+
    EFI_IMAGE_START start_image; // EFI 1.0+UEFI Specification, Version 2.8 EFI System Table
    EFI_EXIT Exit; // EFI 1.0+
    EFI_IMAGE_UNLOAD unload_image; // EFI 1.0+
    EFI_EXIT_BOOT_SERVICES exit_boot_services; // EFI 1.0+

    //
    // Miscellaneous Services
    //
    EFI_GET_NEXT_MONOTONIC_COUNT get_next_monotonic_count; // EFI 1.0+
    EFI_STALL Stall; // EFI 1.0+
    EFI_SET_WATCHDOG_TIMER set_watchdog_timer; // EFI 1.0+

    //
    // DriverSupport Services
    //
    EFI_CONNECT_CONTROLLER connect_controller; // EFI 1.1
    EFI_DISCONNECT_CONTROLLER disconnect_controller;// EFI 1.1+

    //
    // Open and Close Protocol Services
    //
    EFI_OPEN_PROTOCOL open_protocol; // EFI 1.1+
    EFI_CLOSE_PROTOCOL close_protocol; // EFI 1.1+
    EFI_OPEN_PROTOCOL_INFORMATION open_protocol_information; // EFI 1.1+

    //
    // Library Services
    //
    EFI_PROTOCOLS_PER_HANDLE protocols_per_handle; // EFI 1.1+
    EFI_LOCATE_HANDLE_BUFFER locate_handle_buffer; // EFI 1.1+
    EFI_LOCATE_PROTOCOL locate_protocol; // EFI 1.1+
    EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES
    install_multiple_protocol_interfaces; // EFI 1.1+
    EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES
    uninstall_multiple_protocol_interfaces; // EFI 1.1+

    //
    // 32-bit CRC Services
    //
    EFI_CALCULATE_CRC32 calculate_crc32; // EFI 1.1+

    //
    // Miscellaneous Services
    //
    EFI_COPY_MEM copy_mem; // EFI 1.1+
    EFI_SET_MEM set_mem; // EFI 1.1+UEFI Specification, Version 2.8 EFI System Table
    EFI_CREATE_EVENT_EX create_event_ex; // UEFI 2.0+
} EFI_BOOT_SERVICES;

#define EFI_RUNTIME_SERVICES_SIGNATURE 0x56524553544e5552
#define EFI_RUNTIME_SERVICES_REVISION  EFI_SPECIFICATION_VERSION
typedef struct {
    EFI_TABLE_HEADER hdr;

    //
    // Time Services
    //
    EFI_GET_TIME get_time;
    EFI_SET_TIME set_time;
    EFI_GET_WAKEUP_TIME get_wakeup_time;
    EFI_SET_WAKEUP_TIME set_wakeup_time;

    //
    // Virtual Memory Services
    //
    EFI_SET_VIRTUAL_ADDRESS_MAP set_virtual_address_map;
    EFI_CONVERT_POINTER convert_pointer;

    //
    // Variable Services
    //
    EFI_GET_VARIABLE get_variable;
    EFI_GET_NEXT_VARIABLE_NAME get_next_variable_name;
    EFI_SET_VARIABLE set_variable;

    //
    // Miscellaneous Services
    //
    EFI_GET_NEXT_HIGH_MONOTONIC_COUNT get_next_high_monotonic_count;
    EFI_RESET_SYSTEM reset_system;

    //
    // UEFI 2.0 Capsule Services
    //
    EFI_UPDATE_CAPSULE update_capsule;
    EFI_QUERY_CAPSULE_CAPABILITIES query_capsule_capabilities;

    //
    // Miscellaneous UEFI 2.0 Service
    //
    EFI_QUERY_VARIABLE_INFO query_variable_info;
} EFI_RUNTIME_SERVICES;

typedef struct {
    EFI_GUID    vendor_guid;
    void       *vendor_table;
} EFI_CONFIGURATION_TABLE;

#define ACPI_TABLE_GUID \
    {0xeb9d2d30,0x2d88,0x11d3,\
    {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define SAL_SYSTEM_TABLE_GUID \
    {0xeb9d2d32,0x2d88,0x11d3,\
    {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define SMBIOS_TABLE_GUID \
    {0xeb9d2d31,0x2d88,0x11d3,\
    {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define SMBIOS3_TABLE_GUID \
    {0xf2fd1544, 0x9794, 0x4a2c,\
    {0x99,0x2e,0xe5,0xbb,0xcf,0x20,0xe3,0x94}}

#define MPS_TABLE_GUID \
    {0xeb9d2d2f,0x2d88,0x11d3,\
    {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

//
// ACPI 2.0 or newer tables should use EFI_ACPI_TABLE_GUID
//
#define EFI_ACPI_TABLE_GUID \
    {0x8868e871,0xe4f1,0x11d3,\
    {0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}}
#define EFI_ACPI_20_TABLE_GUID EFI_ACPI_TABLE_GUID

#define ACPI_TABLE_GUID \
    {0xeb9d2d30,0x2d88,0x11d3,\
    {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}
#define ACPI_10_TABLE_GUID ACPI_TABLE_GUID

#define EFI_JSON_CONFIG_DATA_TABLE_GUID \
    {0x87367f87, 0x1119, 0x41ce, \
    {0xaa,0xec,0x8b,0xe0,0x11,0x1f,0x55,0x8a}}

#define EFI_JSON_CAPSULE_DATA_TABLE_GUID \
    {0x35e7a725,0x8dd2,0x4cac,\
    {0x80,0x11,0x33,0xcd,0xa8,0x10,0x90,0x56}}

#define EFI_JSON_CAPSULE_RESULT_TABLE_GUID \
    {0xdbc461c3,0xb3de,0x422a,\
    {0xb9,0xb4,0x98,0x86,0xfd,0x49,0xa1,0xe5}}

#define EFI_PROPERTIES_TABLE_VERSION 0x00010000
typedef struct {
    uint32_t version;
    uint32_t length;
    uint32_t memory_protection_attribute;
} EFI_PROPERTIES_TABLE;
//
// Memory attribute (Non-defined bits are reserved)
//
#define EFI_PROPERTIES_RUNTIME_MEMORY_PROTECTION_NON_EXECUTABLE_PE_DATA   0x1
// BIT 0 - description - implies the runtime data is separated from the code

#define EFI_ACPI_TABLE_PROTOCOL_GUID \
    {0xffe06bdd, 0x6107, 0x46a6,\
    {0x7b, 0xb2, 0x5a, 0x9c, 0x7e, 0xc5, 0x27, 0x5c}}

#define EFI_MEMORY_ATTRIBUTES_TABLE_GUID \
    {0xdcfa911d, 0x26eb, 0x469f, \
    {0xa2, 0x20, 0x38, 0xb7, 0xdc, 0x46, 0x12, 0x20}}

//
// EFI_MEMORY_ATTRIBUTES_TABLE
//
typedef struct {
    uint32_t  version;
    uint32_t  number_of_entries;
    uint32_t  descriptor_size;
    uint32_t  reserved;
    // EFI_MEMORY_DESCRIPTOR Entry[1];
} EFI_MEMORY_ATTRIBUTES_TABLE;

#define EFI_SYSTEM_TABLE_SIGNATURE   0x5453595320494249
#define EFI_2_80_SYSTEM_TABLE_REVISION ((2<<16) | (80))
#define EFI_2_70_SYSTEM_TABLE_REVISION ((2<<16) | (70))
#define EFI_2_60_SYSTEM_TABLE_REVISION ((2<<16) | (60))
#define EFI_2_50_SYSTEM_TABLE_REVISION ((2<<16) | (50))
#define EFI_2_40_SYSTEM_TABLE_REVISION ((2<<16) | (40))
#define EFI_2_31_SYSTEM_TABLE_REVISION ((2<<16) | (31))
#define EFI_2_30_SYSTEM_TABLE_REVISION ((2<<16) | (30))
#define EFI_2_20_SYSTEM_TABLE_REVISION ((2<<16) | (20))
#define EFI_2_10_SYSTEM_TABLE_REVISION ((2<<16) | (10))
#define EFI_2_00_SYSTEM_TABLE_REVISION ((2<<16) | (00))
#define EFI_1_10_SYSTEM_TABLE_REVISION ((1<<16) | (10))
#define EFI_1_02_SYSTEM_TABLE_REVISION ((1<<16) | (02))
typedef struct {
    EFI_TABLE_HEADER                  hdr;
    uint16_t                            *firmware_vendor;
    uint32_t                            firmware_revision;
    efi_handle_t                        console_in_handle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL    *con_in;
    efi_handle_t                        console_out_handle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL   *con_out;
    efi_handle_t                        standard_error_handle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL   *std_err;
    EFI_RUNTIME_SERVICES              *runtime_services;
    EFI_BOOT_SERVICES                 *boot_services;
    uintn_t                             number_of_table_entries;
    EFI_CONFIGURATION_TABLE           *configuration_table;
} EFI_SYSTEM_TABLE;

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
    {0x9042a9de,0x23dc,0x4a38,\
    {0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}}

struct EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef enum {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
    uint32_t red_mask;
    uint32_t green_mask;
    uint32_t blue_mask;
    uint32_t reserved_mask;
} EFI_PIXEL_BITMASK;

typedef struct {
    uint32_t version;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    EFI_GRAPHICS_PIXEL_FORMAT pixel_format;
    EFI_PIXEL_BITMASK pixel_information;
    uint32_t pixels_per_scan_line;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef
    efi_status_t
    (__efiapi *EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE) (
        IN struct EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
        IN uint32_t ModeNumber,
        OUT uintn_t *SizeOfInfo,
        OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info
        );

typedef
    efi_status_t
    (__efiapi *EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE) (
        IN struct EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
        IN uint32_t ModeNumber
        );

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;
typedef enum {
    EfiBltVideoFill,
    EfiBltVideoToBltBuffer,
    EfiBltBufferToVideo,
    EfiBltVideoToVideo,
    EfiGraphicsOutputBltOperationMax
} EFI_GRAPHICS_OUTPUT_BLT_OPERATION;
typedef
    efi_status_t
    (__efiapi *EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT) (
        IN struct EFI_GRAPHICS_OUTPUT_PROTOCOL *This,
        IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer, OPTIONAL
        IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation,
        IN uintn_t SourceX,
        IN uintn_t SourceY,
        IN uintn_t DestinationX,
        IN uintn_t DestinationY,
        IN uintn_t Width,
        IN uintn_t Height,
        IN uintn_t Delta OPTIONAL
        );

typedef struct {
    uint32_t max_mode;
    uint32_t mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    uintn_t size_of_info;
    efi_phys_addr_t frame_buffer_base;
    uintn_t frame_buffer_size;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
    EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE query_mode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE set_mode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

//
// ImageEntryPoint
//
typedef
    efi_status_t
    (__efiapi *EFI_IMAGE_ENTRY_POINT) (
        IN efi_handle_t       ImageHandle,
        IN EFI_SYSTEM_TABLE  *SystemTable
        );

#define EFI_MP_SERVICES_PROTOCOL_GUID \
    {0x3fdda605,0xa76e,0x4f46,{0xad,0x29,0x12,0xf4,\
    0x53,0x1b,0x3d,0x08}}

struct EFI_MP_SERVICES_PROTOCOL;

typedef
    efi_status_t
    (__efiapi *EFI_MP_SERVICES_GET_NUMBER_OF_PROCESSORS) (
        IN struct EFI_MP_SERVICES_PROTOCOL *This,
        OUT uintn_t *NumberOfProcessors,
        OUT uintn_t *NumberOfEnabledProcessors
        );

typedef struct {
    uint32_t package;
    uint32_t core;
    uint32_t thread;
} EFI_CPU_PHYSICAL_LOCATION;
typedef struct {
    uint64_t processor_id;
    uint32_t status_flag;
    EFI_CPU_PHYSICAL_LOCATION location;
} EFI_PROCESSOR_INFORMATION;
#define PROCESSOR_AS_BSP_BIT 0x00000001
#define PROCESSOR_ENABLED_BIT 0x00000002
#define PROCESSOR_HEALTH_STATUS_BIT 0x00000004
typedef
    efi_status_t
    (__efiapi *EFI_MP_SERVICES_GET_PROCESSOR_INFO) (
        IN struct EFI_MP_SERVICES_PROTOCOL *This,
        IN uintn_t ProcessorNumber,
        OUT EFI_PROCESSOR_INFORMATION *ProcessorInfoBuffer
        );

#define END_OF_CPU_LIST 0xffffffff
typedef
    void
    (__efiapi *EFI_AP_PROCEDURE) (
        IN void *ProcedureArgument
        );
typedef
    efi_status_t
    (__efiapi *EFI_MP_SERVICES_STARTUP_ALL_APS) (
        IN struct EFI_MP_SERVICES_PROTOCOL *This,
        IN EFI_AP_PROCEDURE Procedure,
        IN BOOLEAN SingleThread,
        IN efi_event_t WaitEvent OPTIONAL,
        IN uintn_t TimeoutInMicroSeconds,
        IN void *ProcedureArgument OPTIONAL,
        OUT uintn_t **FailedCpuList OPTIONAL
        );

typedef
    efi_status_t
    (__efiapi *EFI_MP_SERVICES_STARTUP_THIS_AP) (
        IN struct EFI_MP_SERVICES_PROTOCOL*This,
        IN EFI_AP_PROCEDURE Procedure,
        IN uintn_t ProcessorNumber,
        IN efi_event_t WaitEvent OPTIONAL,
        IN uintn_t TimeoutInMicroseconds,
        IN void *ProcedureArgument OPTIONAL,
        OUT BOOLEAN *Finished OPTIONAL
        );

typedef
    efi_status_t
    (__efiapi *EFI_MP_SERVICES_SWITCH_BSP) (
        IN struct EFI_MP_SERVICES_PROTOCOL *This,
        IN uintn_t ProcessorNumber,
        IN BOOLEAN EnableOldBSP
        );

typedef
    efi_status_t
    (__efiapi *EFI_MP_SERVICES_ENABLEDISABLEAP) (
        IN struct EFI_MP_SERVICES_PROTOCOL*This,
        IN uintn_t ProcessorNumber,
        IN BOOLEAN EnableAP,
        IN uint32_t *HealthFlag OPTIONAL
        );

typedef
    efi_status_t
    (__efiapi *EFI_MP_SERVICES_WHOAMI) (
        IN struct EFI_MP_SERVICES_PROTOCOL *This,
        OUT uintn_t *ProcessorNumber
        );

typedef struct EFI_MP_SERVICES_PROTOCOL {
    EFI_MP_SERVICES_GET_NUMBER_OF_PROCESSORS GetNumberOfProcessors;
    EFI_MP_SERVICES_GET_PROCESSOR_INFO GetProcessorInfo;
    EFI_MP_SERVICES_STARTUP_ALL_APS StartupAllAPs;
    EFI_MP_SERVICES_STARTUP_THIS_AP StartupThisAP;
    EFI_MP_SERVICES_SWITCH_BSP SwitchBSP;
    EFI_MP_SERVICES_ENABLEDISABLEAP EnableDisableAP;
    EFI_MP_SERVICES_WHOAMI WhoAmI;
} EFI_MP_SERVICES_PROTOCOL;

#define EFI_SIMPLE_POINTER_PROTOCOL_GUID \
{0x31878c87,0xb75,0x11d5,\
{0x9a,0x4f,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

typedef struct {
    uint64_t resolution_x;
    uint64_t resolution_y;
    uint64_t resolution_z;
    BOOLEAN left_button;
    BOOLEAN right_button;
} EFI_SIMPLE_POINTER_MODE;

struct _EFI_SIMPLE_POINTER_PROTOCOL;

typedef struct {
    int32_t relative_movement_x;
    int32_t relative_movement_y;
    int32_t relative_movement_z;
    BOOLEAN left_button;
    BOOLEAN right_button;
} EFI_SIMPLE_POINTER_STATE;

typedef
    efi_status_t
    (__efiapi *EFI_SIMPLE_POINTER_RESET) (
        IN struct _EFI_SIMPLE_POINTER_PROTOCOL *This,
        IN BOOLEAN ExtendedVerification
        );

typedef
    efi_status_t
    (__efiapi *EFI_SIMPLE_POINTER_GET_STATE) (
        IN struct _EFI_SIMPLE_POINTER_PROTOCOL *This,
        IN OUT EFI_SIMPLE_POINTER_STATE *State
        );

typedef struct _EFI_SIMPLE_POINTER_PROTOCOL {
    EFI_SIMPLE_POINTER_RESET Reset;
    EFI_SIMPLE_POINTER_GET_STATE GetState;
    efi_event_t WaitForInput;
    EFI_SIMPLE_POINTER_MODE *Mode;
} EFI_SIMPLE_POINTER_PROTOCOL;

// ACPI tables

typedef struct __attribute__((packed)) rsdp_t {
    uint64_t signature;   // Should be "RSD PTR "
    uint8_t checksum;     // Checksum for rsdp v1.0, all of v1.0 bytes added together should equal 0
    uint8_t OEMID[6];     // OEM supplied string
    uint8_t revision;     // Revision of this structure
    uint32_t rsdtAddress; // Physical address of rsdt table

    // For revisions => 2
    uint32_t length;      // Length of the table
    uint64_t xsdtAddress; // Physical address of xdst_t table
    uint8_t extendedChecksum; // Checksum for the entire table, sum of all bytes should equal 0
    uint8_t reserves[3];  // Reserved field.
} rsdp_t;

typedef struct __attribute__((packed)) {
    uint32_t signature;
    uint32_t length;
    uint8_t  revision;
    uint8_t  checksum;
    uint8_t  OEMID[6];
    uint8_t  OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
} sdth_t; // System Description Table Header

typedef struct __attribute__((packed)) {
    sdth_t header;
    uint32_t entry[];   // 32 bit physical memory addresses, number of entries is based on the length in the header
} rsdt_t;

typedef struct __attribute__((packed)) {
    sdth_t header;
    uint64_t entry[];   // 64 bit physical memory addresses, number of entries is based on the length in the header
} xdst_t;

/*
 * -- FILE PROTOCOL --
 */
#define EFI_FILE_INFO_ID {0x09576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_FILE_SYSTEM_INFO_ID {0x09576e93, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_FILE_SYSTEM_VOLUME_LABEL_ID {0xdb47d7d3, 0xfe81, 0x11d3, {0x9a, 0x35, 0x00, 0x90, 0x27, 0x3f, 0xc1, 0x4d}}

#define EFI_FILE_PROTOCOL_REVISION 0x00010000
#define EFI_FILE_PROTOCOL_REVISION2 0x00020000
#define EFI_FILE_PROTOCOL_LATEST_REVISION EFI_FILE_PROTOCOL_REVISION2

#define EFI_FILE_MODE_READ 0x0000000000000001
#define EFI_FILE_MODE_WRITE 0x0000000000000002
#define EFI_FILE_MODE_CREATE 0x8000000000000000

#define EFI_FILE_READ_ONLY 0x0000000000000001
#define EFI_FILE_HIDDEN 0x0000000000000002
#define EFI_FILE_SYSTEM 0x0000000000000004
#define EFI_FILE_RESERVED 0x0000000000000008
#define EFI_FILE_DIRECTORY 0x0000000000000010
#define EFI_FILE_ARCHIVE 0x0000000000000020
#define EFI_FILE_VALID_ATTR 0x0000000000000037

typedef struct EFI_FILE_INFO {
    uint64_t size;
    uint64_t file_size;
    uint64_t physical_size;
    EFI_TIME create_time;
    EFI_TIME last_access_time;
    EFI_TIME modification_time;
    uint64_t attribute;
    uint16_t filename[];
} EFI_FILE_INFO;

typedef struct EFI_FILE_SYSTEM_INFO {
    uint64_t Size;
    BOOLEAN ReadOnly;
    uint64_t VolumeSize;
    uint64_t FreeSpace;
    uint32_t BlockSize;
    uint16_t VolumeLabel;
} EFI_FILE_SYSTEM_INFO;

typedef struct EFI_FILE_SYSTEM_VOLUME_LABEL {
    uint16_t VolumeLabel[];
} EFI_FILE_SYSTEM_VOLUME_LABEL;

typedef struct EFI_FILE_IO_TOKEN {
    EFI_EVENT Event;
    efi_status_t Status;
    uintn_t BufferSize;
    void *Buffer;
} EFI_FILE_IO_TOKEN;

struct EFI_FILE_PROTOCOL;

typedef efi_status_t (__efiapi *EFI_FILE_OPEN)(IN struct EFI_FILE_PROTOCOL *This, OUT struct EFI_FILE_PROTOCOL **NewHandle, IN uint16_t *FileName, IN uint64_t OpenMode, IN uint64_t Attributes);
typedef efi_status_t (__efiapi *EFI_FILE_CLOSE)(IN struct EFI_FILE_PROTOCOL *This);
typedef efi_status_t (__efiapi *EFI_FILE_DELETE)(IN struct EFI_FILE_PROTOCOL *This);
typedef efi_status_t (__efiapi *EFI_FILE_READ)(IN struct EFI_FILE_PROTOCOL *This, IN OUT uintn_t *BufferSize, OUT void *Buffer);
typedef efi_status_t (__efiapi *EFI_FILE_WRITE)(IN struct EFI_FILE_PROTOCOL *This, IN OUT uintn_t *BufferSize, IN void *Buffer);
typedef efi_status_t (__efiapi *EFI_FILE_OPEN_EX)(IN struct EFI_FILE_PROTOCOL *This, OUT struct EFI_FILE_PROTOCOL **NewHandle, IN uint16_t *FileName, IN uint64_t OpenMode, IN uint64_t Attributes, IN OUT EFI_FILE_IO_TOKEN *Token);
typedef efi_status_t (__efiapi *EFI_FILE_READ_EX)(IN struct EFI_FILE_PROTOCOL *This, IN OUT EFI_FILE_IO_TOKEN *Token);
typedef efi_status_t (__efiapi *EFI_FILE_WRITE_EX)(IN struct EFI_FILE_PROTOCOL *This, IN OUT EFI_FILE_IO_TOKEN *Token);
typedef efi_status_t (__efiapi *EFI_FILE_FLUSH_EX)(IN struct EFI_FILE_PROTOCOL *This, IN EFI_FILE_IO_TOKEN *Token);
typedef efi_status_t (__efiapi *EFI_FILE_SET_POSITION)(IN struct EFI_FILE_PROTOCOL *This, IN uint64_t Position);
typedef efi_status_t (__efiapi *EFI_FILE_GET_POSITION)(IN struct EFI_FILE_PROTOCOL *This, OUT uint64_t *Position);
typedef efi_status_t (__efiapi *EFI_FILE_GET_INFO)(IN struct EFI_FILE_PROTOCOL *This, IN EFI_GUID *InformationType, IN OUT uintn_t *BufferSize, OUT void *Buffer);
typedef efi_status_t (__efiapi *EFI_FILE_SET_INFO)(IN struct EFI_FILE_PROTOCOL *This, IN EFI_GUID *InformationType, IN uintn_t BufferSize, IN void *Buffer);
typedef efi_status_t (__efiapi *EFI_FILE_FLUSH)(IN struct EFI_FILE_PROTOCOL *This);

typedef struct EFI_FILE_PROTOCOL {
    uint64_t revision;
    EFI_FILE_OPEN open;
    EFI_FILE_CLOSE close;
    EFI_FILE_DELETE delete;
    EFI_FILE_READ read;
    EFI_FILE_WRITE write;
    EFI_FILE_GET_POSITION get_position;
    EFI_FILE_SET_POSITION set_position;
    EFI_FILE_GET_INFO get_info;
    EFI_FILE_SET_INFO set_info;
    EFI_FILE_FLUSH flush;
    EFI_FILE_OPEN_EX open_ex;
    EFI_FILE_READ_EX read_ex;
    EFI_FILE_WRITE_EX write_ex;
    EFI_FILE_FLUSH_EX flush_ex;
} EFI_FILE_PROTOCOL;

/*
 * -- System filesystem protocol --
 */
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION 0x00010000
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID {0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef efi_status_t (__efiapi *EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME)(
    IN struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *This,
    OUT EFI_FILE_PROTOCOL **Root
);

typedef struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    uint64_t revision;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME open_volume;
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

/*
 * -- Loaded image protocol --
 */
#define EFI_LOADED_IMAGE_PROTOCOL_GUID {0x5b1b31a1, 0x9562, 0x11d2, {0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}
#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000

typedef efi_status_t (__efiapi *EFI_IMAGE_UNLOAD)(IN efi_handle_t ImageHandle);
typedef struct EFI_LOADED_IMAGE_PROTOCOL {
    uint32_t revision;
    efi_handle_t parent_handle;
    EFI_SYSTEM_TABLE *system_table;
    efi_handle_t device_handle;
    EFI_DEVICE_PATH_PROTOCOL *file_path;
    void *reserved;
    uint32_t load_options_size;
    void *load_options;
    void *image_base;
    uint64_t image_size;
    EFI_MEMORY_TYPE image_code_type;
    EFI_MEMORY_TYPE image_data_type;
    EFI_IMAGE_UNLOAD unload;
} EFI_LOADED_IMAGE_PROTOCOL;

#endif  /* !_EFI_H_ */
