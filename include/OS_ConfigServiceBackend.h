/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @addtogroup Backend
 * @{
 *
 * @file OS_ConfigServiceBackend.h
 *
 * @brief Depending on which type of backend is built (either a FileSystem or a MemoryBackend)
 * this module collects functions to interact with the backend (read, write, etc.).
 *
 */


#pragma once

/* Includes ------------------------------------------------------------------*/
#include "OS_Error.h"

#if defined(OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

#include "OS_FileSystem.h"

#endif // OS_CONFIG_SERVICE_BACKEND_FILESYSTEM

/* Exported types/defines/enums ----------------------------------------------*/

#if defined(OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

enum
{
    // Max size of a config backend file name (including null terminator).
    OS_CONFIG_BACKEND_MAX_FILE_NAME_SIZE  = (8 + 1 + 3 + 1)
};

typedef struct
{
    char buffer[OS_CONFIG_BACKEND_MAX_FILE_NAME_SIZE];
}
OS_ConfigServiceBackend_FileName_t;

#endif // OS_CONFIG_SERVICE_BACKEND_FILESYSTEM


typedef enum
{
    OS_CONFIG_BACKEND_BACKEND_TYPE_FS   = 1,
    OS_CONFIG_BACKEND_BACKEND_TYPE_MEM  = 2,
}
OS_ConfigServiceBackend_BackendType_t;


typedef struct
{
    OS_ConfigServiceBackend_BackendType_t  backendType;
    union
    {

#if defined(OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

        struct
        {
            OS_ConfigServiceBackend_FileName_t name;
            OS_FileSystem_Handle_t hFs;
        } fileSystem;

#endif // OS_CONFIG_SERVICE_BACKEND_FILESYSTEM

#if defined(OS_CONFIG_SERVICE_BACKEND_MEMORY)

        struct
        {
            void* buffer;
            size_t bufferSize;
        } memory;

#endif // OS_CONFIG_SERVICE_BACKEND_MEMORY

    } backend;

    unsigned int  numberOfRecords;
    size_t        sizeOfRecord;
}
OS_ConfigServiceBackend_t;

//------------------------------------------------------------------------------
// Filesystem Backend API
//------------------------------------------------------------------------------

#if defined(OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

// Management function: creates the given file to contain the specified backend
// layout.
OS_Error_t
OS_ConfigServiceBackend_createFileBackend(
    OS_ConfigServiceBackend_FileName_t name,
    OS_FileSystem_Handle_t hFs,
    unsigned int numberOfRecords,
    size_t sizeOfRecord);

// Initialize the given backend object with the backend layout retrieved from
// the given file.
OS_Error_t
OS_ConfigServiceBackend_initializeFileBackend(
    OS_ConfigServiceBackend_t* instance,
    OS_ConfigServiceBackend_FileName_t name,
    OS_FileSystem_Handle_t hFs);

#endif // OS_CONFIG_SERVICE_BACKEND_FILESYSTEM


//------------------------------------------------------------------------------
// Memory Backend API
//------------------------------------------------------------------------------

#if defined(OS_CONFIG_SERVICE_BACKEND_MEMORY)

// Management function: initializes the given buffer to contain the specified
// backend layout.
OS_Error_t
OS_ConfigServiceBackend_createMemBackend(
    void* buffer,
    size_t bufferSize,
    unsigned int numberOfRecords,
    size_t sizeOfRecord);


// Management function: initializes the given buffer to contain the specified
// backend layout.
OS_Error_t
OS_ConfigServiceBackend_createMemBackendAutoSized(
    void* buffer,
    size_t bufferSize,
    size_t sizeOfRecord);


// Initialize the given backend object with the backend layout retrieved from
// the given buffer.
OS_Error_t
OS_ConfigServiceBackend_initializeMemBackend(
    OS_ConfigServiceBackend_t* instance,
    void* buffer,
    size_t bufferSize);

#endif // OS_CONFIG_SERVICE_BACKEND_MEMORY


//------------------------------------------------------------------------------
// Generic Function API
//------------------------------------------------------------------------------

unsigned int
OS_ConfigServiceBackend_getNumberOfRecords(
    OS_ConfigServiceBackend_t const* instance);

size_t
OS_ConfigServiceBackend_getSizeOfRecords(
    OS_ConfigServiceBackend_t const* instance);

OS_Error_t
OS_ConfigServiceBackend_readRecord(
    OS_ConfigServiceBackend_t* instance,
    unsigned int recordIndex,
    void* buf,
    size_t bufSize);

OS_Error_t
OS_ConfigServiceBackend_writeRecord(
    OS_ConfigServiceBackend_t* instance,
    unsigned int recordIndex,
    const void* buf,
    size_t bufSize);

/** @} */
