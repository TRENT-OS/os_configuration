/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>

#include "OS_ConfigServiceLibrary.h"

/* Exported functions --------------------------------------------------------*/
// The library implementation does not have an instance store.
OS_ConfigServiceInstanceStore_t*
OS_ConfigServiceLibrary_getInstances(void)
{
    return NULL;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_createHandle(
    OS_ConfigServiceHandle_HandleKind_t handleKind,
    unsigned int id,
    OS_ConfigServiceHandle_t* handle)
{
    return OS_ERROR_INVALID_PARAMETER;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_domainEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        OS_ConfigServiceLib_domainEnumeratorInit(instance, enumerator);

        return OS_SUCCESS;
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_domainEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        OS_ConfigServiceLib_domainEnumeratorClose(instance, enumerator);

        return OS_SUCCESS;
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_domainEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        OS_ConfigServiceLib_domainEnumeratorReset(instance, enumerator);

        return OS_SUCCESS;
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_domainEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_domainEnumeratorIncrement(instance, enumerator);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_domainEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_domainEnumeratorGetElement(
                   instance,
                   enumerator,
                   domain);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterEnumeratorInit(
                   instance,
                   domainEnumerator,
                   enumerator);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        OS_ConfigServiceLib_parameterEnumeratorClose(instance, enumerator);

        return OS_SUCCESS;
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterEnumeratorReset(instance, enumerator);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterEnumeratorIncrement(instance, enumerator);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterEnumeratorGetElement(
                   instance,
                   enumerator,
                   parameter);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceLibrary_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName)
{
    OS_ConfigServiceLib_domainGetName(domain, domainName);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_domainCreateParameterEnumerator(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_domainCreateParameterEnumerator(
                   instance,
                   domain,
                   parameterName,
                   parameterEnumerator);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_domainGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_domainGetElement(
                   instance,
                   domain,
                   parameterName,
                   parameter);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceLibrary_parameterGetName(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterName_t* parameterName)
{
    OS_ConfigServiceLib_parameterGetName(parameter, parameterName);
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceLibrary_parameterGetType(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t* parameterType)
{
    OS_ConfigServiceLib_parameterGetType(parameter, parameterType);
}

//------------------------------------------------------------------------------
size_t
OS_ConfigServiceLibrary_parameterGetSize(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter)
{
    return OS_ConfigServiceLib_parameterGetSize(parameter);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterGetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterGetValue(
                   instance,
                   parameter,
                   buffer,
                   bufferLength,
                   bytesCopied);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterGetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterGetValueAsU32(instance, parameter, value);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterGetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterGetValueAsU64(instance, parameter, value);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterGetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    char* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterGetValueAsString(
                   instance,
                   parameter,
                   buffer,
                   bufferLength);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterGetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterGetValueAsBlob(
                   instance,
                   parameter,
                   buffer,
                   bufferLength);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterSetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterSetValue(
                   instance,
                   enumerator,
                   parameterType,
                   buffer,
                   bufferLength);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterSetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterSetValueAsU32(instance, enumerator, value);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterSetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterSetValueAsU64(instance, enumerator, value);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterSetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    char const* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterSetValueAsString(
                   instance,
                   enumerator,
                   parameterType,
                   buffer,
                   bufferLength);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterSetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterSetValueAsBlob(
                   instance,
                   enumerator,
                   parameterType,
                   buffer,
                   bufferLength);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceLibrary_parameterGetValueFromDomainName(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    if (OS_CONFIG_HANDLE_KIND_LOCAL == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
        OS_ConfigServiceLib_t* instance = (OS_ConfigServiceLib_t*)
                                          OS_ConfigServiceHandle_getLocalInstance(handle);
        return OS_ConfigServiceLib_parameterGetValueFromDomainName(
                   instance,
                   domainName,
                   parameterName,
                   parameterType,
                   buffer,
                   bufferLength,
                   bytesCopied);
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}
