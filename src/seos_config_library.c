/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#include <stdlib.h>

#include "seos_config_library.h"

// The library implementation does not have an instance store.
SeosConfigInstanceStore* 
library_seos_configuration_getInstances()
{
    return NULL;
}


seos_err_t 
library_seos_configuration_createHandle(
    SeosConfigLib_HandleKind handleKind,
    unsigned int id,
    SeosConfigHandle* handle)
{
    return SEOS_ERROR_INVALID_PARAMETER;
}


seos_err_t
library_seos_configuration_domainEnumeratorInit(
    SeosConfigHandle handle,
    SeosConfigLib_DomainEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        SeosConfigLib_domainEnumeratorInit(instance, enumerator);

        return SEOS_SUCCESS;
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_domainEnumeratorClose(SeosConfigHandle handle,
                                    SeosConfigLib_DomainEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        SeosConfigLib_domainEnumeratorClose(instance, enumerator);

        return SEOS_SUCCESS;
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_domainEnumeratorReset(SeosConfigHandle handle,
                                    SeosConfigLib_DomainEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        SeosConfigLib_domainEnumeratorReset(instance, enumerator);

        return SEOS_SUCCESS;
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_domainEnumeratorIncrement(SeosConfigHandle handle,
                                        SeosConfigLib_DomainEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_domainEnumeratorIncrement(instance, enumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_domainEnumeratorGetElement(
    SeosConfigHandle handle,
    SeosConfigLib_DomainEnumerator const* enumerator,
    SeosConfigLib_Domain* domain)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_domainEnumeratorGetElement(instance, enumerator, domain);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterEnumeratorInit(
    SeosConfigHandle handle,
    SeosConfigLib_DomainEnumerator const* domainEnumerator,
    SeosConfigLib_ParameterEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterEnumeratorInit(instance, domainEnumerator, enumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterEnumeratorClose(SeosConfigHandle handle,
                                       SeosConfigLib_ParameterEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        SeosConfigLib_parameterEnumeratorClose(instance, enumerator);

        return SEOS_SUCCESS;
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterEnumeratorReset(SeosConfigHandle handle,
                                       SeosConfigLib_ParameterEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterEnumeratorReset(instance, enumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterEnumeratorIncrement(SeosConfigHandle handle,
                                               SeosConfigLib_ParameterEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterEnumeratorIncrement(instance, enumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterEnumeratorGetElement(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_Parameter* parameter)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterEnumeratorGetElement(instance, enumerator, parameter);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


void
library_seos_configuration_domainGetName(
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_DomainName* domainName)
{
    SeosConfigLib_domainGetName(domain, domainName);
}


seos_err_t
library_seos_configuration_domainCreateParameterEnumerator(
    SeosConfigHandle handle,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_ParameterEnumerator* parameterEnumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_domainCreateParameterEnumerator(instance, domain, parameterName, parameterEnumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_domainGetElement(
    SeosConfigHandle handle,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_Parameter* parameter)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_domainGetElement(instance, domain, parameterName, parameter);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


void
library_seos_configuration_parameterGetName(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterName* parameterName)
{
    SeosConfigLib_parameterGetName(parameter, parameterName);
}


void
library_seos_configuration_parameterGetType(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterType* parameterType)
{
    SeosConfigLib_parameterGetType(parameter, parameterType);
}


size_t
library_seos_configuration_parameterGetSize(
    SeosConfigLib_Parameter const* parameter)
{
    return SeosConfigLib_parameterGetSize(parameter);
}


seos_err_t
library_seos_configuration_parameterGetValue(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterGetValue(instance, parameter, buffer, bufferLength, bytesCopied);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterGetValueAsU32(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    uint32_t* value)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterGetValueAsU32(instance, parameter, value);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterGetValueAsU64(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    uint64_t* value)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterGetValueAsU64(instance, parameter, value);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterGetValueAsString(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    char* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterGetValueAsString(instance, parameter, buffer, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterGetValueAsBlob(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    void* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterGetValueAsBlob(instance, parameter, buffer, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterSetValue(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterSetValue(instance, enumerator, parameterType, buffer, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterSetValueAsU32(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint32_t value)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterSetValueAsU32(instance, enumerator, value);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterSetValueAsU64(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint64_t value)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterSetValueAsU64(instance, enumerator, value);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterSetValueAsString(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    char const* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterSetValueAsString(instance, enumerator, parameterType, buffer, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
library_seos_configuration_parameterSetValueAsBlob(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_LOCAL == seos_configuration_handle_getHandleKind(handle))
    {
        SeosConfigLib* instance = (SeosConfigLib*)seos_configuration_handle_getLocalInstance(handle);
        return SeosConfigLib_parameterSetValueAsBlob(instance, enumerator, parameterType, buffer, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}
