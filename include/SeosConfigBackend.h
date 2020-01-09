/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "SeosError.h"

#if defined(CONFIG_SERVER_BACKEND_FILESYSTEM)

#include "seos_fs_datatypes.h"

#endif // CONFIG_SERVER_BACKEND_FILESYSTEM


#if defined(CONFIG_SERVER_BACKEND_FILESYSTEM)

enum
{
    SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN  = (8 + 1 + 3 + 1)
};

typedef struct SeosConfigBackend_FileName
{
    char buffer[SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN];
}
SeosConfigBackend_FileName;

#endif // CONFIG_SERVER_BACKEND_FILESYSTEM


typedef enum
{
    SEOS_CONFIG_BACKEND_BACKEND_TYPE_FS   = 1,
    SEOS_CONFIG_BACKEND_BACKEND_TYPE_MEM  = 2,
}
SeosConfigBackend_BackendType;


typedef struct SeosConfigBackend
{
    SeosConfigBackend_BackendType  backendType;
    union
    {

#if defined(CONFIG_SERVER_BACKEND_FILESYSTEM)

        struct
        {
            SeosConfigBackend_FileName  name;
            hPartition_t                phandle;
        } fileSystem;

#endif // CONFIG_SERVER_BACKEND_FILESYSTEM

#if defined(CONFIG_SERVER_BACKEND_MEMORY)

        struct
        {
            void *  buffer;
            size_t  bufferSize;
        } memory;

#endif // CONFIG_SERVER_BACKEND_MEMORY

    } backend;

    unsigned int  numberOfRecords;
    size_t        sizeOfRecord;
}
SeosConfigBackend;


//------------------------------------------------------------------------------
// Filesystem Backend API
//------------------------------------------------------------------------------

#if defined(CONFIG_SERVER_BACKEND_FILESYSTEM)

// Management function: creates the given file to contain the specified backend
// layout.
seos_err_t
SeosConfigBackend_createFileBackend(
    SeosConfigBackend_FileName  name,
    hPartition_t                phandle,
    unsigned int                numberOfRecords,
    size_t                      sizeOfRecord);

// Initialize the given backend object with the backend layout retrieved from
// the given file.
seos_err_t
SeosConfigBackend_initializeFileBackend(
    SeosConfigBackend *         instance,
    SeosConfigBackend_FileName  name,
    hPartition_t                phandle);

#endif // CONFIG_SERVER_BACKEND_FILESYSTEM


//------------------------------------------------------------------------------
// Memory Backend API
//------------------------------------------------------------------------------

#if defined(CONFIG_SERVER_BACKEND_MEMORY)

// Management function: initializes the given buffer to contain the specified
// backend layout.
seos_err_t
SeosConfigBackend_createMemBackend(
    void *        buffer,
    size_t        bufferSize,
    unsigned int  numberOfRecords,
    size_t        sizeOfRecord);


// Management function: initializes the given buffer to contain the specified
// backend layout.
seos_err_t
SeosConfigBackend_createMemBackendAutoSized(
    void *  buffer,
    size_t  bufferSize,
    size_t  sizeOfRecord);


// Initialize the given backend object with the backend layout retrieved from
// the given buffer.
seos_err_t
SeosConfigBackend_initializeMemBackend(
    SeosConfigBackend *  instance,
    void *               buffer,
    size_t               bufferSize);

#endif //CONFIG_SERVER_BACKEND_MEMORY


//------------------------------------------------------------------------------
// Generic Function API
//------------------------------------------------------------------------------

unsigned int
SeosConfigBackend_getNumberOfRecords(
    SeosConfigBackend const *  instance);


size_t
SeosConfigBackend_getSizeOfRecords(
    SeosConfigBackend const *  instance);


seos_err_t
SeosConfigBackend_readRecord(
    SeosConfigBackend *  instance,
    unsigned int         recordIndex,
    void *               buf,
    size_t               bufLen);


seos_err_t
SeosConfigBackend_writeRecord(
    SeosConfigBackend *  instance,
    unsigned int         recordIndex,
    const void  *        buf,
    size_t               bufLen);
