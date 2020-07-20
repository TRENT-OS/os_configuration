/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include "LibDebug/Debug.h"
#include "OS_Error.h"
#include "OS_ConfigServiceBackend.h"

#include <string.h>
#include <inttypes.h>

//------------------------------------------------------------------------------
// Filesystem Backend API
//------------------------------------------------------------------------------

#if defined(OS_CONFIG_SERVICE_BACKEND_FILESYSTEM)

// we have to use exactly defined types here, so the layout of the data
// structure is well defined on 32-bit and 64-bit systems.
typedef struct
{
    uint32_t  numberOfRecords;
    uint64_t  sizeOfRecord;
}
OS_ConfigServiceBackend_BackendFsLayout_t;

/* Exported functions --------------------------------------------------------*/
static
OS_Error_t OS_ConfigServiceBackend_writeToFile(
    OS_FileSystem_Handle_t  hFs,
    const char*   name,
    unsigned int  offset,
    void*         buffer,
    int           length)
{
    OS_FileSystemFile_Handle_t hFile;

    // Open file
    OS_Error_t err = OS_FileSystemFile_open(
                         hFs,
                         &hFile,
                         name,
                         OS_FileSystem_OpenMode_WRONLY,
                         OS_FileSystem_OpenFlags_NONE);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystemFile_open() failed with: %d", err);
        return err;
    }

    Debug_LOG_DEBUG("file_write name:%s\n", name);
    Debug_LOG_DEBUG("file_write offset:%u\n", offset);
    Debug_LOG_DEBUG("file_write length:%d\n", length);

    // Call filesystem API function to write into a file
    err = OS_FileSystemFile_write(
              hFs,
              hFile,
              (long)offset,
              (long)length,
              buffer);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystemFile_write() failed with: %d", err);
        return err;
    }

    // Close this file
    err = OS_FileSystemFile_close(hFs, hFile);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystemFile_close() failed with: %d", err);
        return err;
    }

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
static
OS_Error_t OS_ConfigServiceBackend_readFromFile(
    OS_FileSystem_Handle_t  hFs,
    const char*   name,
    unsigned int  offset,
    void*         buffer,
    int           length)
{
    OS_FileSystemFile_Handle_t hFile;

    // Open file
    OS_Error_t err = OS_FileSystemFile_open(
                         hFs,
                         &hFile,
                         name,
                         OS_FileSystem_OpenMode_RDONLY,
                         OS_FileSystem_OpenFlags_NONE);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystemFile_open() failed with: %d", err);
        return err;
    }

    Debug_LOG_DEBUG("file_read name:%s\n", name);
    Debug_LOG_DEBUG("file_read offset:%u\n", offset);
    Debug_LOG_DEBUG("file_read length:%d\n", length);

    // Call filesystem API function to write into a file
    err = OS_FileSystemFile_read(
              hFs,
              hFile,
              (long)offset,
              (long)length,
              buffer);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystemFile_read() failed with: %d", err);
        return err;
    }

    // Close this file
    err = OS_FileSystemFile_close(hFs, hFile);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystemFile_close() failed with: %d", err);
        return err;
    }

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
static
OS_Error_t OS_ConfigServiceBackend_createFile(
    OS_FileSystem_Handle_t  hFs,
    const char*   name,
    int           length)
{
    enum {BLOCK_SIZE = 256};
    static char buf[256];
    OS_FileSystemFile_Handle_t hFile;

    // Open file
    OS_Error_t err = OS_FileSystemFile_open(
                         hFs,
                         &hFile,
                         name,
                         OS_FileSystem_OpenMode_RDWR,
                         OS_FileSystem_OpenFlags_CREATE);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystemFile_open() failed with: %d", err);
        return err;
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
        err = OS_FileSystemFile_write(
                  hFs,
                  hFile,
                  (long)fileSize,
                  (long)BLOCK_SIZE,
                  buf);
        if (OS_SUCCESS != err)
        {
            Debug_LOG_ERROR("OS_FileSystemFile_write() failed with: %d", err);
            return err;
        }

        fileSize += BLOCK_SIZE;
    }

    err = OS_FileSystemFile_write(
              hFs,
              hFile,
              (long)fileSize,
              (long)sizeOfLastBlock,
              buf);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystemFile_write() failed with: %d", err);
        return err;
    }

    // Close this file
    err = OS_FileSystemFile_close(hFs, hFile);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystemFile_close() failed with: %d", err);
        return err;
    }

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
static OS_Error_t
writeRecord_backend_filesystem(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    const void*          buf,
    size_t               bufLen)
{
    unsigned int offset = sizeof(OS_ConfigServiceBackend_BackendFsLayout_t) +
                          recordIndex *
                          instance->sizeOfRecord;

    OS_Error_t writeResult = OS_ConfigServiceBackend_writeToFile(
                                 instance->backend.fileSystem.hFs,
                                 instance->backend.fileSystem.name.buffer,
                                 offset,
                                 (void*)buf,
                                 bufLen);

    if (OS_SUCCESS != writeResult)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    }

    return writeResult;
}

//------------------------------------------------------------------------------
static OS_Error_t
readRecord_backend_filesystem(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    void*                buf,
    size_t               bufLen)
{
    unsigned int offset = sizeof(OS_ConfigServiceBackend_BackendFsLayout_t) +
                          recordIndex *
                          instance->sizeOfRecord;

    OS_Error_t readResult = OS_ConfigServiceBackend_readFromFile(
                                instance->backend.fileSystem.hFs,
                                instance->backend.fileSystem.name.buffer,
                                offset,
                                buf,
                                bufLen);

    if (OS_SUCCESS != readResult)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    }

    return readResult;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceBackend_createFileBackend(
    OS_ConfigServiceBackend_FileName_t  name,
    OS_FileSystem_Handle_t              hFs,
    unsigned int               numberOfRecords,
    size_t                     sizeOfRecord)
{
    size_t fileSize = sizeof(OS_ConfigServiceBackend_BackendFsLayout_t) +
                      numberOfRecords *
                      sizeOfRecord;
    OS_ConfigServiceBackend_BackendFsLayout_t backendFsLayout = {0};

    backendFsLayout.numberOfRecords = numberOfRecords;
    backendFsLayout.sizeOfRecord = sizeOfRecord;

    OS_Error_t result = OS_ConfigServiceBackend_createFile(hFs, name.buffer,
                                                           fileSize);
    if (OS_SUCCESS != result)
    {
        return result;
    }

    return OS_ConfigServiceBackend_writeToFile(
               hFs,
               name.buffer,
               0,
               &backendFsLayout,
               sizeof(OS_ConfigServiceBackend_BackendFsLayout_t));
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceBackend_initializeFileBackend(
    OS_ConfigServiceBackend_t*          instance,
    OS_ConfigServiceBackend_FileName_t  name,
    OS_FileSystem_Handle_t              hFs)
{
    OS_ConfigServiceBackend_BackendFsLayout_t backendFsLayout;

    if (OS_SUCCESS != OS_ConfigServiceBackend_readFromFile(
            hFs,
            name.buffer,
            0,
            &backendFsLayout,
            sizeof(OS_ConfigServiceBackend_BackendFsLayout_t)))
    {
        return OS_ERROR_GENERIC;
    }

    Debug_LOG_DEBUG("header of file backend read ok\n");
    Debug_LOG_DEBUG("number of records: %u\n", backendFsLayout.numberOfRecords);
    Debug_LOG_DEBUG("size of records: %" PRIu64"\n", backendFsLayout.sizeOfRecord);

    instance->backendType = OS_CONFIG_BACKEND_BACKEND_TYPE_FS;

    instance->backend.fileSystem.hFs = hFs;
    instance->backend.fileSystem.name = name;

    instance->numberOfRecords = backendFsLayout.numberOfRecords;
    instance->sizeOfRecord = backendFsLayout.sizeOfRecord;

    return OS_SUCCESS;
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
static OS_Error_t
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

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
static OS_Error_t
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

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
OS_Error_t
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

        return OS_SUCCESS;
    }
    else
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
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
OS_Error_t
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

    return OS_SUCCESS;
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
OS_Error_t
OS_ConfigServiceBackend_readRecord(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    void*                buf,
    size_t               bufLen)
{
    if (recordIndex >= instance->numberOfRecords)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (bufLen < instance->sizeOfRecord)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_INVALID_PARAMETER;
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
    return OS_ERROR_NOT_SUPPORTED;
}


//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceBackend_writeRecord(
    OS_ConfigServiceBackend_t*   instance,
    unsigned int         recordIndex,
    const void*          buf,
    size_t               bufLen)
{
    if (recordIndex >= instance->numberOfRecords)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (bufLen != instance->sizeOfRecord)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        Debug_LOG_DEBUG("Error: bufLen: %zu - instance->sizeOfRecord: %zu\n",
                        bufLen, instance->sizeOfRecord);
        return OS_ERROR_INVALID_PARAMETER;
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
    return OS_ERROR_NOT_SUPPORTED;
}
