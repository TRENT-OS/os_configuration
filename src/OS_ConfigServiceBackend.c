/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include "LibDebug/Debug.h"
#include "SeosError.h"
#include "OS_ConfigServiceBackend.h"

#include <string.h>

//------------------------------------------------------------------------------
// Filesystem Backend API
//------------------------------------------------------------------------------

#if defined(OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

typedef struct
{
    unsigned int  numberOfRecords;
    size_t        sizeOfRecord;
}
OS_ConfigServiceBackend_BackendFsLayout_t;

/* Exported functions --------------------------------------------------------*/
static
seos_err_t OS_ConfigServiceBackend_writeToFile(
    hPartition_t  phandle,
    const char*   name,
    unsigned int  offset,
    void*         buffer,
    int           length)
{
    hFile_t fhandle;
    seos_err_t file_stat = SEOS_SUCCESS;

    // Open file
    fhandle = OS_FilesystemApi_openFile(phandle, name, FA_WRITE);
    if (!OS_FilesystemApi_validateFileHandle(fhandle))
    {
        return SEOS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("file_write name:%s\n", name);
    Debug_LOG_DEBUG("file_write offset:%u\n", offset);
    Debug_LOG_DEBUG("file_write length:%d\n", length);

    // Call filesystem API function to write into a file
    file_stat = OS_FilesystemApi_writeFile(fhandle, (long)offset, (long)length, buffer);
    Debug_LOG_DEBUG("file_write:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    // Close this file
    file_stat = OS_FilesystemApi_closeFile(fhandle);
    Debug_LOG_DEBUG("file_close:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
static
seos_err_t OS_ConfigServiceBackend_readFromFile(
    hPartition_t  phandle,
    const char*   name,
    unsigned int  offset,
    void*         buffer,
    int           length)
{
    hFile_t fhandle;
    seos_err_t file_stat = SEOS_SUCCESS;

    // Open file
    fhandle = OS_FilesystemApi_openFile(phandle, name, FA_READ);
    if (!OS_FilesystemApi_validateFileHandle(fhandle))
    {
        return SEOS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("file_read name:%s\n", name);
    Debug_LOG_DEBUG("file_read offset:%u\n", offset);
    Debug_LOG_DEBUG("file_read length:%d\n", length);

    // Call filesystem API function to write into a file
    file_stat = OS_FilesystemApi_readFile(fhandle, (long)offset, (long)length, buffer);
    Debug_LOG_DEBUG("file_read:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    // Close this file
    file_stat = OS_FilesystemApi_closeFile(fhandle);
    Debug_LOG_DEBUG("file_close:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
static
seos_err_t OS_ConfigServiceBackend_createFile(
    hPartition_t  phandle,
    const char*   name,
    int           length)
{
    enum {BLOCK_SIZE = 256};
    static char buf[256];
    hFile_t fhandle;
    seos_err_t file_stat = SEOS_SUCCESS;

    // Open file
    fhandle = OS_FilesystemApi_openFile(phandle, name, FA_CREATE_ALWAYS | FA_WRITE);
    if (!OS_FilesystemApi_validateFileHandle(fhandle))
    {
        return SEOS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("file_create: size: %d\n", length);

    // fill buffer with test data
    memset(buf, 0, BLOCK_SIZE);

    // Call filesystem API function to write into a file
    unsigned int numberOfBlocks = length / BLOCK_SIZE;
    unsigned int sizeOfLastBlock = length % BLOCK_SIZE;
    unsigned int fileSize = 0;

    for (unsigned int k = 0; k < numberOfBlocks; ++k)
    {
        file_stat = OS_FilesystemApi_writeFile(fhandle, (long)fileSize, (long)BLOCK_SIZE, buf);
        Debug_LOG_DEBUG("file_write:%d\n", (uint8_t)file_stat);
        if (file_stat > 0)
        {
            return SEOS_ERROR_GENERIC;
        }

        fileSize += BLOCK_SIZE;
    }

    file_stat = OS_FilesystemApi_writeFile(fhandle, (long)fileSize, (long)sizeOfLastBlock, buf);
    Debug_LOG_DEBUG("file_write:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    // Close this file
    file_stat = OS_FilesystemApi_closeFile(fhandle);
    Debug_LOG_DEBUG("file_close:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
static seos_err_t
writeRecord_backend_filesystem(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    const void*          buf,
    size_t               bufLen)
{
    unsigned int offset = sizeof(OS_ConfigServiceBackend_BackendFsLayout_t) +
                          recordIndex *
                          instance->sizeOfRecord;

    seos_err_t writeResult = OS_ConfigServiceBackend_writeToFile(
                                 instance->backend.fileSystem.phandle,
                                 instance->backend.fileSystem.name.buffer,
                                 offset,
                                 (void*)buf,
                                 bufLen);

    if (SEOS_SUCCESS != writeResult)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    }

    return writeResult;
}

//------------------------------------------------------------------------------
static seos_err_t
readRecord_backend_filesystem(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    void*                buf,
    size_t               bufLen)
{
    unsigned int offset = sizeof(OS_ConfigServiceBackend_BackendFsLayout_t) +
                          recordIndex *
                          instance->sizeOfRecord;

    seos_err_t readResult = OS_ConfigServiceBackend_readFromFile(
                                instance->backend.fileSystem.phandle,
                                instance->backend.fileSystem.name.buffer,
                                offset,
                                buf,
                                bufLen);

    if (SEOS_SUCCESS != readResult)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    }

    return readResult;
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceBackend_createFileBackend(
    OS_ConfigServiceBackend_FileName_t  name,
    hPartition_t                phandle,
    unsigned int                numberOfRecords,
    size_t                     sizeOfRecord)
{
    size_t fileSize = sizeof(OS_ConfigServiceBackend_BackendFsLayout_t) +
                      numberOfRecords *
                      sizeOfRecord;
    OS_ConfigServiceBackend_BackendFsLayout_t backendFsLayout;

    backendFsLayout.numberOfRecords = numberOfRecords;
    backendFsLayout.sizeOfRecord = sizeOfRecord;

    seos_err_t result = OS_ConfigServiceBackend_createFile(phandle, name.buffer,
                                                           fileSize);
    if (SEOS_SUCCESS != result)
    {
        return result;
    }

    return OS_ConfigServiceBackend_writeToFile(
               phandle,
               name.buffer,
               0,
               &backendFsLayout,
               sizeof(OS_ConfigServiceBackend_BackendFsLayout_t));
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceBackend_initializeFileBackend(
    OS_ConfigServiceBackend_t*          instance,
    OS_ConfigServiceBackend_FileName_t  name,
    hPartition_t                phandle)
{
    OS_ConfigServiceBackend_BackendFsLayout_t backendFsLayout;

    if (SEOS_SUCCESS != OS_ConfigServiceBackend_readFromFile(
            phandle,
            name.buffer,
            0,
            &backendFsLayout,
            sizeof(OS_ConfigServiceBackend_BackendFsLayout_t)))
    {
        return SEOS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("header of file backend read ok\n");
    Debug_LOG_DEBUG("number of records: %u\n", backendFsLayout.numberOfRecords);
    Debug_LOG_DEBUG("size of records: %d\n", backendFsLayout.sizeOfRecord);

    instance->backendType = OS_CONFIG_BACKEND_BACKEND_TYPE_FS;

    instance->backend.fileSystem.phandle = phandle;
    instance->backend.fileSystem.name = name;

    instance->numberOfRecords = backendFsLayout.numberOfRecords;
    instance->sizeOfRecord = backendFsLayout.sizeOfRecord;

    return SEOS_SUCCESS;
}

#endif // OS_CONFIG_SERVICE_BACKEND_FILESYSTEM


//------------------------------------------------------------------------------
// Memory Backend API
//------------------------------------------------------------------------------

#if defined(OS_CONFIG_SERVICE_BACKEND_MEMORY)

typedef struct
{
    unsigned int  numberOfRecords;
    size_t        sizeOfRecord;
    size_t        bufferSize;
    char          buffer;
}
OS_ConfigServiceBackend_BackendMemLayout_t;

/* Exported functions --------------------------------------------------------*/
static seos_err_t
writeRecord_backend_memory(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    const void*          buf,
    size_t               bufLen)
{
    OS_ConfigServiceBackend_BackendMemLayout_t* memLayout =
        (OS_ConfigServiceBackend_BackendMemLayout_t*)(instance->backend.memory.buffer);
    char* record = &memLayout->buffer + instance->sizeOfRecord * recordIndex;
    memcpy(record, buf, instance->sizeOfRecord);

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
static seos_err_t
readRecord_backend_memory(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    void*                buf,
    size_t               bufLen)
{
    OS_ConfigServiceBackend_BackendMemLayout_t* memLayout =
        (OS_ConfigServiceBackend_BackendMemLayout_t*)(instance->backend.memory.buffer);
    char* record = &memLayout->buffer + instance->sizeOfRecord * recordIndex;
    memcpy(buf, record, instance->sizeOfRecord);

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceBackend_createMemBackend(
    void*         buffer,
    size_t        bufferSize,
    unsigned int  numberOfRecords,
    size_t        sizeOfRecord)
{
    if (sizeof(OS_ConfigServiceBackend_BackendMemLayout_t) + numberOfRecords*
        sizeOfRecord
        <= bufferSize)
    {
        OS_ConfigServiceBackend_BackendMemLayout_t* memLayout =
            (OS_ConfigServiceBackend_BackendMemLayout_t*)buffer;

        memLayout->numberOfRecords = numberOfRecords;
        memLayout->sizeOfRecord = sizeOfRecord;
        memLayout->bufferSize = bufferSize;

        return SEOS_SUCCESS;
    }
    else
    {
        return SEOS_ERROR_BUFFER_TOO_SMALL;
    }
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceBackend_createMemBackendAutoSized(
    void*   buffer,
    size_t  bufferSize,
    size_t  sizeOfRecord)
{
    unsigned int numberOfRecords = (bufferSize - sizeof(
                                        OS_ConfigServiceBackend_BackendMemLayout_t)) / sizeOfRecord;
    return OS_ConfigServiceBackend_createMemBackend(
               buffer,
               bufferSize,
               numberOfRecords,
               sizeOfRecord);
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceBackend_initializeMemBackend(
    OS_ConfigServiceBackend_t*   instance,
    void*                buffer,
    size_t               bufferSize)
{
    instance->backendType = OS_CONFIG_BACKEND_BACKEND_TYPE_MEM;

    instance->backend.memory.buffer = buffer;
    instance->backend.memory.bufferSize = bufferSize;

    OS_ConfigServiceBackend_BackendMemLayout_t* memLayout =
        (OS_ConfigServiceBackend_BackendMemLayout_t*)buffer;
    instance->numberOfRecords = memLayout->numberOfRecords;
    instance->sizeOfRecord = memLayout->sizeOfRecord;

    return SEOS_SUCCESS;
}

#endif // OS_CONFIG_SERVICE_BACKEND_MEMORY


//------------------------------------------------------------------------------
// Generic Function API
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
unsigned int
OS_ConfigServiceBackend_getNumberOfRecords(
    OS_ConfigServiceBackend_t const*   instance)
{
    return instance->numberOfRecords;
}


//------------------------------------------------------------------------------
size_t
OS_ConfigServiceBackend_getSizeOfRecords(
    OS_ConfigServiceBackend_t const*   instance)
{
    return instance->sizeOfRecord;
}


//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceBackend_readRecord(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    void*                buf,
    size_t               bufLen)
{
    if (recordIndex >= instance->numberOfRecords)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (bufLen < instance->sizeOfRecord)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    switch (instance->backendType)
    {

#if defined(OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

    case OS_CONFIG_SERVICE_BACKEND_FILESYSTEM:
        return readRecord_backend_filesystem(
                   instance,
                   recordIndex,
                   buf,
                   bufLen);

#endif // OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

#if defined(OS_CONFIG_SERVICE_BACKEND_MEMORY)

    case OS_CONFIG_BACKEND_BACKEND_TYPE_MEM:
        return readRecord_backend_memory(
                   instance,
                   recordIndex,
                   buf,
                   bufLen);

#endif // OS_CONFIG_SERVICE_BACKEND_MEMORY)

    default:
        break;
    } // end switch (instance->backendType)

    Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    return SEOS_ERROR_NOT_SUPPORTED;
}


//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceBackend_writeRecord(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    const void*          buf,
    size_t               bufLen)
{
    if (recordIndex >= instance->numberOfRecords)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (bufLen != instance->sizeOfRecord)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        Debug_LOG_DEBUG("Error: bufLen: %d - instance->sizeOfRecord: %d\n", bufLen,
                        instance->sizeOfRecord);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    switch (instance->backendType)
    {

#if defined(OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

    case OS_CONFIG_BACKEND_BACKEND_TYPE_FS:
        return writeRecord_backend_filesystem(
                   instance,
                   recordIndex,
                   buf,
                   bufLen);

#endif // OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

#if defined(OS_CONFIG_SERVICE_BACKEND_MEMORY)

    case OS_CONFIG_BACKEND_BACKEND_TYPE_MEM:
        return writeRecord_backend_memory(
                   instance,
                   recordIndex,
                   buf,
                   bufLen);

#endif // OS_CONFIG_SERVICE_BACKEND_MEMORY)

    default:
        break;
    } // end switch (instance->backendType)

    Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    return SEOS_ERROR_NOT_SUPPORTED;
}
