/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include <memory.h>

#include "OS_ConfigServiceLib.h"
#include "OS_ConfigServiceAppIdentifier.h"
#include "lib_debug/Debug.h"

#define LOG_NOT_INITIALIZED(func)\
    Debug_LOG_ERROR("initialization failed or missing, fail call %s()", (func))

/* Exported functions --------------------------------------------------------*/
static
OS_Error_t
OS_ConfigServiceLib_enumeratorRawIncrement(
    unsigned int maxIndex,
    uint32_t* index)
{
    OS_Error_t err;

    if (*index + 1 == maxIndex)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        Debug_LOG_DEBUG("Error: index: %u - maxIndex: %u\n", *index, maxIndex);
        err = OS_ERROR_GENERIC;
    }
    else
    {
        *index += 1;
        err = OS_SUCCESS;
    }

    return err;
}

//------------------------------------------------------------------------------
static
OS_Error_t
OS_ConfigServiceLib_parameterEnumeratorRawIncrement(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{

    return OS_ConfigServiceLib_enumeratorRawIncrement(
               OS_ConfigServiceBackend_getNumberOfRecords(&instance->parameterBackend),
               &enumerator->index);
}

//------------------------------------------------------------------------------
static
bool
OS_ConfigServiceLib_ParameterIsVisibleForMe(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter)
{
    unsigned int id;
    unsigned int maxId;

    OS_ConfigServiceAppIdentifier_WhoAmI(&id, &maxId);
    return (OS_ConfigServiceAccessRights_IsSet(
                &parameter->readAccess,
                id));
}

//------------------------------------------------------------------------------
static
bool
OS_ConfigServiceLib_ParameterIsWriteableForMe(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter)
{
    unsigned int id;
    unsigned int maxId;

    OS_ConfigServiceAppIdentifier_WhoAmI(&id, &maxId);
    return (OS_ConfigServiceAccessRights_IsSet(
                &parameter->writeAccess,
                id));
}

//------------------------------------------------------------------------------
// Advances the given parameter enumerator to the next parameter of the given domain.
// If the enumerator already points to a parameter of the given domain: do nothing.
static
OS_Error_t
OS_ConfigServiceLib_findParamter(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceLibTypes_ParameterEnumerator_t searchEnumerator = *enumerator;

    for (;;)
    {
        OS_Error_t err;

        OS_ConfigServiceLibTypes_Parameter_t parameter = {0};
        err = OS_ConfigServiceBackend_readRecord(
                  &instance->parameterBackend,
                  searchEnumerator.index,
                  &parameter,
                  sizeof(OS_ConfigServiceLibTypes_Parameter_t));

        if (OS_SUCCESS != err)
        {
            Debug_LOG_ERROR("OS_ConfigServiceBackend_readRecord() failed, error %d", err);
            return OS_ERROR_GENERIC;
        }

        if ((parameter.domain.index == searchEnumerator.domainEnumerator.index) &&
            (OS_ConfigServiceLib_ParameterIsVisibleForMe(&parameter)))
        {
            *enumerator = searchEnumerator;
            return OS_SUCCESS;
        }

        err = OS_ConfigServiceLib_parameterEnumeratorRawIncrement(
                  instance,
                  &searchEnumerator);
        if (OS_SUCCESS != err)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_parameterEnumeratorRawIncrement() failed, error %d",
                            err);
            return OS_ERROR_GENERIC;
        }
    }
}

//------------------------------------------------------------------------------
static
OS_Error_t
OS_ConfigServiceLib_advanceParameterEnumerator(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* searchEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* resultEnumerator)
{
    OS_Error_t findResult = OS_ConfigServiceLib_findParamter(
                                instance,
                                searchEnumerator);

    if (OS_SUCCESS == findResult)
    {
        *resultEnumerator = *searchEnumerator;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    }

    return findResult;
}

//------------------------------------------------------------------------------
static
OS_Error_t
OS_ConfigServiceLib_compareParameterName(
    OS_ConfigServiceLibTypes_ParameterName_t const* a,
    OS_ConfigServiceLibTypes_ParameterName_t const* b)
{
    for (unsigned int k = 0; k < OS_CONFIG_LIB_PARAMETER_NAME_LEN; ++k)
    {
        if (a->name[k] != b->name[k])
        {
            //Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return OS_ERROR_GENERIC;
        }
    }

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
static
OS_Error_t
OS_ConfigServiceLib_compareDomainName(
    OS_ConfigServiceLibTypes_DomainName_t const* a,
    OS_ConfigServiceLibTypes_DomainName_t const* b)
{
    for (unsigned int k = 0; k < OS_CONFIG_LIB_DOMAIN_NAME_LEN; ++k)
    {
        if (a->name[k] != b->name[k])
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return OS_ERROR_GENERIC;
        }
    }

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
static
OS_Error_t
OS_ConfigServiceLib_fetchVariableLengthString(
    OS_ConfigServiceBackend_t* backend,
    uint32_t index,
    size_t stringSize,
    void* buffer,
    size_t bufferLength)
{
    // We anticipate a maximum size here which should be ok to place on the stack.
    char tmpBuf[OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH];

    if (stringSize > bufferLength)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }

    OS_Error_t fetchResult = OS_ConfigServiceBackend_readRecord(
                                 backend,
                                 index,
                                 tmpBuf,
                                 sizeof(tmpBuf));

    if (OS_SUCCESS == fetchResult)
    {
        memcpy(buffer, tmpBuf, stringSize);
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    }

    return fetchResult;
}

//------------------------------------------------------------------------------
static
OS_Error_t
OS_ConfigServiceLib_fetchVariableLengthBlob(
    OS_ConfigServiceBackend_t* backend,
    size_t blobSize,
    uint32_t index,
    uint32_t numberOfBlocks,
    void* buffer,
    size_t bufferLength)
{
    if (blobSize > bufferLength)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }

    // We anticipate a maximum size here which should be ok to place on the stack.
    char tmpBuf[OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH];
    size_t blobBlockSize = OS_ConfigServiceBackend_getSizeOfRecords(backend);
    size_t bytesCopied = 0;

    while (bytesCopied < blobSize)
    {
        OS_Error_t fetchResult = OS_ConfigServiceBackend_readRecord(
                                     backend,
                                     index,
                                     tmpBuf,
                                     sizeof(tmpBuf));

        if (OS_SUCCESS != fetchResult)
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return OS_ERROR_GENERIC;
        }

        size_t bytesToCopy;

        if ((blobSize - bytesCopied) >= blobBlockSize)
        {
            bytesToCopy = blobBlockSize;
        }
        else
        {
            bytesToCopy = blobSize - bytesCopied;
        }

        memcpy((char*)buffer + bytesCopied, tmpBuf, bytesToCopy);

        bytesCopied += bytesToCopy;
        index++;
    }

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
static
OS_Error_t
OS_ConfigServiceLib_writeVariableLengthString(
    OS_ConfigServiceBackend_t* backend,
    uint32_t index,
    void const* buffer,
    size_t bufferLength)
{
    // We anticipate a maximum size here which should be ok to place on the stack.
    char tmpBuf[OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH];

    if (bufferLength > OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }

    memset(tmpBuf, 0, OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH);
    memcpy(tmpBuf, buffer, bufferLength);

    OS_Error_t writeResult = OS_ConfigServiceBackend_writeRecord(
                                 backend,
                                 index,
                                 tmpBuf,
                                 sizeof(tmpBuf));

    return writeResult;
}

//------------------------------------------------------------------------------
static
OS_Error_t
OS_ConfigServiceLib_writeVariableLengthBlob(
    OS_ConfigServiceBackend_t* backend,
    uint32_t index,
    uint32_t numberOfBlocks,
    void const* buffer,
    size_t bufferLength)
{
    size_t blobBlockSize = OS_ConfigServiceBackend_getSizeOfRecords(backend);
    size_t blobCapacity = blobBlockSize * numberOfBlocks;

    if (bufferLength > blobCapacity)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }

    // We anticipate a maximum size here which should be ok to place on the stack.
    char tmpBuf[OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH];
    size_t bytesCopied = 0;

    while (bytesCopied < bufferLength)
    {
        size_t bytesToCopy;

        if ((bufferLength - bytesCopied) >= blobBlockSize)
        {
            bytesToCopy = blobBlockSize;
        }
        else
        {
            bytesToCopy = bufferLength - bytesCopied;
        }

        memcpy(tmpBuf, (char*)buffer + bytesCopied, bytesToCopy);

        OS_Error_t fetchResult = OS_ConfigServiceBackend_writeRecord(
                                     backend,
                                     index,
                                     tmpBuf,
                                     sizeof(tmpBuf));

        if (OS_SUCCESS != fetchResult)
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return OS_ERROR_GENERIC;
        }

        bytesCopied += bytesToCopy;
        index++;
    }

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_Init(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceBackend_t const* parameterBackend,
    OS_ConfigServiceBackend_t const* domainBackend,
    OS_ConfigServiceBackend_t const* stringBackend,
    OS_ConfigServiceBackend_t const* blobBackend)
{
    instance->isInitialized = false;

    if (OS_ConfigServiceBackend_getSizeOfRecords(parameterBackend) != sizeof(
            OS_ConfigServiceLibTypes_Parameter_t))
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (OS_ConfigServiceBackend_getSizeOfRecords(domainBackend) != sizeof(
            OS_ConfigServiceLibTypes_Domain_t))
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (OS_ConfigServiceBackend_getSizeOfRecords(stringBackend) !=
        OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_INVALID_PARAMETER;
    }

    if (OS_ConfigServiceBackend_getSizeOfRecords(blobBackend) !=
        OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_INVALID_PARAMETER;
    }

    instance->parameterBackend = *parameterBackend;
    instance->domainBackend = *domainBackend;
    instance->stringBackend = *stringBackend;
    instance->blobBackend = *blobBackend;

    instance->isInitialized = true;

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_domainEnumeratorInit(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }
    OS_ConfigServiceLib_domainEnumeratorReset(instance, enumerator);
    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_domainEnumeratorClose(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }
    // No action required.
    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_domainEnumeratorReset(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }
    enumerator->index = 0;
    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_domainEnumeratorIncrement(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }
    // Store the packed index value in a temporary variable to avoid pointer
    // misalignment.
    uint32_t enumeratorIncrement = enumerator->index;

    OS_Error_t err = OS_ConfigServiceLib_enumeratorRawIncrement(
                         OS_ConfigServiceBackend_getNumberOfRecords(&instance->domainBackend),
                         &enumeratorIncrement);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return err;
    }

    // Increment the index with successfully incremented temporary variable.
    enumerator->index = enumeratorIncrement;
    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_domainEnumeratorGetElement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    OS_Error_t fetchResult = OS_ConfigServiceBackend_readRecord(
                                 &instance->domainBackend,
                                 enumerator->index,
                                 domain,
                                 sizeof(OS_ConfigServiceLibTypes_Domain_t));

    if (OS_SUCCESS == fetchResult)
    {
        domain->enumerator = *enumerator;
        return OS_SUCCESS;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_parameterEnumeratorInit(
    OS_ConfigServiceLib_t*  instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    OS_ConfigServiceLibTypes_ParameterEnumerator_t initEnumerator;
    OS_Error_t err;

    initEnumerator.domainEnumerator = *domainEnumerator;
    err = OS_ConfigServiceLib_parameterEnumeratorReset(
              instance,
              &initEnumerator);
    if (OS_SUCCESS == err)
    {
        *enumerator = initEnumerator;
    }

    return err;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_parameterEnumeratorClose(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }
    // No action required.
    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
// We start at the first parameter and starting from there try to find the first paramter of the given domain.
OS_Error_t
OS_ConfigServiceLib_parameterEnumeratorReset(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    OS_ConfigServiceLibTypes_ParameterEnumerator_t searchEnumerator;
    OS_Error_t err;

    searchEnumerator.index = 0;
    searchEnumerator.domainEnumerator = enumerator->domainEnumerator;
    err = OS_ConfigServiceLib_advanceParameterEnumerator(
              instance,
              &searchEnumerator,
              enumerator);
    if (OS_SUCCESS == err)
    {
        *enumerator = searchEnumerator;
    }

    return err;
}

//------------------------------------------------------------------------------
// We try to increment the given enumerator and start the search from there.
// If no parameter was found the given enumerator is not changed.
OS_Error_t OS_ConfigServiceLib_parameterEnumeratorIncrement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    OS_ConfigServiceLibTypes_ParameterEnumerator_t searchEnumerator = *enumerator;

    if (OS_SUCCESS == OS_ConfigServiceLib_parameterEnumeratorRawIncrement(
            instance,
            &searchEnumerator))
    {
        OS_Error_t err = OS_ConfigServiceLib_advanceParameterEnumerator(
                             instance,
                             &searchEnumerator,
                             enumerator);
        return err;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_parameterEnumeratorGetElement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    OS_ConfigServiceLibTypes_Parameter_t retrievedParameter;

    OS_Error_t fetchResult = OS_ConfigServiceBackend_readRecord(
                                 &instance->parameterBackend,
                                 enumerator->index,
                                 &retrievedParameter,
                                 sizeof(OS_ConfigServiceLibTypes_Parameter_t));

    if (OS_SUCCESS == fetchResult)
    {
        *parameter = retrievedParameter;
        return OS_SUCCESS;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }
}

//------------------------------------------------------------------------------
// Extracts the domain name from the given domain.
void
OS_ConfigServiceLib_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName)
{
    *domainName = domain->name;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_domainCreateParameterEnumerator(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    OS_ConfigServiceLibTypes_ParameterEnumerator_t searchEnumerator;
    OS_ConfigServiceLibTypes_Parameter_t searchParameter;

    OS_Error_t err = OS_ConfigServiceLib_parameterEnumeratorInit(
                         instance,
                         &domain->enumerator,
                         &searchEnumerator);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }

    for (;;)
    {
        err = OS_ConfigServiceLib_parameterEnumeratorGetElement(
                  instance,
                  &searchEnumerator,
                  &searchParameter);
        if (OS_SUCCESS != err)
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return OS_ERROR_GENERIC;
        }

        if (OS_SUCCESS == OS_ConfigServiceLib_compareParameterName(
                parameterName,
                &searchParameter.parameterName))
        {
            *parameterEnumerator = searchEnumerator;
            return OS_SUCCESS;
        }
        err = OS_ConfigServiceLib_parameterEnumeratorIncrement(
                  instance,
                  &searchEnumerator);
        if (OS_SUCCESS != err)
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return OS_ERROR_GENERIC;
        }
    } // end for(;;)
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_domainGetElement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    OS_ConfigServiceLibTypes_ParameterEnumerator_t parameterEnumerator;
    OS_ConfigServiceLibTypes_Parameter_t searchParameter;

    OS_Error_t err = OS_ConfigServiceLib_domainCreateParameterEnumerator(
                         instance,
                         domain,
                         parameterName,
                         &parameterEnumerator);

    if (OS_SUCCESS != err)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }

    err = OS_ConfigServiceLib_parameterEnumeratorGetElement(
              instance,
              &parameterEnumerator,
              &searchParameter);
    if (OS_SUCCESS == err)
    {
        *parameter = searchParameter;
        return OS_SUCCESS;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceLib_parameterGetName(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterName_t* parameterName)
{
    *parameterName = parameter->parameterName;
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceLib_parameterGetType(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t* parameterType)
{
    *parameterType = parameter->parameterType;
}

//------------------------------------------------------------------------------
size_t
OS_ConfigServiceLib_parameterGetSize(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter)
{
    switch (parameter->parameterType)
    {
    case OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32:
        return sizeof(uint32_t);
        break;

    case OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64:
        return sizeof(uint64_t);
        break;

    case OS_CONFIG_LIB_PARAMETER_TYPE_STRING:
        return parameter->parameterValue.valueString.size;
        break;

    case OS_CONFIG_LIB_PARAMETER_TYPE_BLOB:
        return parameter->parameterValue.valueBlob.size;
        break;

    default:
        return 0;
        break;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_parameterGetValue(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    size_t parameterSize = OS_ConfigServiceLib_parameterGetSize(parameter);

    if (parameterSize <= bufferLength)
    {
        switch (parameter->parameterType)
        {
        case OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32:
        case OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64:
            memcpy(buffer, &parameter->parameterValue, parameterSize);
            *bytesCopied = parameterSize;
            break;

        case OS_CONFIG_LIB_PARAMETER_TYPE_STRING:
        {
            OS_Error_t err = OS_ConfigServiceLib_fetchVariableLengthString(
                                 &instance->stringBackend,
                                 parameter->parameterValue.valueString.index,
                                 parameterSize,
                                 buffer,
                                 bufferLength);
            *bytesCopied = (err == OS_SUCCESS) ? parameterSize : 0;
        }
        break;

        case OS_CONFIG_LIB_PARAMETER_TYPE_BLOB:
        {
            OS_Error_t err = OS_ConfigServiceLib_fetchVariableLengthBlob(
                                 &instance->blobBackend,
                                 parameterSize,
                                 parameter->parameterValue.valueBlob.index,
                                 parameter->parameterValue.valueBlob.numberOfBlocks,
                                 buffer,
                                 bufferLength);
            *bytesCopied = (err == OS_SUCCESS) ? parameterSize : 0;
        }
        break;

        default:
            *bytesCopied = 0;
            break;
        }
    }
    else
    {
        *bytesCopied = 0;
    }

    if (bytesCopied <= 0)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    }

    return (bytesCopied > 0) ? OS_SUCCESS : OS_ERROR_GENERIC;
}

//------------------------------------------------------------------------------
// Get the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterGetValueAsU32(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    *value = parameter->parameterValue.valueInteger32;

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
// Get the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterGetValueAsU64(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    *value = parameter->parameterValue.valueInteger64;

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
// Get the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterGetValueAsString(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    char* buffer,
    size_t bufferLength)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    size_t bytesCopied;

    return OS_ConfigServiceLib_parameterGetValue(
               instance,
               parameter,
               buffer,
               bufferLength,
               &bytesCopied);
}

//------------------------------------------------------------------------------
// Get the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterGetValueAsBlob(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    size_t bytesCopied;

    return OS_ConfigServiceLib_parameterGetValue(
               instance,
               parameter,
               buffer,
               bufferLength,
               &bytesCopied);
}

//------------------------------------------------------------------------------
// Set the parameter value - raw format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValue(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    OS_ConfigServiceLibTypes_Parameter_t parameter;

    // Fetch the parameter pointed to by the enumerator.
    // Check the parameter type to be written is correct.
    // All integral types: handling is straight forward.
    // String and blob: check the given size is ok and
    // which may be smaller than the maximum record size.

    OS_Error_t err = OS_ConfigServiceLib_parameterEnumeratorGetElement(
                         instance,
                         enumerator,
                         &parameter);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }

    err = OS_ConfigServiceLib_ParameterIsWriteableForMe(&parameter) ? 0 : -1;
    if (OS_SUCCESS != err)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }

    if (parameterType != parameter.parameterType)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return OS_ERROR_GENERIC;
    }

    size_t parameterSize = OS_ConfigServiceLib_parameterGetSize(&parameter);

    switch (parameter.parameterType)
    {
    case OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32:
    case OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64:
        if (parameterSize == bufferLength)
        {
            memcpy(&parameter.parameterValue, buffer, parameterSize);
            err = OS_SUCCESS;
        }
        else
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            err = OS_ERROR_GENERIC;
        }
        break;

    case OS_CONFIG_LIB_PARAMETER_TYPE_STRING:
        parameter.parameterValue.valueString.size = bufferLength;

        err = OS_ConfigServiceLib_writeVariableLengthString(
                  &instance->stringBackend,
                  parameter.parameterValue.valueString.index,
                  buffer,
                  bufferLength);
        break;

    case OS_CONFIG_LIB_PARAMETER_TYPE_BLOB:
        parameter.parameterValue.valueBlob.size = bufferLength;

        err = OS_ConfigServiceLib_writeVariableLengthBlob(
                  &instance->blobBackend,
                  parameter.parameterValue.valueBlob.index,
                  parameter.parameterValue.valueBlob.numberOfBlocks,
                  buffer,
                  bufferLength);
        break;

    default:
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        err = OS_ERROR_GENERIC;
        break;
    }

    if (OS_SUCCESS == err)
    {
        err = OS_ConfigServiceBackend_writeRecord(
                  &instance->parameterBackend,
                  enumerator->index,
                  &parameter,
                  sizeof(parameter));
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        err = OS_ERROR_GENERIC;
    }

    return err;
}

//------------------------------------------------------------------------------
// Set the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValueAsU32(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    return OS_ConfigServiceLib_parameterSetValue(
               instance,
               enumerator,
               OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32,
               &value,
               sizeof(uint32_t));
}

//------------------------------------------------------------------------------
// Set the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValueAsU64(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    return OS_ConfigServiceLib_parameterSetValue(
               instance,
               enumerator,
               OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64,
               &value,
               sizeof(uint64_t));
}

//------------------------------------------------------------------------------
// Set the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValueAsString(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    char const* buffer,
    size_t bufferLength)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    return OS_ConfigServiceLib_parameterSetValue(
               instance,
               enumerator,
               OS_CONFIG_LIB_PARAMETER_TYPE_STRING,
               buffer,
               bufferLength);
}

//------------------------------------------------------------------------------
// Set the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValueAsBlob(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    return OS_ConfigServiceLib_parameterSetValue(
               instance,
               enumerator,
               OS_CONFIG_LIB_PARAMETER_TYPE_BLOB,
               buffer,
               bufferLength);
}

//------------------------------------------------------------------------------
static
OS_Error_t
find_parameter(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    OS_Error_t err;
    OS_ConfigServiceLibTypes_ParameterEnumerator_t paramEnumerator = {0};

    err = OS_ConfigServiceLib_parameterEnumeratorInit(
              instance,
              domainEnumerator,
              &paramEnumerator);
    if (0 != err)
    {
        Debug_LOG_ERROR("OS_ConfigServiceLib_parameterEnumeratorInit() failed, err %d",
                        err);
        return OS_ERROR_GENERIC;
    }

    for (;;)
    {
        err = OS_ConfigServiceLib_parameterEnumeratorGetElement(
                  instance,
                  &paramEnumerator,
                  parameter);
        if (0 != err)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_parameterEnumeratorGetElement() failed, err %d",
                            err);
            return OS_ERROR_GENERIC;
        }

        OS_ConfigServiceLibTypes_ParameterName_t parameterNameTmp;
        OS_ConfigServiceLib_parameterGetName(parameter, &parameterNameTmp);
        if (OS_SUCCESS == OS_ConfigServiceLib_compareParameterName(
                &parameterNameTmp,
                parameterName))
        {
            // enumerator holds the right paramter
            return OS_SUCCESS;
        }

        err = OS_ConfigServiceLib_parameterEnumeratorIncrement(
                  instance,
                  &paramEnumerator);
        if (0 != err)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_parameterEnumeratorIncrement() failed, err %d",
                            err);
            return OS_ERROR_GENERIC;
        }
    } // end for(;;)
}

//------------------------------------------------------------------------------
static
OS_Error_t
find_domain(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName)
{
    OS_Error_t err = OS_ConfigServiceLib_domainEnumeratorInit(instance,
                                                              enumerator);
    if (OS_SUCCESS != err)
    {
        return err;
    }

    for (;;)
    {
        OS_ConfigServiceLibTypes_Domain_t domain;
        err = OS_ConfigServiceLib_domainEnumeratorGetElement(
                  instance,
                  enumerator,
                  &domain);
        if (0 != err)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_domainEnumeratorGetElement() failed, err %d",
                            err);
            return OS_ERROR_GENERIC;
        }

        OS_ConfigServiceLibTypes_DomainName_t domainNameTmp;
        OS_ConfigServiceLib_domainGetName(&domain, &domainNameTmp);
        if (OS_SUCCESS == OS_ConfigServiceLib_compareDomainName(
                &domainNameTmp,
                domainName))
        {
            // enumerator holds the right domain
            return OS_SUCCESS;
        }

        err = OS_ConfigServiceLib_domainEnumeratorIncrement(instance, enumerator);
        if (0 != err)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_domainEnumeratorIncrement() failed, err %d",
                            err);
            return OS_ERROR_GENERIC;
        }
    } // end for(;;)
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLib_parameterGetValueFromDomainName(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    if (!instance->isInitialized)
    {
        LOG_NOT_INITIALIZED(__func__);
        return OS_ERROR_INVALID_STATE;
    }

    OS_Error_t err;

    OS_ConfigServiceLibTypes_DomainEnumerator_t domain_enumerator = {0};
    err = find_domain(instance, &domain_enumerator, domainName);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("find_domain() failed, err %d", err);
        return OS_ERROR_CONFIG_DOMAIN_NOT_FOUND;
    }

    OS_ConfigServiceLibTypes_Parameter_t parameter = {0};
    err = find_parameter(instance, &domain_enumerator, parameterName, &parameter);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("find_parameter() failed, err %d", err);
        return OS_ERROR_CONFIG_PARAMETER_NOT_FOUND;
    }

    if (parameterType != parameter.parameterType)
    {
        Debug_LOG_ERROR("parameter tpye mismatch, requested %d found %d",
                        parameterType, parameter.parameterType);
        return OS_ERROR_CONFIG_TYPE_MISMATCH;
    }

    err = OS_ConfigServiceLib_parameterGetValue(
              instance,
              &parameter,
              buffer,
              bufferLength,
              bytesCopied);
    if (err < 0)
    {
        Debug_LOG_ERROR("OS_ConfigServiceLib_parameterGetValue() failed, err %d", err);
        // ToDo: OS_ConfigServiceLib_parameterGetValue() should return error codes
        //       about the actual problem, so we can return them
        return OS_ERROR_GENERIC;
    }

    return OS_SUCCESS;
}
