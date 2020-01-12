/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#include <memory.h>

#include "SeosConfigLib.h"
#include "SeosConfigAppIdentifier.h"
#include "LibDebug/Debug.h"


static
seos_err_t
SeosConfigLib_enumeratorRawIncrement(unsigned int maxIndex, unsigned int* index)
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


static
seos_err_t
SeosConfigLib_parameterEnumeratorRawIncrement(SeosConfigLib const* instance,
                                              SeosConfigLib_ParameterEnumerator* enumerator)
{
    return SeosConfigLib_enumeratorRawIncrement(
               SeosConfigBackend_getNumberOfRecords(&instance->parameterBackend),
               &enumerator->index);
}


static
bool
SeosConfigLib_ParameterIsVisibleForMe(SeosConfigLib_Parameter const* parameter)
{
    unsigned int id;
    unsigned int maxId;

    SeosConfigAppIdentifier_WhoAmI(&id, &maxId);
    return (SeosConfigAccessRights_IsSet(
                &parameter->readAccess,
                id));
}


static
bool
SeosConfigLib_ParameterIsWriteableForMe(SeosConfigLib_Parameter const*
                                        parameter)
{
    unsigned int id;
    unsigned int maxId;

    SeosConfigAppIdentifier_WhoAmI(&id, &maxId);
    return (SeosConfigAccessRights_IsSet(
                &parameter->writeAccess,
                id));
}


// Advances the given parameter enumerator to the next parameter of the given domain.
// If the enumerator already points to a parameter of the given domain: do nothing.
static
seos_err_t
SeosConfigLib_findParamter(SeosConfigLib* instance,
                           SeosConfigLib_ParameterEnumerator* enumerator)
{
    SeosConfigLib_ParameterEnumerator searchEnumerator = *enumerator;

    for (;;)
    {
        seos_err_t ret;

        SeosConfigLib_Parameter parameter = {0};
        ret = SeosConfigBackend_readRecord(
                  &instance->parameterBackend,
                  searchEnumerator.index,
                  &parameter,
                  sizeof(SeosConfigLib_Parameter));

        if (SEOS_SUCCESS != ret)
        {
            Debug_LOG_ERROR("SeosConfigBackend_readRecord() failed, error %d", ret);
            return SEOS_ERROR_GENERIC;
        }

        if ((parameter.domain.index == searchEnumerator.domainEnumerator.index) &&
            (SeosConfigLib_ParameterIsVisibleForMe(&parameter)))
        {
            *enumerator = searchEnumerator;
            return SEOS_SUCCESS;
        }

        ret = SeosConfigLib_parameterEnumeratorRawIncrement(
                  instance,
                  &searchEnumerator);
        if (SEOS_SUCCESS != ret)
        {
            Debug_LOG_ERROR("SeosConfigLib_parameterEnumeratorRawIncrement() failed, error %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }
    }
}


static
seos_err_t
SeosConfigLib_advanceParameterEnumerator(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator* searchEnumerator,
    SeosConfigLib_ParameterEnumerator* resultEnumerator)
{
    seos_err_t findResult = SeosConfigLib_findParamter(instance, searchEnumerator);

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


static
seos_err_t
SeosConfigLib_compareParameterName(SeosConfigLib_ParameterName const* a,
                                   SeosConfigLib_ParameterName const* b)
{
    for (unsigned int k = 0; k < SEOS_CONFIG_LIB_PARAMETER_NAME_LEN; ++k)
    {
        if (a->name[k] != b->name[k])
        {
            //Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return SEOS_ERROR_GENERIC;
        }
    }

    return SEOS_SUCCESS;
}


static
seos_err_t
SeosConfigLib_compareDomainName(SeosConfigLib_DomainName const* a,
                                SeosConfigLib_DomainName const* b)
{
    for (unsigned int k = 0; k < SEOS_CONFIG_LIB_DOMAIN_NAME_LEN; ++k)
    {
        if (a->name[k] != b->name[k])
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return SEOS_ERROR_GENERIC;
        }
    }

    return SEOS_SUCCESS;
}


static
seos_err_t
SeosConfigLib_fetchVariableLengthString(
    SeosConfigBackend* backend,
    uint32_t index,
    size_t stringSize,
    void* buffer,
    size_t bufferLength)
{
    // We anticipate a maximum size here which should be ok to place on the stack.
    char tmpBuf[SEOS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH];

    if (stringSize > bufferLength)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }

    seos_err_t fetchResult = SeosConfigBackend_readRecord(
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


static
seos_err_t
SeosConfigLib_fetchVariableLengthBlob(
    SeosConfigBackend* backend,
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
    char tmpBuf[SEOS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH];
    size_t blobBlockSize = SeosConfigBackend_getSizeOfRecords(backend);
    size_t bytesCopied = 0;

    while (bytesCopied < blobSize)
    {
        seos_err_t fetchResult = SeosConfigBackend_readRecord(
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

        return SEOS_SUCCESS;
    }

    return SEOS_SUCCESS;
}


static
seos_err_t
SeosConfigLib_writeVariableLengthString(
    SeosConfigBackend* backend,
    uint32_t index,
    void const* buffer,
    size_t bufferLength)
{
    // We anticipate a maximum size here which should be ok to place on the stack.
    char tmpBuf[SEOS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH];

    if (bufferLength > SEOS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }

    memset(tmpBuf, 0, SEOS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH);
    memcpy(tmpBuf, buffer, bufferLength);

    seos_err_t writeResult = SeosConfigBackend_writeRecord(
                                 backend,
                                 index,
                                 tmpBuf,
                                 sizeof(tmpBuf));

    return writeResult;
}


static
seos_err_t
SeosConfigLib_writeVariableLengthBlob(
    SeosConfigBackend* backend,
    uint32_t index,
    uint32_t numberOfBlocks,
    void const* buffer,
    size_t bufferLength)
{
    size_t blobBlockSize = SeosConfigBackend_getSizeOfRecords(backend);
    size_t blobCapacity = blobBlockSize * numberOfBlocks;

    if (bufferLength > blobCapacity)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_GENERIC;
    }

    // We anticipate a maximum size here which should be ok to place on the stack.
    char tmpBuf[SEOS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH];
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

        seos_err_t fetchResult = SeosConfigBackend_writeRecord(
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

        return SEOS_SUCCESS;
    }

    return SEOS_SUCCESS;
}


int
SeosConfigLib_Init(
    SeosConfigLib* instance,
    SeosConfigBackend const* parameterBackend,
    SeosConfigBackend const* domainBackend,
    SeosConfigBackend const* stringBackend,
    SeosConfigBackend const* blobBackend)
{
    if (SeosConfigBackend_getSizeOfRecords(parameterBackend) != sizeof(
            SeosConfigLib_Parameter))
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (SeosConfigBackend_getSizeOfRecords(domainBackend) != sizeof(
            SeosConfigLib_Domain))
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (SeosConfigBackend_getSizeOfRecords(stringBackend) !=
        SEOS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    if (SeosConfigBackend_getSizeOfRecords(blobBackend) !=
        SEOS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH)
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


void
SeosConfigLib_domainEnumeratorInit(SeosConfigLib const* instance,
                                   SeosConfigLib_DomainEnumerator* enumerator)
{
    SeosConfigLib_domainEnumeratorReset(instance, enumerator);
}


void
SeosConfigLib_domainEnumeratorClose(SeosConfigLib const* instance,
                                    SeosConfigLib_DomainEnumerator* enumerator)
{
    // No action required.
}


void
SeosConfigLib_domainEnumeratorReset(SeosConfigLib const* instance,
                                    SeosConfigLib_DomainEnumerator* enumerator)
{
    enumerator->index = 0;
}


int
SeosConfigLib_domainEnumeratorIncrement(SeosConfigLib const* instance,
                                        SeosConfigLib_DomainEnumerator* enumerator)
{
    seos_err_t result = SeosConfigLib_enumeratorRawIncrement(
                            SeosConfigBackend_getNumberOfRecords(&instance->domainBackend),
                            &enumerator->index);
    return result == SEOS_SUCCESS ? 0 : -1;
}


int
SeosConfigLib_domainEnumeratorGetElement(
    SeosConfigLib* instance,
    SeosConfigLib_DomainEnumerator const* enumerator,
    SeosConfigLib_Domain* domain)
{
    seos_err_t fetchResult = SeosConfigBackend_readRecord(
                                 &instance->domainBackend,
                                 enumerator->index,
                                 domain,
                                 sizeof(SeosConfigLib_Domain));

    if (SEOS_SUCCESS == fetchResult)
    {
        domain->enumerator = *enumerator;
        return 0;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return -1;
    }
}


int
SeosConfigLib_parameterEnumeratorInit(
    SeosConfigLib*  instance,
    SeosConfigLib_DomainEnumerator const* domainEnumerator,
    SeosConfigLib_ParameterEnumerator* enumerator)
{
    SeosConfigLib_ParameterEnumerator initEnumerator;
    int result;

    initEnumerator.domainEnumerator = *domainEnumerator;
    result = SeosConfigLib_parameterEnumeratorReset(instance, &initEnumerator);
    if (0 == result)
    {
        *enumerator = initEnumerator;
    }

    return result;
}


void
SeosConfigLib_parameterEnumeratorClose(SeosConfigLib const* instance,
                                       SeosConfigLib_ParameterEnumerator* enumerator)
{
    // No action required.
}


// We start at the first parameter and starting from there try to find the first paramter of the given domain.
int
SeosConfigLib_parameterEnumeratorReset(SeosConfigLib* instance,
                                       SeosConfigLib_ParameterEnumerator* enumerator)
{
    SeosConfigLib_ParameterEnumerator searchEnumerator;
    seos_err_t result;

    searchEnumerator.index = 0;
    searchEnumerator.domainEnumerator = enumerator->domainEnumerator;
    result = SeosConfigLib_advanceParameterEnumerator(instance, &searchEnumerator,
                                                      enumerator);
    if (SEOS_SUCCESS == result)
    {
        *enumerator = searchEnumerator;
    }

    return result == SEOS_SUCCESS ? 0 : -1;
}


// We try to increment the given enumerator and start the search from there.
// If no parameter was found the given enumerator is not changed.
int SeosConfigLib_parameterEnumeratorIncrement(SeosConfigLib* instance,
                                               SeosConfigLib_ParameterEnumerator* enumerator)
{
    SeosConfigLib_ParameterEnumerator searchEnumerator = *enumerator;

    if (SEOS_SUCCESS == SeosConfigLib_parameterEnumeratorRawIncrement(instance,
            &searchEnumerator))
    {
        seos_err_t result = SeosConfigLib_advanceParameterEnumerator(instance,
                            &searchEnumerator,
                            enumerator);
        return result == SEOS_SUCCESS ? 0 : -1;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return -1;
    }
}


int
SeosConfigLib_parameterEnumeratorGetElement(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_Parameter* parameter)
{
    SeosConfigLib_Parameter retrievedParameter;

    seos_err_t fetchResult = SeosConfigBackend_readRecord(
                                 &instance->parameterBackend,
                                 enumerator->index,
                                 &retrievedParameter,
                                 sizeof(SeosConfigLib_Parameter));

    if (SEOS_SUCCESS == fetchResult)
    {
        *parameter = retrievedParameter;
        return 0;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return -1;
    }
}


// Extracts the domain name fromt he given domain.
void
SeosConfigLib_domainGetName(
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_DomainName* domainName)
{
    *domainName = domain->name;
}


int
SeosConfigLib_domainCreateParameterEnumerator(
    SeosConfigLib* instance,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_ParameterEnumerator* parameterEnumerator)
{
    SeosConfigLib_ParameterEnumerator searchEnumerator;
    SeosConfigLib_Parameter searchParameter;

    int result = SeosConfigLib_parameterEnumeratorInit(
                     instance,
                     &domain->enumerator,
                     &searchEnumerator);
    if (result != 0)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return -1;
    }

    bool parametersLeftToTry = true;
    while (parametersLeftToTry)
    {
        result = SeosConfigLib_parameterEnumeratorGetElement(
                     instance,
                     &searchEnumerator,
                     &searchParameter);
        if (result != 0)
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            return -1;
        }

        if (SEOS_SUCCESS == SeosConfigLib_compareParameterName(parameterName,
                                                               &searchParameter.parameterName))
        {
            *parameterEnumerator = searchEnumerator;
            return 0;
        }
        else
        {
            parametersLeftToTry = SEOS_SUCCESS ==
                                  SeosConfigLib_parameterEnumeratorRawIncrement(instance,
                                          &searchEnumerator);
        }
    }

    Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
    return -1;
}


int
SeosConfigLib_domainGetElement(
    SeosConfigLib* instance,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_Parameter* parameter)
{
    SeosConfigLib_ParameterEnumerator parameterEnumerator;
    SeosConfigLib_Parameter searchParameter;

    int result = SeosConfigLib_domainCreateParameterEnumerator(
                     instance,
                     domain,
                     parameterName,
                     &parameterEnumerator);

    if (result != 0)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return -1;
    }

    result = SeosConfigLib_parameterEnumeratorGetElement(
                 instance,
                 &parameterEnumerator,
                 &searchParameter);
    if (result == 0)
    {
        *parameter = searchParameter;
        return 0;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return -1;
    }
}


void
SeosConfigLib_parameterGetName(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterName* parameterName)
{
    *parameterName = parameter->parameterName;
}


void
SeosConfigLib_parameterGetType(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterType* parameterType)
{
    *parameterType = parameter->parameterType;
}


size_t
SeosConfigLib_parameterGetSize(
    SeosConfigLib_Parameter const* parameter)
{
    switch (parameter->parameterType)
    {
    case SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32:
        return sizeof(uint32_t);
        break;

    case SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64:
        return sizeof(uint64_t);
        break;

    case SEOS_CONFIG_LIB_PARAMETER_TYPE_STRING:
        return parameter->parameterValue.valueString.size;
        break;

    case SEOS_CONFIG_LIB_PARAMETER_TYPE_BLOB:
        return parameter->parameterValue.valueBlob.size;
        break;

    default:
        return 0;
        break;
    }
}


int
SeosConfigLib_parameterGetValue(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    size_t parameterSize = SeosConfigLib_parameterGetSize(parameter);

    if (parameterSize <= bufferLength)
    {
        switch (parameter->parameterType)
        {
        case SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32:
        case SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64:
            memcpy(buffer, &parameter->parameterValue, parameterSize);
            *bytesCopied = parameterSize;
            break;

        case SEOS_CONFIG_LIB_PARAMETER_TYPE_STRING:
        {
            seos_err_t result = SeosConfigLib_fetchVariableLengthString(
                                    &instance->stringBackend,
                                    parameter->parameterValue.valueString.index,
                                    parameterSize,
                                    buffer,
                                    bufferLength);
            *bytesCopied = (result == SEOS_SUCCESS) ? parameterSize : 0;
        }
        break;

        case SEOS_CONFIG_LIB_PARAMETER_TYPE_BLOB:
        {
            seos_err_t result = SeosConfigLib_fetchVariableLengthBlob(
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

    return (bytesCopied > 0) ? 0 : -1;
}


// Get the parameter value - specific format.
int
SeosConfigLib_parameterGetValueAsU32(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    uint32_t* value)
{
    *value = parameter->parameterValue.valueInteger32;

    return 0;
}


// Get the parameter value - specific format.
int
SeosConfigLib_parameterGetValueAsU64(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    uint64_t* value)
{
    *value = parameter->parameterValue.valueInteger64;

    return 0;
}


// Get the parameter value - specific format.
int
SeosConfigLib_parameterGetValueAsString(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    char* buffer,
    size_t bufferLength)
{
    size_t bytesCopied;

    return SeosConfigLib_parameterGetValue(
               instance,
               parameter,
               buffer,
               bufferLength,
               &bytesCopied);
}


// Get the parameter value - specific format.
int
SeosConfigLib_parameterGetValueAsBlob(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    void* buffer,
    size_t bufferLength)
{
    size_t bytesCopied;

    return SeosConfigLib_parameterGetValue(
               instance,
               parameter,
               buffer,
               bufferLength,
               &bytesCopied);
}


// Set the parameter value - raw format.
int
SeosConfigLib_parameterSetValue(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    void const* buffer,
    size_t bufferLength)
{
    SeosConfigLib_Parameter parameter;

    // Fetch the parameter pointed to by the enumerator.
    // Check the parameter type to be written is correct.
    // All integral types: handling is straight forward.
    // String and blob: check the given size is ok and
    // which may be smaller than the maximum record size.

    int result = SeosConfigLib_parameterEnumeratorGetElement(
                     instance,
                     enumerator,
                     &parameter);
    if (result != 0)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return -1;
    }

    result = SeosConfigLib_ParameterIsWriteableForMe(&parameter) ? 0 : -1;
    if (result != 0)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return -1;
    }

    if (parameterType != parameter.parameterType)
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        return -1;
    }

    size_t parameterSize = SeosConfigLib_parameterGetSize(&parameter);
    seos_err_t resultTmp;

    switch (parameter.parameterType)
    {
    case SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32:
    case SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64:
        if (parameterSize == bufferLength)
        {
            memcpy(&parameter.parameterValue, buffer, parameterSize);
            result = 0;
        }
        else
        {
            Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
            result = -1;
        }
        break;

    case SEOS_CONFIG_LIB_PARAMETER_TYPE_STRING:
        parameter.parameterValue.valueString.size = bufferLength;

        resultTmp = SeosConfigLib_writeVariableLengthString(
                        &instance->stringBackend,
                        parameter.parameterValue.valueString.index,
                        buffer,
                        bufferLength);
        result = resultTmp == SEOS_SUCCESS ? 0 : -1;
        break;

    case SEOS_CONFIG_LIB_PARAMETER_TYPE_BLOB:
        parameter.parameterValue.valueBlob.size = bufferLength;

        resultTmp = SeosConfigLib_writeVariableLengthBlob(
                        &instance->blobBackend,
                        parameter.parameterValue.valueBlob.index,
                        parameter.parameterValue.valueBlob.numberOfBlocks,
                        buffer,
                        bufferLength);
        result = resultTmp == SEOS_SUCCESS ? 0 : -1;
        break;

    default:
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        result = -1;
        break;
    }

    if (result == 0)
    {
        seos_err_t resultTmp = SeosConfigBackend_writeRecord(
                                   &instance->parameterBackend,
                                   enumerator->index,
                                   &parameter,
                                   sizeof(parameter));
        result = resultTmp == SEOS_SUCCESS ? 0 : -1;
    }
    else
    {
        Debug_LOG_DEBUG("Error: function: %s - line: %d\n", __FUNCTION__, __LINE__);
        result = -1;
    }

    return result;
}


// Set the parameter value - specific format.
int
SeosConfigLib_parameterSetValueAsU32(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint32_t value)
{
    return SeosConfigLib_parameterSetValue(
               instance,
               enumerator,
               SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32,
               &value,
               sizeof(uint32_t));
}


// Set the parameter value - specific format.
int
SeosConfigLib_parameterSetValueAsU64(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint64_t value)
{
    return SeosConfigLib_parameterSetValue(
               instance,
               enumerator,
               SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64,
               &value,
               sizeof(uint64_t));
}


// Set the parameter value - specific format.
int
SeosConfigLib_parameterSetValueAsString(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    char const* buffer,
    size_t bufferLength)
{
    return SeosConfigLib_parameterSetValue(
               instance,
               enumerator,
               SEOS_CONFIG_LIB_PARAMETER_TYPE_STRING,
               buffer,
               bufferLength);
}


// Set the parameter value - specific format.
int
SeosConfigLib_parameterSetValueAsBlob(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    void const* buffer,
    size_t bufferLength)
{
    return SeosConfigLib_parameterSetValue(
               instance,
               enumerator,
               SEOS_CONFIG_LIB_PARAMETER_TYPE_BLOB,
               buffer,
               bufferLength);
}


//------------------------------------------------------------------------------
static
seos_err_t
find_parameter(
    SeosConfigLib* instance,
    SeosConfigLib_DomainEnumerator* domainEnumerator,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_Parameter* parameter)
{
    int ret;
    SeosConfigLib_ParameterEnumerator paramEnumerator = {0};

    ret = SeosConfigLib_parameterEnumeratorInit(instance,
                                                domainEnumerator,
                                                &paramEnumerator);
    if (0 != ret)
    {
        Debug_LOG_ERROR("SeosConfigLib_parameterEnumeratorInit() failed, ret %d",
                        ret);
        return SEOS_ERROR_GENERIC;
    }

    for (;;)
    {
        ret = SeosConfigLib_parameterEnumeratorGetElement(instance,
                                                          &paramEnumerator,
                                                          parameter);
        if (0 != ret)
        {
            Debug_LOG_ERROR("SeosConfigLib_parameterEnumeratorGetElement() failed, ret %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }

        SeosConfigLib_ParameterName parameterNameTmp;
        SeosConfigLib_parameterGetName(parameter, &parameterNameTmp);
        if (SEOS_SUCCESS == SeosConfigLib_compareParameterName(&parameterNameTmp,
                                                               parameterName))
        {
            // enumerator holds the right paramter
            return SEOS_SUCCESS;
        }

        ret = SeosConfigLib_parameterEnumeratorIncrement(instance,
                                                         &paramEnumerator);
        if (0 != ret)
        {
            Debug_LOG_ERROR("SeosConfigLib_parameterEnumeratorIncrement() failed, ret %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }
    } // end for(;;)
}


//------------------------------------------------------------------------------
static
seos_err_t
find_domain(
    SeosConfigLib* instance,
    SeosConfigLib_DomainEnumerator* enumerator,
    SeosConfigLib_DomainName const* domainName)
{
    int ret;
    SeosConfigLib_domainEnumeratorInit(instance, enumerator);

    for (;;)
    {
        SeosConfigLib_Domain domain;
        ret = SeosConfigLib_domainEnumeratorGetElement(
                  instance,
                  enumerator,
                  &domain);
        if (0 != ret)
        {
            Debug_LOG_ERROR("SeosConfigLib_domainEnumeratorGetElement() failed, ret %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }

        SeosConfigLib_DomainName domainNameTmp;
        SeosConfigLib_domainGetName(&domain, &domainNameTmp);
        if (SEOS_SUCCESS == SeosConfigLib_compareDomainName(&domainNameTmp, domainName))
        {
            // enumerator holds the right domain
            return SEOS_SUCCESS;
        }

        ret = SeosConfigLib_domainEnumeratorIncrement(instance, enumerator);
        if (0 != ret)
        {
            Debug_LOG_ERROR("SeosConfigLib_domainEnumeratorIncrement() failed, ret %d",
                            ret);
            return SEOS_ERROR_GENERIC;
        }
    } // end for(;;)
}


//------------------------------------------------------------------------------

int
SeosConfigLib_parameterGetValueFromDomainName(
    SeosConfigLib* instance,
    SeosConfigLib_DomainName const* domainName,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_ParameterType parameterType,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    seos_err_t ret;

    SeosConfigLib_DomainEnumerator domain_enumerator = {0};
    ret = find_domain(instance, &domain_enumerator, domainName);
    if (SEOS_SUCCESS != ret)
    {
        Debug_LOG_ERROR("find_domain() failed, ret %d", ret);
        return SEOS_ERROR_CONFIG_DOMAIN_NOT_FOUND;
    }

    SeosConfigLib_Parameter parameter = {0};
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

    int err = SeosConfigLib_parameterGetValue(
                  instance,
                  &parameter,
                  buffer,
                  bufferLength,
                  bytesCopied);
    if (err <= 0)
    {
        Debug_LOG_ERROR("SeosConfigLib_parameterGetValue() failed, ret %d", ret);
        // ToDo: SeosConfigLib_parameterGetValue() should return error codes
        //       about the actual problem, so we can return them
        return SEOS_ERROR_GENERIC;
    }

    return SEOS_SUCCESS;
}
