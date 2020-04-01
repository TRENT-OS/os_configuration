/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include <memory.h>

#include "OS_ConfigServiceLib.h"
#include "OS_ConfigServiceAppIdentifier.h"
#include "LibDebug/Debug.h"

/* Exported functions --------------------------------------------------------*/
static
seos_err_t
OS_ConfigServiceLib_enumeratorRawIncrement(
    unsigned int maxIndex,
    unsigned int* index)
{
    seos_err_t result;


    if (*index + 1 == maxIndex)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        Debug_LOG_DEBUG("Error: index: %u - maxIndex: %u\n", *index, maxIndex);
        result = SEOS_ERROR_GENERIC;
    }
    else
    {
        *index += 1;
        result = SEOS_SUCCESS;
    }

    return result;
}

//------------------------------------------------------------------------------
static
seos_err_t
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
seos_err_t
OS_ConfigServiceLib_findParamter(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceLibTypes_ParameterEnumerator_t searchEnumerator = *enumerator;

    for (;;)
    {
        seos_err_t ret;

        OS_ConfigServiceLibTypes_Parameter_t parameter = {0};
        ret = OS_ConfigServiceBackend_readRecord(
                  &instance->parameterBackend,
                  searchEnumerator.index,
                  &parameter,
                  sizeof(OS_ConfigServiceLibTypes_Parameter_t));

        if (SEOS_SUCCESS != ret)
        {
            Debug_LOG_ERROR("OS_ConfigServiceBackend_readRecord() failed, error %d", ret);
            return SEOS_ERROR_GENERIC;
        }

        if ((parameter.domain.index == searchEnumerator.domainEnumerator.index) &&
            (OS_ConfigServiceLib_ParameterIsVisibleForMe(&parameter)))
        {
            *enumerator = searchEnumerator;
            return SEOS_SUCCESS;
        }

        ret = OS_ConfigServiceLib_parameterEnumeratorRawIncrement(
                  instance,
                  &searchEnumerator);
        if (SEOS_SUCCESS != ret)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_parameterEnumeratorRawIncrement() failed, error %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }
    }
}

//------------------------------------------------------------------------------
static
seos_err_t
OS_ConfigServiceLib_advanceParameterEnumerator(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* searchEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* resultEnumerator)
{
    seos_err_t findResult = OS_ConfigServiceLib_findParamter(
                                instance,
                                searchEnumerator);

    if (SEOS_SUCCESS == findResult)
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
seos_err_t
OS_ConfigServiceLib_compareParameterName(
    OS_ConfigServiceLibTypes_ParameterName_t const* a,
    OS_ConfigServiceLibTypes_ParameterName_t const* b)
{
    for (unsigned int k = 0; k < OS_CONFIG_LIB_PARAMETER_NAME_LEN; ++k)
    {
        if (a->name[k] != b->name[k])
        {
            //Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return SEOS_ERROR_GENERIC;
        }
    }

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
static
seos_err_t
OS_ConfigServiceLib_compareDomainName(
    OS_ConfigServiceLibTypes_DomainName_t const* a,
    OS_ConfigServiceLibTypes_DomainName_t const* b)
{
    for (unsigned int k = 0; k < OS_CONFIG_LIB_DOMAIN_NAME_LEN; ++k)
    {
        if (a->name[k] != b->name[k])
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return SEOS_ERROR_GENERIC;
        }
    }

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
static
seos_err_t
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
        return SEOS_ERROR_GENERIC;
    }

    seos_err_t fetchResult = OS_ConfigServiceBackend_readRecord(
                                 backend,
                                 index,
                                 tmpBuf,
                                 sizeof(tmpBuf));

    if (SEOS_SUCCESS == fetchResult)
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
seos_err_t
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
        return SEOS_ERROR_GENERIC;
    }

    // We anticipate a maximum size here which should be ok to place on the stack.
    char tmpBuf[OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH];
    size_t blobBlockSize = OS_ConfigServiceBackend_getSizeOfRecords(backend);
    size_t bytesCopied = 0;

    while (bytesCopied < blobSize)
    {
        seos_err_t fetchResult = OS_ConfigServiceBackend_readRecord(
                                     backend,
                                     index,
                                     tmpBuf,
                                     sizeof(tmpBuf));

        if (SEOS_SUCCESS != fetchResult)
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return SEOS_ERROR_GENERIC;
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

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
static
seos_err_t
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
        return SEOS_ERROR_GENERIC;
    }

    memset(tmpBuf, 0, OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH);
    memcpy(tmpBuf, buffer, bufferLength);

    seos_err_t writeResult = OS_ConfigServiceBackend_writeRecord(
                                 backend,
                                 index,
                                 tmpBuf,
                                 sizeof(tmpBuf));

    return writeResult;
}

//------------------------------------------------------------------------------
static
seos_err_t
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
        return SEOS_ERROR_GENERIC;
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

        seos_err_t fetchResult = OS_ConfigServiceBackend_writeRecord(
                                     backend,
                                     index,
                                     tmpBuf,
                                     sizeof(tmpBuf));

        if (SEOS_SUCCESS != fetchResult)
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return SEOS_ERROR_GENERIC;
        }

        bytesCopied += bytesToCopy;
        index++;
    }

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
int
OS_ConfigServiceLib_Init(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceBackend_t const* parameterBackend,
    OS_ConfigServiceBackend_t const* domainBackend,
    OS_ConfigServiceBackend_t const* stringBackend,
    OS_ConfigServiceBackend_t const* blobBackend)
{
    if (OS_ConfigServiceBackend_getSizeOfRecords(parameterBackend) != sizeof(
            OS_ConfigServiceLibTypes_Parameter_t))
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (OS_ConfigServiceBackend_getSizeOfRecords(domainBackend) != sizeof(
            OS_ConfigServiceLibTypes_Domain_t))
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (OS_ConfigServiceBackend_getSizeOfRecords(stringBackend) !=
        OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (OS_ConfigServiceBackend_getSizeOfRecords(blobBackend) !=
        OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    instance->parameterBackend = *parameterBackend;
    instance->domainBackend = *domainBackend;
    instance->stringBackend = *stringBackend;
    instance->blobBackend = *blobBackend;

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceLib_domainEnumeratorInit(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    OS_ConfigServiceLib_domainEnumeratorReset(instance, enumerator);
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceLib_domainEnumeratorClose(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    // No action required.
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceLib_domainEnumeratorReset(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    enumerator->index = 0;
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceLib_domainEnumeratorIncrement(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    seos_err_t result = OS_ConfigServiceLib_enumeratorRawIncrement(
                            OS_ConfigServiceBackend_getNumberOfRecords(&instance->domainBackend),
                            &enumerator->index);
    return result;
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceLib_domainEnumeratorGetElement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain)
{
    seos_err_t fetchResult = OS_ConfigServiceBackend_readRecord(
                                 &instance->domainBackend,
                                 enumerator->index,
                                 domain,
                                 sizeof(OS_ConfigServiceLibTypes_Domain_t));

    if (SEOS_SUCCESS == fetchResult)
    {
        domain->enumerator = *enumerator;
        return SEOS_SUCCESS;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceLib_parameterEnumeratorInit(
    OS_ConfigServiceLib_t*  instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceLibTypes_ParameterEnumerator_t initEnumerator;
    seos_err_t result;

    initEnumerator.domainEnumerator = *domainEnumerator;
    result = OS_ConfigServiceLib_parameterEnumeratorReset(
                 instance,
                 &initEnumerator);
    if (SEOS_SUCCESS == result)
    {
        *enumerator = initEnumerator;
    }

    return result;
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceLib_parameterEnumeratorClose(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    // No action required.
}

//------------------------------------------------------------------------------
// We start at the first parameter and starting from there try to find the first paramter of the given domain.
seos_err_t
OS_ConfigServiceLib_parameterEnumeratorReset(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceLibTypes_ParameterEnumerator_t searchEnumerator;
    seos_err_t result;

    searchEnumerator.index = 0;
    searchEnumerator.domainEnumerator = enumerator->domainEnumerator;
    result = OS_ConfigServiceLib_advanceParameterEnumerator(
                 instance,
                 &searchEnumerator,
                 enumerator);
    if (SEOS_SUCCESS == result)
    {
        *enumerator = searchEnumerator;
    }

    return result;
}

//------------------------------------------------------------------------------
// We try to increment the given enumerator and start the search from there.
// If no parameter was found the given enumerator is not changed.
seos_err_t OS_ConfigServiceLib_parameterEnumeratorIncrement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceLibTypes_ParameterEnumerator_t searchEnumerator = *enumerator;

    if (SEOS_SUCCESS == OS_ConfigServiceLib_parameterEnumeratorRawIncrement(
            instance,
            &searchEnumerator))
    {
        seos_err_t result = OS_ConfigServiceLib_advanceParameterEnumerator(
                                instance,
                                &searchEnumerator,
                                enumerator);
        return result;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceLib_parameterEnumeratorGetElement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    OS_ConfigServiceLibTypes_Parameter_t retrievedParameter;

    seos_err_t fetchResult = OS_ConfigServiceBackend_readRecord(
                                 &instance->parameterBackend,
                                 enumerator->index,
                                 &retrievedParameter,
                                 sizeof(OS_ConfigServiceLibTypes_Parameter_t));

    if (SEOS_SUCCESS == fetchResult)
    {
        *parameter = retrievedParameter;
        return SEOS_SUCCESS;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
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
seos_err_t
OS_ConfigServiceLib_domainCreateParameterEnumerator(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator)
{
    OS_ConfigServiceLibTypes_ParameterEnumerator_t searchEnumerator;
    OS_ConfigServiceLibTypes_Parameter_t searchParameter;

    seos_err_t result = OS_ConfigServiceLib_parameterEnumeratorInit(
                            instance,
                            &domain->enumerator,
                            &searchEnumerator);
    if (SEOS_SUCCESS != result)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }

    for (;;)
    {
        result = OS_ConfigServiceLib_parameterEnumeratorGetElement(
                     instance,
                     &searchEnumerator,
                     &searchParameter);
        if (SEOS_SUCCESS != result)
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return SEOS_ERROR_GENERIC;
        }

        if (SEOS_SUCCESS == OS_ConfigServiceLib_compareParameterName(
                parameterName,
                &searchParameter.parameterName))
        {
            *parameterEnumerator = searchEnumerator;
            return SEOS_SUCCESS;
        }
        result = OS_ConfigServiceLib_parameterEnumeratorIncrement(
                     instance,
                     &searchEnumerator);
        if (SEOS_SUCCESS != result)
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return SEOS_ERROR_GENERIC;
        }
    } // end for(;;)
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceLib_domainGetElement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    OS_ConfigServiceLibTypes_ParameterEnumerator_t parameterEnumerator;
    OS_ConfigServiceLibTypes_Parameter_t searchParameter;

    seos_err_t result = OS_ConfigServiceLib_domainCreateParameterEnumerator(
                            instance,
                            domain,
                            parameterName,
                            &parameterEnumerator);

    if (SEOS_SUCCESS != result)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }

    result = OS_ConfigServiceLib_parameterEnumeratorGetElement(
                 instance,
                 &parameterEnumerator,
                 &searchParameter);
    if (SEOS_SUCCESS == result)
    {
        *parameter = searchParameter;
        return SEOS_SUCCESS;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
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
seos_err_t
OS_ConfigServiceLib_parameterGetValue(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
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
            seos_err_t result = OS_ConfigServiceLib_fetchVariableLengthString(
                                    &instance->stringBackend,
                                    parameter->parameterValue.valueString.index,
                                    parameterSize,
                                    buffer,
                                    bufferLength);
            *bytesCopied = (result == SEOS_SUCCESS) ? parameterSize : 0;
        }
        break;

        case OS_CONFIG_LIB_PARAMETER_TYPE_BLOB:
        {
            seos_err_t result = OS_ConfigServiceLib_fetchVariableLengthBlob(
                                    &instance->blobBackend,
                                    parameterSize,
                                    parameter->parameterValue.valueBlob.index,
                                    parameter->parameterValue.valueBlob.numberOfBlocks,
                                    buffer,
                                    bufferLength);
            *bytesCopied = (result == SEOS_SUCCESS) ? parameterSize : 0;
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

    return (bytesCopied > 0) ? SEOS_SUCCESS : SEOS_ERROR_GENERIC;
}

//------------------------------------------------------------------------------
// Get the parameter value - specific format.
seos_err_t
OS_ConfigServiceLib_parameterGetValueAsU32(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value)
{
    *value = parameter->parameterValue.valueInteger32;

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
// Get the parameter value - specific format.
seos_err_t
OS_ConfigServiceLib_parameterGetValueAsU64(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value)
{
    *value = parameter->parameterValue.valueInteger64;

    return SEOS_SUCCESS;
}

//------------------------------------------------------------------------------
// Get the parameter value - specific format.
seos_err_t
OS_ConfigServiceLib_parameterGetValueAsString(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    char* buffer,
    size_t bufferLength)
{
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
seos_err_t
OS_ConfigServiceLib_parameterGetValueAsBlob(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength)
{
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
seos_err_t
OS_ConfigServiceLib_parameterSetValue(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength)
{
    OS_ConfigServiceLibTypes_Parameter_t parameter;

    // Fetch the parameter pointed to by the enumerator.
    // Check the parameter type to be written is correct.
    // All integral types: handling is straight forward.
    // String and blob: check the given size is ok and
    // which may be smaller than the maximum record size.

    int result = OS_ConfigServiceLib_parameterEnumeratorGetElement(
                     instance,
                     enumerator,
                     &parameter);
    if (SEOS_SUCCESS != result)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }

    result = OS_ConfigServiceLib_ParameterIsWriteableForMe(&parameter) ? 0 : -1;
    if (SEOS_SUCCESS != result)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }

    if (parameterType != parameter.parameterType)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }

    size_t parameterSize = OS_ConfigServiceLib_parameterGetSize(&parameter);

    switch (parameter.parameterType)
    {
    case OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32:
    case OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64:
        if (parameterSize == bufferLength)
        {
            memcpy(&parameter.parameterValue, buffer, parameterSize);
            result = SEOS_SUCCESS;
        }
        else
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            result = SEOS_ERROR_GENERIC;
        }
        break;

    case OS_CONFIG_LIB_PARAMETER_TYPE_STRING:
        parameter.parameterValue.valueString.size = bufferLength;

        result = OS_ConfigServiceLib_writeVariableLengthString(
                     &instance->stringBackend,
                     parameter.parameterValue.valueString.index,
                     buffer,
                     bufferLength);
        break;

    case OS_CONFIG_LIB_PARAMETER_TYPE_BLOB:
        parameter.parameterValue.valueBlob.size = bufferLength;

        result = OS_ConfigServiceLib_writeVariableLengthBlob(
                     &instance->blobBackend,
                     parameter.parameterValue.valueBlob.index,
                     parameter.parameterValue.valueBlob.numberOfBlocks,
                     buffer,
                     bufferLength);
        break;

    default:
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        result = SEOS_ERROR_GENERIC;
        break;
    }

    if (SEOS_SUCCESS == result)
    {
        result = OS_ConfigServiceBackend_writeRecord(
                     &instance->parameterBackend,
                     enumerator->index,
                     &parameter,
                     sizeof(parameter));
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        result = SEOS_ERROR_GENERIC;
    }

    return result;
}

//------------------------------------------------------------------------------
// Set the parameter value - specific format.
seos_err_t
OS_ConfigServiceLib_parameterSetValueAsU32(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value)
{
    return OS_ConfigServiceLib_parameterSetValue(
               instance,
               enumerator,
               OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32,
               &value,
               sizeof(uint32_t));
}

//------------------------------------------------------------------------------
// Set the parameter value - specific format.
seos_err_t
OS_ConfigServiceLib_parameterSetValueAsU64(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value)
{
    return OS_ConfigServiceLib_parameterSetValue(
               instance,
               enumerator,
               OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64,
               &value,
               sizeof(uint64_t));
}

//------------------------------------------------------------------------------
// Set the parameter value - specific format.
seos_err_t
OS_ConfigServiceLib_parameterSetValueAsString(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    char const* buffer,
    size_t bufferLength)
{
    return OS_ConfigServiceLib_parameterSetValue(
               instance,
               enumerator,
               OS_CONFIG_LIB_PARAMETER_TYPE_STRING,
               buffer,
               bufferLength);
}

//------------------------------------------------------------------------------
// Set the parameter value - specific format.
seos_err_t
OS_ConfigServiceLib_parameterSetValueAsBlob(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength)
{
    return OS_ConfigServiceLib_parameterSetValue(
               instance,
               enumerator,
               OS_CONFIG_LIB_PARAMETER_TYPE_BLOB,
               buffer,
               bufferLength);
}

//------------------------------------------------------------------------------
static
seos_err_t
find_parameter(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    seos_err_t ret;
    OS_ConfigServiceLibTypes_ParameterEnumerator_t paramEnumerator = {0};

    ret = OS_ConfigServiceLib_parameterEnumeratorInit(
              instance,
              domainEnumerator,
              &paramEnumerator);
    if (0 != ret)
    {
        Debug_LOG_ERROR("OS_ConfigServiceLib_parameterEnumeratorInit() failed, ret %d",
                        ret);
        return SEOS_ERROR_GENERIC;
    }

    for (;;)
    {
        ret = OS_ConfigServiceLib_parameterEnumeratorGetElement(
                  instance,
                  &paramEnumerator,
                  parameter);
        if (0 != ret)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_parameterEnumeratorGetElement() failed, ret %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }

        OS_ConfigServiceLibTypes_ParameterName_t parameterNameTmp;
        OS_ConfigServiceLib_parameterGetName(parameter, &parameterNameTmp);
        if (SEOS_SUCCESS == OS_ConfigServiceLib_compareParameterName(
                &parameterNameTmp,
                parameterName))
        {
            // enumerator holds the right paramter
            return SEOS_SUCCESS;
        }

        ret = OS_ConfigServiceLib_parameterEnumeratorIncrement(
                  instance,
                  &paramEnumerator);
        if (0 != ret)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_parameterEnumeratorIncrement() failed, ret %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }
    } // end for(;;)
}

//------------------------------------------------------------------------------
static
seos_err_t
find_domain(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName)
{
    int ret;
    OS_ConfigServiceLib_domainEnumeratorInit(instance, enumerator);

    for (;;)
    {
        OS_ConfigServiceLibTypes_Domain_t domain;
        ret = OS_ConfigServiceLib_domainEnumeratorGetElement(
                  instance,
                  enumerator,
                  &domain);
        if (0 != ret)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_domainEnumeratorGetElement() failed, ret %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }

        OS_ConfigServiceLibTypes_DomainName_t domainNameTmp;
        OS_ConfigServiceLib_domainGetName(&domain, &domainNameTmp);
        if (SEOS_SUCCESS == OS_ConfigServiceLib_compareDomainName(
                &domainNameTmp,
                domainName))
        {
            // enumerator holds the right domain
            return SEOS_SUCCESS;
        }

        ret = OS_ConfigServiceLib_domainEnumeratorIncrement(instance, enumerator);
        if (0 != ret)
        {
            Debug_LOG_ERROR("OS_ConfigServiceLib_domainEnumeratorIncrement() failed, ret %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }
    } // end for(;;)
}

//------------------------------------------------------------------------------
seos_err_t
OS_ConfigServiceLib_parameterGetValueFromDomainName(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    seos_err_t ret;

    OS_ConfigServiceLibTypes_DomainEnumerator_t domain_enumerator = {0};
    ret = find_domain(instance, &domain_enumerator, domainName);
    if (SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("find_domain() failed, ret %d", ret);
        return SEOS_ERROR_CONFIG_DOMAIN_NOT_FOUND;
    }

    OS_ConfigServiceLibTypes_Parameter_t parameter = {0};
    ret = find_parameter(instance, &domain_enumerator, parameterName, &parameter);
    if (SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("find_parameter() failed, ret %d", ret);
        return SEOS_ERROR_CONFIG_PARAMETER_NOT_FOUND;
    }

    if (parameterType != parameter.parameterType)
    {
        Debug_LOG_ERROR("parameter tpye mismatch, requested %d found %d",
                        parameterType, parameter.parameterType);
        return SEOS_ERROR_CONFIG_TYPE_MISMATCH;
    }

    int err = OS_ConfigServiceLib_parameterGetValue(
                  instance,
                  &parameter,
                  buffer,
                  bufferLength,
                  bytesCopied);
    if (err < 0)
    {
        Debug_LOG_ERROR("OS_ConfigServiceLib_parameterGetValue() failed, ret %d", ret);
        // ToDo: OS_ConfigServiceLib_parameterGetValue() should return error codes
        //       about the actual problem, so we can return them
        return SEOS_ERROR_GENERIC;
    }

    return SEOS_SUCCESS;
}