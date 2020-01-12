/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#if defined (SEOS_CONFIG_CAMKES_SERVER) || !defined(SEOS_CONFIG_CAMKES_CLIENT)

#include "seos_config_server.h"
#include "seos_config_library.h"


typedef struct
{
    SeosConfigInstanceStore instanceStore;
}
SeosConfigServer;


static SeosConfigServer server;


static
seos_err_t
server_seos_configuration_transformRemoteHandleToLocalHandle(
    SeosConfigHandle remoteHandle,
    SeosConfigHandle* localHandle)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            remoteHandle))
    {
        SeosConfigLib* instance = seos_configuration_instance_store_getInstance(
                                      &server.instanceStore,
                                      seos_configuration_handle_getRemoteInstance(remoteHandle));

        if (instance != NULL)
        {
            seos_configuration_handle_initLocalHandle(
                (void*)instance,
                localHandle);

            return SEOS_SUCCESS;
        }
        else
        {
            return SEOS_ERROR_INVALID_PARAMETER;
        }
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


SeosConfigInstanceStore*
server_seos_configuration_getInstances()
{
    return &server.instanceStore;
}


seos_err_t
server_seos_configuration_createHandle(
    SeosConfigLib_HandleKind handleKind,
    unsigned int id,
    SeosConfigHandle* handle)
{
    if (handleKind == SEOS_CONFIG_HANDLE_KIND_RPC)
    {
        SeosConfigLib* instance = seos_configuration_instance_store_getInstance(
                                      &server.instanceStore,
                                      id);

        if (instance != NULL)
        {
            seos_configuration_handle_initRemoteHandle(
                id,
                handle);

            return SEOS_SUCCESS;
        }
        else
        {
            return SEOS_ERROR_INVALID_PARAMETER;
        }
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_domainEnumeratorInit(
    SeosConfigHandle handle,
    SeosConfigLib_DomainEnumerator* enumerator)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_domainEnumeratorInit(localHandle, enumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}

seos_err_t
server_seos_configuration_domainEnumeratorClose(SeosConfigHandle handle,
                                                SeosConfigLib_DomainEnumerator* enumerator)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        library_seos_configuration_domainEnumeratorClose(localHandle, enumerator);

        return SEOS_SUCCESS;
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_domainEnumeratorReset(SeosConfigHandle handle,
                                                SeosConfigLib_DomainEnumerator* enumerator)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        library_seos_configuration_domainEnumeratorReset(localHandle, enumerator);

        return SEOS_SUCCESS;
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_domainEnumeratorIncrement(SeosConfigHandle handle,
                                                    SeosConfigLib_DomainEnumerator* enumerator)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_domainEnumeratorIncrement(localHandle,
                                                                    enumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_domainEnumeratorGetElement(
    SeosConfigHandle handle,
    SeosConfigLib_DomainEnumerator const* enumerator,
    SeosConfigLib_Domain* domain)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_domainEnumeratorGetElement(localHandle,
                enumerator, domain);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterEnumeratorInit(
    SeosConfigHandle handle,
    SeosConfigLib_DomainEnumerator const* domainEnumerator,
    SeosConfigLib_ParameterEnumerator* enumerator)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_parameterEnumeratorInit(localHandle,
                                                                  domainEnumerator, enumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterEnumeratorClose(SeosConfigHandle handle,
                                                   SeosConfigLib_ParameterEnumerator* enumerator)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        library_seos_configuration_parameterEnumeratorClose(localHandle, enumerator);

        return SEOS_SUCCESS;
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterEnumeratorReset(SeosConfigHandle handle,
                                                   SeosConfigLib_ParameterEnumerator* enumerator)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_parameterEnumeratorReset(localHandle,
                                                                   enumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterEnumeratorIncrement(SeosConfigHandle handle,
                                                       SeosConfigLib_ParameterEnumerator* enumerator)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_parameterEnumeratorIncrement(localHandle,
                enumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterEnumeratorGetElement(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_Parameter* parameter)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_parameterEnumeratorGetElement(localHandle,
                enumerator, parameter);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


void
server_seos_configuration_domainGetName(
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_DomainName* domainName)
{
    library_seos_configuration_domainGetName(domain, domainName);
}


seos_err_t
server_seos_configuration_domainCreateParameterEnumerator(
    SeosConfigHandle handle,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_ParameterEnumerator* parameterEnumerator)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_domainCreateParameterEnumerator(localHandle,
                domain, parameterName, parameterEnumerator);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_domainGetElement(
    SeosConfigHandle handle,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_Parameter* parameter)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_domainGetElement(localHandle, domain,
                                                           parameterName, parameter);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


void
server_seos_configuration_parameterGetName(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterName* parameterName)
{
    library_seos_configuration_parameterGetName(parameter, parameterName);
}


void
server_seos_configuration_parameterGetType(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterType* parameterType)
{
    library_seos_configuration_parameterGetType(parameter, parameterType);
}


size_t
server_seos_configuration_parameterGetSize(
    SeosConfigLib_Parameter const* parameter)
{
    return library_seos_configuration_parameterGetSize(parameter);
}


seos_err_t
server_seos_configuration_parameterGetValue(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    dataport_ptr_t buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        return library_seos_configuration_parameterGetValue(localHandle, parameter,
                                                            tmpBuf, bufferLength, bytesCopied);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterGetValueAsU32(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    uint32_t* value)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_parameterGetValueAsU32(localHandle, parameter,
                                                                 value);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterGetValueAsU64(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    uint64_t* value)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_parameterGetValueAsU64(localHandle, parameter,
                                                                 value);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterGetValueAsString(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        return library_seos_configuration_parameterGetValueAsString(localHandle,
                                                                    parameter, tmpBuf, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterGetValueAsBlob(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        return library_seos_configuration_parameterGetValueAsBlob(localHandle,
                                                                  parameter, tmpBuf, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterSetValue(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        return library_seos_configuration_parameterSetValue(localHandle, enumerator,
                                                            parameterType, tmpBuf, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterSetValueAsU32(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint32_t value)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_parameterSetValueAsU32(localHandle,
                                                                 enumerator, value);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterSetValueAsU64(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint64_t value)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        return library_seos_configuration_parameterSetValueAsU64(localHandle,
                                                                 enumerator, value);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterSetValueAsString(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        return library_seos_configuration_parameterSetValueAsString(localHandle,
                                                                    enumerator, parameterType, tmpBuf, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterSetValueAsBlob(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    SeosConfigHandle localHandle;

    if (SEOS_SUCCESS ==
        server_seos_configuration_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        return library_seos_configuration_parameterSetValueAsBlob(localHandle,
                                                                  enumerator, parameterType, tmpBuf, bufferLength);
    }
    else
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }
}


seos_err_t
server_seos_configuration_parameterGetValueFromDomainName(
    SeosConfigHandle                   handle,
    const char*                        domain_name,
    const char*                        param_name,
    const SeosConfigLib_ParameterType  param_type,
    dataport_ptr_t                     buffer,
    size_t*                            len)
{

    SeosConfigHandle localHandle;
    seos_err_t ret = server_seos_configuration_transformRemoteHandleToLocalHandle(
                         handle,
                         &localHandle);
    if (SEOS_SUCCESS != ret)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    void* tmpBuf = dataport_unwrap_ptr(buffer);
    return library_seos_configuration_parameterGetValueFromDomainName(
               localHandle,
               domain_name,
               param_name,
               param_type,
               tmpBuf,
               len);
}

#endif
