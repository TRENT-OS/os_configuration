/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#if defined(SEOS_CONFIG_CAMKES_CLIENT)
#include "camkes.h"
#endif

#include "string.h"

#include "seos_config_client.h"
#include "seos_config_server.h"
#include "seos_config_library.h"


typedef struct
{
    SeosConfigInstanceStore instanceStore;
}
SeosConfigClient;


static SeosConfigClient client;


SeosConfigInstanceStore*
seos_configuration_getInstances()
{
    return &client.instanceStore;
}

seos_err_t
seos_configuration_createHandle(
    SeosConfigLib_HandleKind handleKind,
    unsigned int id,
    SeosConfigHandle* handle)
{
    if (handleKind == SEOS_CONFIG_HANDLE_KIND_RPC)
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_createHandle(handleKind, id, handle);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        SeosConfigLib* instance = seos_configuration_instance_store_getInstance(
                                      &client.instanceStore,
                                      id);

        if (instance != NULL)
        {
            seos_configuration_handle_initLocalHandle(
                (void*)instance,
                handle);

            return SEOS_SUCCESS;
        }
        else
        {
            return SEOS_ERROR_INVALID_PARAMETER;
        }
    }
}


seos_err_t
seos_configuration_domainEnumeratorInit(
    SeosConfigHandle handle,
    SeosConfigLib_DomainEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_domainEnumeratorInit(handle, enumerator);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_domainEnumeratorInit(handle, enumerator);
    }
}


seos_err_t
seos_configuration_domainEnumeratorClose(SeosConfigHandle handle,
                                         SeosConfigLib_DomainEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_domainEnumeratorClose(handle, enumerator);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_domainEnumeratorClose(handle, enumerator);
    }
}


seos_err_t
seos_configuration_domainEnumeratorReset(SeosConfigHandle handle,
                                         SeosConfigLib_DomainEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_domainEnumeratorReset(handle, enumerator);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_domainEnumeratorReset(handle, enumerator);
    }
}


seos_err_t
seos_configuration_domainEnumeratorIncrement(SeosConfigHandle handle,
                                             SeosConfigLib_DomainEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_domainEnumeratorIncrement(handle, enumerator);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_domainEnumeratorIncrement(handle, enumerator);
    }
}


seos_err_t
seos_configuration_domainEnumeratorGetElement(
    SeosConfigHandle handle,
    SeosConfigLib_DomainEnumerator const* enumerator,
    SeosConfigLib_Domain* domain)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_domainEnumeratorGetElement(handle, enumerator,
                                                                    domain);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_domainEnumeratorGetElement(handle, enumerator,
                domain);
    }
}


seos_err_t
seos_configuration_parameterEnumeratorInit(
    SeosConfigHandle handle,
    SeosConfigLib_DomainEnumerator const* domainEnumerator,
    SeosConfigLib_ParameterEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_parameterEnumeratorInit(handle,
                                                                 domainEnumerator, enumerator);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterEnumeratorInit(handle,
                                                                  domainEnumerator, enumerator);
    }
}


seos_err_t
seos_configuration_parameterEnumeratorClose(SeosConfigHandle handle,
                                            SeosConfigLib_ParameterEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_parameterEnumeratorClose(handle, enumerator);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterEnumeratorClose(handle, enumerator);
    }
}


seos_err_t
seos_configuration_parameterEnumeratorReset(SeosConfigHandle handle,
                                            SeosConfigLib_ParameterEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_parameterEnumeratorReset(handle, enumerator);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterEnumeratorReset(handle, enumerator);
    }
}


seos_err_t
seos_configuration_parameterEnumeratorIncrement(SeosConfigHandle handle,
                                                SeosConfigLib_ParameterEnumerator* enumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_parameterEnumeratorIncrement(handle,
                enumerator);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterEnumeratorIncrement(handle,
                enumerator);
    }
}


seos_err_t
seos_configuration_parameterEnumeratorGetElement(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_Parameter* parameter)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_parameterEnumeratorGetElement(handle,
                enumerator, parameter);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterEnumeratorGetElement(handle,
                enumerator, parameter);
    }
}


void
seos_configuration_domainGetName(
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_DomainName* domainName)
{
    library_seos_configuration_domainGetName(domain, domainName);
}


seos_err_t
seos_configuration_domainCreateParameterEnumerator(
    SeosConfigHandle handle,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_ParameterEnumerator* parameterEnumerator)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_domainCreateParameterEnumerator(handle, domain,
                parameterName, parameterEnumerator);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_domainCreateParameterEnumerator(handle,
                domain, parameterName, parameterEnumerator);
    }
}


seos_err_t
seos_configuration_domainGetElement(
    SeosConfigHandle handle,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_Parameter* parameter)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_domainGetElement(handle, domain, parameterName,
                                                          parameter);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_domainGetElement(handle, domain,
                                                           parameterName, parameter);
    }
}


void
seos_configuration_parameterGetName(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterName* parameterName)
{
    library_seos_configuration_parameterGetName(parameter, parameterName);
}


void
seos_configuration_parameterGetType(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterType* parameterType)
{
    library_seos_configuration_parameterGetType(parameter, parameterType);
}


size_t
seos_configuration_parameterGetSize(
    SeosConfigLib_Parameter const* parameter)
{
    return library_seos_configuration_parameterGetSize(parameter);
}


seos_err_t
seos_configuration_parameterGetValue(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        static dataport_ptr_t bufferReceive;

        bufferReceive = dataport_wrap_ptr((void*)cfg_dataport_buf);

        int result =
            server_seos_configuration_parameterGetValue(handle, parameter, bufferReceive,
                                                        bufferLength, bytesCopied);

        if (result == SEOS_SUCCESS)
        {
            const void* buf = dataport_unwrap_ptr(bufferReceive);
            memcpy(buffer, buf, *bytesCopied);
        }

        return result;
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterGetValue(handle, parameter, buffer,
                                                            bufferLength, bytesCopied);
    }
}


seos_err_t
seos_configuration_parameterGetValueAsU32(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    uint32_t* value)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_parameterGetValueAsU32(handle, parameter,
                                                                value);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterGetValueAsU32(handle, parameter,
                                                                 value);
    }
}


seos_err_t
seos_configuration_parameterGetValueAsU64(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    uint64_t* value)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_parameterGetValueAsU64(handle, parameter,
                                                                value);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterGetValueAsU64(handle, parameter,
                                                                 value);
    }
}


seos_err_t
seos_configuration_parameterGetValueAsString(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    char* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        static dataport_ptr_t bufferReceive;

        bufferReceive = dataport_wrap_ptr((void*)cfg_dataport_buf);

        int result =
            server_seos_configuration_parameterGetValueAsString(handle, parameter,
                                                                bufferReceive, bufferLength);

        if (result == SEOS_SUCCESS)
        {
            const void* buf = dataport_unwrap_ptr(bufferReceive);
            memcpy(buffer, buf, bufferLength);
        }

        return result;
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterGetValueAsString(handle, parameter,
                                                                    buffer, bufferLength);
    }
}


seos_err_t
seos_configuration_parameterGetValueAsBlob(
    SeosConfigHandle handle,
    SeosConfigLib_Parameter const* parameter,
    void* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        static dataport_ptr_t bufferReceive;

        bufferReceive = dataport_wrap_ptr((void*)cfg_dataport_buf);

        int result =
            server_seos_configuration_parameterGetValueAsBlob(handle, parameter,
                                                              bufferReceive, bufferLength);

        if (result == SEOS_SUCCESS)
        {
            const void* buf = dataport_unwrap_ptr(bufferReceive);
            memcpy(buffer, buf, bufferLength);
        }

        return result;
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterGetValueAsBlob(handle, parameter,
                                                                  buffer, bufferLength);
    }
}


seos_err_t
seos_configuration_parameterSetValue(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        static dataport_ptr_t bufferSend;

        memcpy((void*)cfg_dataport_buf, buffer, bufferLength);
        bufferSend = dataport_wrap_ptr((void*)cfg_dataport_buf);

        return server_seos_configuration_parameterSetValue(handle, enumerator,
                                                           parameterType, bufferSend, bufferLength);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterSetValue(handle, enumerator,
                                                            parameterType, buffer, bufferLength);
    }
}


seos_err_t
seos_configuration_parameterSetValueAsU32(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint32_t value)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_parameterSetValueAsU32(handle, enumerator,
                                                                value);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterSetValueAsU32(handle, enumerator,
                                                                 value);
    }
}


seos_err_t
seos_configuration_parameterSetValueAsU64(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint64_t value)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        return server_seos_configuration_parameterSetValueAsU64(handle, enumerator,
                                                                value);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterSetValueAsU64(handle, enumerator,
                                                                 value);
    }
}


seos_err_t
seos_configuration_parameterSetValueAsString(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    char const* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        static dataport_ptr_t bufferSend;

        memcpy((void*)cfg_dataport_buf, buffer, bufferLength);
        bufferSend = dataport_wrap_ptr((void*)cfg_dataport_buf);

        return server_seos_configuration_parameterSetValueAsString(handle, enumerator,
                                                                   parameterType, bufferSend, bufferLength);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterSetValueAsString(handle, enumerator,
                                                                    parameterType, buffer, bufferLength);
    }
}


seos_err_t
seos_configuration_parameterSetValueAsBlob(
    SeosConfigHandle handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {
#if defined(SEOS_CONFIG_CAMKES_CLIENT)
        static dataport_ptr_t bufferSend;

        memcpy((void*)cfg_dataport_buf, buffer, bufferLength);
        bufferSend = dataport_wrap_ptr((void*)cfg_dataport_buf);

        return server_seos_configuration_parameterSetValueAsBlob(handle, enumerator,
                                                                 parameterType, bufferSend, bufferLength);
#else
        return SEOS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return library_seos_configuration_parameterSetValueAsBlob(handle, enumerator,
                                                                  parameterType, buffer, bufferLength);
    }
}


seos_err_t
seos_configuration_parameterGetValueFromDomainName(
    SeosConfigHandle                   handle,
    const char*                        domain_name,
    const char*                        param_name,
    const SeosConfigLib_ParameterType  param_type,
    void*                              buffer,
    size_t*                            len)
{
    if (SEOS_CONFIG_HANDLE_KIND_RPC == seos_configuration_handle_getHandleKind(
            handle))
    {

#if defined(SEOS_CONFIG_CAMKES_CLIENT)

        static dataport_ptr_t bufferReceive = dataport_wrap_ptr((
                                                                    void*)cfg_dataport_buf);
        int result = server_seos_configuration_parameterGetValueFromDomainName(
                         handle,
                         domain_name,
                         param_name,
                         param_type,
                         bufferReceive,
                         len);

        if (result == SEOS_SUCCESS)
        {
            const void* buf = dataport_unwrap_ptr(bufferReceive);
            memcpy(buffer, buf, bufferLength);
        }

        return result;

#else
        return SEOS_ERROR_INVALID_PARAMETER;

#endif

    }
    else
    {
        return library_seos_configuration_parameterGetValueFromDomainName(
                   handle,
                   domain_name,
                   param_name,
                   param_type,
                   buffer,
                   len);
    }
}
