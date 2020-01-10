/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */


#include "LibDebug/Debug.h"
#include "SeosError.h"
#include "SeosConfigBackend.h"

#if defined(CONFIG_SERVER_FILESYSTEM_BACKEND)

#include "seos_fs.h"

#endif // CONFIG_SERVER_FILESYSTEM_BACKEND

#include <string.h>


typedef struct SeosConfigBackend_BackendMemLayout
{
    unsigned int  numberOfRecords;
    size_t        sizeOfRecord;
    size_t        bufferSize;
    char          buffer;
}
SeosConfigBackend_BackendMemLayout;


typedef struct SeosConfigBackend_BackendFsLayout
{
    unsigned int  numberOfRecords;
    size_t        sizeOfRecord;
}
SeosConfigBackend_BackendFsLayout;


//------------------------------------------------------------------------------
// Filesystem Backend API
//------------------------------------------------------------------------------

#if defined(CONFIG_SERVER_FILESYSTEM_BACKEND)

//------------------------------------------------------------------------------
static
bool is_valid_file_handle(
    hFile_t fhandle)
{

#if defined (SEOS_FS_BUILD_AS_LIB)

    return (fhandle != NULL);

#else // not SEOS_FS_BUILD_AS_LIB

    return (fhandle >= 0); // ToDo: is 0 really a valid handle?

#endif // [not] SEOS_FS_BUILD_AS_LIB

}


//------------------------------------------------------------------------------
static
seos_err_t SeosConfigBackend_writeToFile(
    hPartition_t  phandle,
    const char*   name,
    unsigned int  offset,
    void*         buffer,
    int           length)
{
    hFile_t fhandle;
    seos_fs_result_t file_stat = SEOS_FS_SUCCESS;

    // Open file
    fhandle = file_open(phandle, name, FA_WRITE);
    if (!is_valid_file_handle(fhandle))
    {
        return SEOS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("file_write name:%s\n", name);
    Debug_LOG_DEBUG("file_write offset:%u\n", offset);
    Debug_LOG_DEBUG("file_write length:%d\n", length);

    // Call filesystem API function to write into a file
    file_stat = file_write(fhandle, (long)offset, (long)length, buffer);
    Debug_LOG_DEBUG("file_write:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    // Close this file
    file_stat = file_close(fhandle);
    Debug_LOG_DEBUG("file_close:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    return SEOS_SUCCESS;
}


//------------------------------------------------------------------------------
static
seos_err_t SeosConfigBackend_readFromFile(
    hPartition_t  phandle,
    const char*   name,
    unsigned int  offset,
    void*         buffer,
    int           length)
{
    hFile_t fhandle;
    seos_fs_result_t file_stat = SEOS_FS_SUCCESS;

    // Open file
    fhandle = file_open(phandle, name, FA_READ);
    if (!is_valid_file_handle(fhandle))
    {
        return SEOS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("file_read name:%s\n", name);
    Debug_LOG_DEBUG("file_read offset:%u\n", offset);
    Debug_LOG_DEBUG("file_read length:%d\n", length);

    // Call filesystem API function to write into a file
    file_stat = file_read(fhandle, (long)offset, (long)length, buffer);
    Debug_LOG_DEBUG("file_read:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    // Close this file
    file_stat = file_close(fhandle);
    Debug_LOG_DEBUG("file_close:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    return SEOS_SUCCESS;
}


//------------------------------------------------------------------------------
static
seos_err_t SeosConfigBackend_createFile(
    hPartition_t  phandle,
    const char*   name,
    int           length)
{
    enum {BLOCK_SIZE = 256};
    static char buf[256];
    hFile_t fhandle;
    seos_fs_result_t file_stat = SEOS_FS_SUCCESS;

    // Open file
    fhandle = file_open(phandle, name, FA_CREATE_ALWAYS | FA_WRITE);
    if (!is_valid_file_handle(fhandle))
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
        file_stat = file_write(fhandle, (long)fileSize, (long)BLOCK_SIZE, buf);
        Debug_LOG_DEBUG("file_write:%d\n", (uint8_t)file_stat);
        if (file_stat > 0)
        {
            return SEOS_ERROR_GENERIC;
        }

        fileSize += BLOCK_SIZE;
    }

    file_stat = file_write(fhandle, (long)fileSize, (long)sizeOfLastBlock, buf);
    Debug_LOG_DEBUG("file_write:%d\n", (uint8_t)file_stat);
    if (file_stat > 0)
    {
        return SEOS_ERROR_GENERIC;
    }

    // Close this file
    file_stat = file_close(fhandle);
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
    SeosConfigBackend*   instance,
    unsigned int         recordIndex,
    const void*          buf,
    size_t               bufLen)
{
    unsigned int offset = sizeof(SeosConfigBackend_BackendFsLayout) + recordIndex *
                          instance->sizeOfRecord;

    seos_err_t writeResult = SeosConfigBackend_writeToFile(
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
    SeosConfigBackend*   instance,
    unsigned int         recordIndex,
    const void*          buf,
    size_t               bufLen)
{
    unsigned int offset = sizeof(SeosConfigBackend_BackendFsLayout) + recordIndex *
                          instance->sizeOfRecord;

    seos_err_t readResult = SeosConfigBackend_readFromFile(
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
SeosConfigBackend_createFileBackend(
    SeosConfigBackend_FileName  name,
    hPartition_t                phandle,
    unsigned int                numberOfRecords,
    size_t                     sizeOfRecord)
{
    size_t fileSize = sizeof(SeosConfigBackend_BackendFsLayout) + numberOfRecords *
                      sizeOfRecord;
    SeosConfigBackend_BackendFsLayout backendFsLayout;

    backendFsLayout.numberOfRecords = numberOfRecords;
    backendFsLayout.sizeOfRecord = sizeOfRecord;

    seos_err_t result = SeosConfigBackend_createFile(phandle, name.buffer,
                                                     fileSize);
    if (SEOS_SUCCESS != result)
    {
        return result;
    }

    return SeosConfigBackend_writeToFile(phandle, name.buffer, 0, &backendFsLayout,
                                         sizeof(SeosConfigBackend_BackendFsLayout)))
}

//------------------------------------------------------------------------------
seos_err_t
SeosConfigBackend_initializeFileBackend(
    SeosConfigBackend*          instance,
    SeosConfigBackend_FileName  name,
    hPartition_t                phandle)
{
    SeosConfigBackend_BackendFsLayout backendFsLayout;

    if (!SeosConfigBackend_readFromFile(phandle, name.buffer, 0, &backendFsLayout,
                                        sizeof(SeosConfigBackend_BackendFsLayout)))
    {
        return SEOS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("header of file backend read ok\n");
    Debug_LOG_DEBUG("number of records: %u\n", backendFsLayout.numberOfRecords);
    Debug_LOG_DEBUG("size of records: %d\n", backendFsLayout.sizeOfRecord);

    instance->backendType = SEOS_CONFIG_BACKEND_BACKEND_TYPE_FS;

    instance->backend.fileSystem.phandle = phandle;
    instance->backend.fileSystem.name = name;

    instance->numberOfRecords = backendFsLayout.numberOfRecords;
    instance->sizeOfRecord = backendFsLayout.sizeOfRecord;

    return SEOS_SUCCESS;
}

#endif // CONFIG_SERVER_FILESYSTEM_BACKEND


//------------------------------------------------------------------------------
// Memory Backend API
//------------------------------------------------------------------------------

#if defined(CONFIG_SERVER_BACKEND_MEMORY)

//------------------------------------------------------------------------------
static seos_err_t
writeRecord_backend_memory(
    SeosConfigBackend*   instance,
    unsigned int         recordIndex,
    const void*          buf,
    size_t               bufLen)
{
    SeosConfigBackend_BackendMemLayout* memLayout =
        (SeosConfigBackend_BackendMemLayout*)(instance->backend.memory.buffer);
    char* record = &memLayout->buffer + instance->sizeOfRecord * recordIndex;
    memcpy(record, buf, instance->sizeOfRecord);

    return SEOS_SUCCESS;
}


//------------------------------------------------------------------------------
static seos_err_t
readRecord_backend_memory(
    SeosConfigBackend*   instance,
    unsigned int         recordIndex,
    void*                buf,
    size_t               bufLen)
{
    SeosConfigBackend_BackendMemLayout* memLayout =
        (SeosConfigBackend_BackendMemLayout*)(instance->backend.memory.buffer);
    char* record = &memLayout->buffer + instance->sizeOfRecord * recordIndex;
    memcpy(buf, record, instance->sizeOfRecord);

    return SEOS_SUCCESS;
}


//------------------------------------------------------------------------------
seos_err_t
SeosConfigBackend_createMemBackend(
    void*         buffer,
    size_t        bufferSize,
    unsigned int  numberOfRecords,
    size_t        sizeOfRecord)
{
    if (sizeof(SeosConfigBackend_BackendMemLayout) + numberOfRecords * sizeOfRecord
        <= bufferSize)
    {
        SeosConfigBackend_BackendMemLayout* memLayout =
            (SeosConfigBackend_BackendMemLayout*)buffer;

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
SeosConfigBackend_createMemBackendAutoSized(
    void*   buffer,
    size_t  bufferSize,
    size_t  sizeOfRecord)
{
    unsigned int numberOfRecords = (bufferSize - sizeof(
                                        SeosConfigBackend_BackendMemLayout)) / sizeOfRecord;
    return SeosConfigBackend_createMemBackend(
               buffer,
               bufferSize,
               numberOfRecords,
               sizeOfRecord);
}


//------------------------------------------------------------------------------
seos_err_t
SeosConfigBackend_initializeMemBackend(
    SeosConfigBackend*   instance,
    void*                buffer,
    size_t               bufferSize)
{
    instance->backendType = SEOS_CONFIG_BACKEND_BACKEND_TYPE_MEM;

    instance->backend.memory.buffer = buffer;
    instance->backend.memory.bufferSize = bufferSize;

    SeosConfigBackend_BackendMemLayout* memLayout =
        (SeosConfigBackend_BackendMemLayout*)buffer;
    instance->numberOfRecords = memLayout->numberOfRecords;
    instance->sizeOfRecord = memLayout->sizeOfRecord;

    return SEOS_SUCCESS;
}

#endif // CONFIG_SERVER_BACKEND_MEMORY


//------------------------------------------------------------------------------
// Generic Function API
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
unsigned int
SeosConfigBackend_getNumberOfRecords(
    SeosConfigBackend const*   instance)
{
    return instance->numberOfRecords;
}


//------------------------------------------------------------------------------
size_t
SeosConfigBackend_getSizeOfRecords(
    SeosConfigBackend const*   instance)
{
    return instance->sizeOfRecord;
}


//------------------------------------------------------------------------------
seos_err_t
SeosConfigBackend_readRecord(
    SeosConfigBackend*   instance,
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

#if defined(CONFIG_SERVER_FILESYSTEM_BACKEND)

    case CONFIG_SERVER_FILESYSTEM_BACKEND:
        return readRecord_backend_filesystem(
                   instance,
                   recordIndex,
                   buf,
                   bufLen);

#endif // CONFIG_SERVER_FILESYSTEM_BACKEND)

#if defined(CONFIG_SERVER_BACKEND_MEMORY)

    case SEOS_CONFIG_BACKEND_BACKEND_TYPE_MEM:
        return readRecord_backend_memory(
                   instance,
                   recordIndex,
                   buf,
                   bufLen);

#endif // CONFIG_SERVER_BACKEND_MEMORY)

    default:
        break;
    } // end switch (instance->backendType)

    Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    return SEOS_ERROR_NOT_SUPPORTED;
}


//------------------------------------------------------------------------------
seos_err_t
SeosConfigBackend_writeRecord(
    SeosConfigBackend*   instance,
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

#if defined(CONFIG_SERVER_FILESYSTEM_BACKEND)

    case CONFIG_SERVER_FILESYSTEM_BACKEND:
        return writeRecord_backend_filesystem(
                   instance,
                   recordIndex,
                   buf,
                   bufLen);

#endif // CONFIG_SERVER_FILESYSTEM_BACKEND)

#if defined(CONFIG_SERVER_BACKEND_MEMORY)

    case SEOS_CONFIG_BACKEND_BACKEND_TYPE_MEM:
        return writeRecord_backend_memory(
                   instance,
                   recordIndex,
                   buf,
                   bufLen);

#endif // CONFIG_SERVER_BACKEND_MEMORY)

    default:
        break;
    } // end switch (instance->backendType)

    Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    return SEOS_ERROR_NOT_SUPPORTED;
}
