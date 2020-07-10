/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include <unistd.h>

#include "string.h"

#include "OS_ConfigService.h"
#include "OS_ConfigServiceLibrary.h"

#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
#include "camkes.h"
#endif

#if defined(OS_CONFIG_SERVICE_CAMKES_SERVER)
#include "OS_ConfigServiceServer.h"
#endif

/* Local types ---------------------------------------------------------------*/
typedef struct
{
    OS_ConfigServiceInstanceStore_t instanceStore;
}
OS_ConfigService_Client_t;


static OS_ConfigService_Client_t client;

/* Exported functions --------------------------------------------------------*/
OS_ConfigServiceInstanceStore_t*
OS_ConfigService_getInstances(void)
{
#if defined(OS_CONFIG_SERVICE_CAMKES_SERVER)
    return OS_ConfigServiceServer_getInstances();
#endif
    return &client.instanceStore;
}

OS_Error_t
OS_ConfigService_createHandleLocal(
    unsigned int id,
    OS_ConfigServiceHandle_t* handle)
{
    OS_ConfigServiceLib_t* instance = OS_ConfigServiceInstanceStore_getInstance(
                                          &client.instanceStore,
                                          id);

    if (instance != NULL)
    {
        OS_ConfigServiceHandle_initLocalHandle(
            (void*)instance,
            handle);

        return OS_SUCCESS;
    }
    else
    {
        return OS_ERROR_INVALID_PARAMETER;
    }
}

OS_Error_t
OS_ConfigService_createHandleRemote(
    unsigned int id,
    OS_ConfigService_ClientCtx_t* clientCtx,
    OS_ConfigServiceHandle_t* handle)
{
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
    OS_Error_t err = OS_ConfigServiceServer_createHandle(
                         id,
                         (intptr_t) clientCtx,
                         dataport_wrap_ptr(*clientCtx->dataport.io),
                         clientCtx->dataport.size,
                         handle);
    return err;
#else
    return OS_ERROR_INVALID_PARAMETER;
#endif
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_domainEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_domainEnumeratorInit(handle, enumerator);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_domainEnumeratorInit(handle, enumerator);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_domainEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_domainEnumeratorClose(handle, enumerator);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_domainEnumeratorClose(handle, enumerator);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_domainEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_domainEnumeratorReset(handle, enumerator);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_domainEnumeratorReset(handle, enumerator);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_domainEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_domainEnumeratorIncrement(handle, enumerator);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_domainEnumeratorIncrement(handle, enumerator);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_domainEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_domainEnumeratorGetElement(
                   handle,
                   enumerator,
                   domain);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_domainEnumeratorGetElement(
                   handle,
                   enumerator,
                   domain);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_parameterEnumeratorInit(
                   handle,
                   domainEnumerator,
                   enumerator);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterEnumeratorInit(
                   handle,
                   domainEnumerator,
                   enumerator);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_parameterEnumeratorClose(handle, enumerator);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterEnumeratorClose(handle, enumerator);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_parameterEnumeratorReset(handle, enumerator);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterEnumeratorReset(handle, enumerator);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_parameterEnumeratorIncrement(
                   handle,
                   enumerator);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterEnumeratorIncrement(
                   handle,
                   enumerator);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_parameterEnumeratorGetElement(
                   handle,
                   enumerator,
                   parameter);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterEnumeratorGetElement(
                   handle,
                   enumerator,
                   parameter);
    }
}

//------------------------------------------------------------------------------
void
OS_ConfigService_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName)
{
    OS_ConfigServiceLibrary_domainGetName(domain, domainName);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_domainCreateParameterEnumerator(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_domainCreateParameterEnumerator(
                   handle,
                   domain,
                   parameterName,
                   parameterEnumerator);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_domainCreateParameterEnumerator(
                   handle,
                   domain,
                   parameterName,
                   parameterEnumerator);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_domainGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_domainGetElement(
                   handle,
                   domain,
                   parameterName,
                   parameter);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_domainGetElement(
                   handle,
                   domain,
                   parameterName,
                   parameter);
    }
}

//------------------------------------------------------------------------------
void
OS_ConfigService_parameterGetName(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterName_t* parameterName)
{
    OS_ConfigServiceLibrary_parameterGetName(parameter, parameterName);
}

//------------------------------------------------------------------------------
void
OS_ConfigService_parameterGetType(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t* parameterType)
{
    OS_ConfigServiceLibrary_parameterGetType(parameter, parameterType);
}

//------------------------------------------------------------------------------
size_t
OS_ConfigService_parameterGetSize(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter)
{
    return OS_ConfigServiceLibrary_parameterGetSize(parameter);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterGetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        int result =
            OS_ConfigServiceServer_parameterGetValue(
                handle,
                parameter,
                bufferLength,
                bytesCopied);

        if (result == OS_SUCCESS)
        {
            if (*bytesCopied > bufferLength)
            {
                return OS_ERROR_BUFFER_TOO_SMALL;
            }
            OS_ConfigService_ClientCtx_t* clientCtx =
                (OS_ConfigService_ClientCtx_t*)
                OS_ConfigServiceHandle_getClientContext(&handle);
            memcpy(buffer, *clientCtx->dataport.io, *bytesCopied);
        }

        return result;
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterGetValue(
                   handle,
                   parameter,
                   buffer,
                   bufferLength,
                   bytesCopied);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterGetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_parameterGetValueAsU32(
                   handle,
                   parameter,
                   value);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterGetValueAsU32(
                   handle,
                   parameter,
                   value);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterGetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_parameterGetValueAsU64(
                   handle,
                   parameter,
                   value);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterGetValueAsU64(
                   handle,
                   parameter,
                   value);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterGetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    char* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        int result =
            OS_ConfigServiceServer_parameterGetValueAsString(
                handle,
                parameter,
                bufferLength);

        if (result == OS_SUCCESS)
        {
            OS_ConfigService_ClientCtx_t* clientCtx =
                (OS_ConfigService_ClientCtx_t*)
                OS_ConfigServiceHandle_getClientContext(&handle);
            memcpy(buffer, *clientCtx->dataport.io, bufferLength);
        }

        return result;
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterGetValueAsString(
                   handle,
                   parameter,
                   buffer,
                   bufferLength);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterGetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        int result =
            OS_ConfigServiceServer_parameterGetValueAsBlob(
                handle,
                parameter,
                bufferLength);

        if (result == OS_SUCCESS)
        {
            OS_ConfigService_ClientCtx_t* clientCtx =
                (OS_ConfigService_ClientCtx_t*)
                OS_ConfigServiceHandle_getClientContext(&handle);
            memcpy(buffer, *clientCtx->dataport.io, bufferLength);
        }

        return result;
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterGetValueAsBlob(
                   handle,
                   parameter,
                   buffer,
                   bufferLength);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterSetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        OS_ConfigService_ClientCtx_t* clientCtx =
            (OS_ConfigService_ClientCtx_t*)
            OS_ConfigServiceHandle_getClientContext(&handle);

        if (bufferLength > clientCtx->dataport.size)
        {
            return OS_ERROR_BUFFER_TOO_SMALL;
        }
        memcpy(*clientCtx->dataport.io, buffer, bufferLength);

        return OS_ConfigServiceServer_parameterSetValue(
                   handle,
                   enumerator,
                   parameterType,
                   bufferLength);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterSetValue(
                   handle,
                   enumerator,
                   parameterType,
                   buffer,
                   bufferLength);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterSetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_parameterSetValueAsU32(
                   handle,
                   enumerator,
                   value);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterSetValueAsU32(
                   handle,
                   enumerator,
                   value);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterSetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        return OS_ConfigServiceServer_parameterSetValueAsU64(
                   handle,
                   enumerator,
                   value);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterSetValueAsU64(
                   handle,
                   enumerator,
                   value);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterSetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    char const* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        OS_ConfigService_ClientCtx_t* clientCtx =
            (OS_ConfigService_ClientCtx_t*)
            OS_ConfigServiceHandle_getClientContext(&handle);

        if (bufferLength > clientCtx->dataport.size)
        {
            return OS_ERROR_BUFFER_TOO_SMALL;
        }
        memcpy(*clientCtx->dataport.io, buffer, bufferLength);

        return OS_ConfigServiceServer_parameterSetValueAsString(
                   handle,
                   enumerator,
                   parameterType,
                   bufferLength);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterSetValueAsString(
                   handle,
                   enumerator,
                   parameterType,
                   buffer,
                   bufferLength);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterSetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {
#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)
        OS_ConfigService_ClientCtx_t* clientCtx =
            (OS_ConfigService_ClientCtx_t*)
            OS_ConfigServiceHandle_getClientContext(&handle);

        if (bufferLength > clientCtx->dataport.size)
        {
            return OS_ERROR_BUFFER_TOO_SMALL;
        }
        memcpy(*clientCtx->dataport.io, buffer, bufferLength);

        return OS_ConfigServiceServer_parameterSetValueAsBlob(
                   handle,
                   enumerator,
                   parameterType,
                   bufferLength);
#else
        return OS_ERROR_INVALID_PARAMETER;
#endif
    }
    else
    {
        return OS_ConfigServiceLibrary_parameterSetValueAsBlob(
                   handle,
                   enumerator,
                   parameterType,
                   buffer,
                   bufferLength);
    }
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigService_parameterGetValueFromDomainName(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            handle))
    {

#if defined(OS_CONFIG_SERVICE_CAMKES_CLIENT)

        int result = OS_ConfigServiceServer_parameterGetValueFromDomainName(
                         handle,
                         domainName,
                         parameterName,
                         parameterType,
                         bufferLength,
                         bytesCopied);

        if (result == OS_SUCCESS)
        {
            OS_ConfigService_ClientCtx_t* clientCtx =
                (OS_ConfigService_ClientCtx_t*)
                OS_ConfigServiceHandle_getClientContext(&handle);

            if (*bytesCopied > bufferLength)
            {
                return OS_ERROR_BUFFER_TOO_SMALL;
            }
            memcpy(buffer, *clientCtx->dataport.io, *bytesCopied);
        }

        return result;

#else
        return OS_ERROR_INVALID_PARAMETER;

#endif

    }
    else
    {
        return OS_ConfigServiceLibrary_parameterGetValueFromDomainName(
                   handle,
                   domainName,
                   parameterName,
                   parameterType,
                   buffer,
                   bufferLength,
                   bytesCopied);
    }
}
