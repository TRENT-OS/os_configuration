/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "SeosError.h"
#include "seos_fs_datatypes.h"


enum
{
    SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN  = (8 + 1 + 3 + 1)
};

typedef struct SeosConfigBackend_FileName
{
    char buffer[SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN];
}
SeosConfigBackend_FileName;

typedef enum
{
    SEOS_CONFIG_BACKEND_BACKEND_TYPE_FS,
    SEOS_CONFIG_BACKEND_BACKEND_TYPE_MEM,
}
SeosConfigBackend_BackendType;

typedef struct SeosConfigBackend
{
    SeosConfigBackend_BackendType backendType;
    union
    {
        struct
        {
            SeosConfigBackend_FileName name;
            hPartition_t phandle;
        } fileSystem;

        struct
        {
            void* buffer;
            size_t bufferSize;
        } memory;
    } backend;

    unsigned int numberOfRecords;
    size_t sizeOfRecord;
}
SeosConfigBackend;

// Management function: creates the given file to contain the specified backend layout.
seos_err_t
SeosConfigBackend_createFileBackend(SeosConfigBackend_FileName name,
                                    hPartition_t phandle, unsigned int numberOfRecords, size_t sizeOfRecord);

// Management function: initializes the given buffer to contain the specified backend layout.
seos_err_t
SeosConfigBackend_createMemBackend(void* buffer, size_t bufferSize,
                                   unsigned int numberOfRecords, size_t sizeOfRecord);

// Management function: initializes the given buffer to contain the specified backend layout.
seos_err_t
SeosConfigBackend_createMemBackendAutoSized(void* buffer, size_t bufferSize,
                                            size_t sizeOfRecord);

// Initialize the given backend object with the backend layout retrieved from the given file.
seos_err_t
SeosConfigBackend_initializeFileBackend(SeosConfigBackend* instance,
                                        SeosConfigBackend_FileName name, hPartition_t phandle);

// Initialize the given backend object with the backend layout retrieved from the given buffer.
seos_err_t
SeosConfigBackend_initializeMemBackend(SeosConfigBackend* instance,
                                       void* buffer, size_t bufferSize);

unsigned int
SeosConfigBackend_getNumberOfRecords(SeosConfigBackend const* instance);

size_t
SeosConfigBackend_getSizeOfRecords(SeosConfigBackend const* instance);

seos_err_t
SeosConfigBackend_readRecord(SeosConfigBackend* instance,
                             unsigned int recordIndex, void* buf, size_t bufLen);

seos_err_t
SeosConfigBackend_writeRecord(SeosConfigBackend* instance,
                              unsigned int recordIndex, const void* buf, size_t bufLen);
