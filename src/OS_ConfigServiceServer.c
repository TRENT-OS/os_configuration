/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#if defined (OS_CONFIG_SERVICE_CAMKES_SERVER)

#include "OS_ConfigServiceServer.h"
#include "OS_ConfigServiceLibrary.h"


typedef struct
{
    OS_ConfigServiceInstanceStore_t instanceStore;
}
OS_ConfigServiceServer_t;


static OS_ConfigServiceServer_t server;


static
seos_err_t
OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(
    OS_ConfigServiceHandle_t remoteHandle,
    OS_ConfigServiceHandle_t* localHandle)
{
    if (OS_CONFIG_HANDLE_KIND_RPC == OS_ConfigServiceHandle_getHandleKind(
            remoteHandle))
    {
        OS_ConfigServiceLib_t* instance = OS_ConfigServiceInstanceStore_getInstance(
                                              &server.instanceStore,
                                              OS_ConfigServiceHandle_getRemoteInstance(remoteHandle));

        if (instance != NULL)
        {
            OS_ConfigServiceHandle_initLocalHandle(
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


OS_ConfigServiceInstanceStore_t*
OS_ConfigServiceServer_getInstances(void)
{
    return &server.instanceStore;
}


seos_err_t
OS_ConfigServiceServer_createHandle(
    OS_ConfigServiceHandle_HandleKind_t handleKind,
    unsigned int id,
    OS_ConfigServiceHandle_t* handle)
{
    seos_err_t result;

    if (handleKind == OS_CONFIG_HANDLE_KIND_RPC)
    {
        OS_ConfigServiceLib_t* instance = OS_ConfigServiceInstanceStore_getInstance(
                                              &server.instanceStore,
                                              id);

        if (instance != NULL)
        {
            OS_ConfigServiceHandle_initRemoteHandle(
                id,
                handle);

            result = SEOS_SUCCESS;
        }
        else
        {
            result = SEOS_ERROR_INVALID_PARAMETER;
        }
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_domainEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_domainEnumeratorInit(
                     localHandle,
                     enumerator);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}

seos_err_t
OS_ConfigServiceServer_domainEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        OS_ConfigServiceLibrary_domainEnumeratorClose(localHandle, enumerator);

        result = SEOS_SUCCESS;
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_domainEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        OS_ConfigServiceLibrary_domainEnumeratorReset(localHandle, enumerator);

        result = SEOS_SUCCESS;
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_domainEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_domainEnumeratorIncrement(
                     localHandle,
                     enumerator);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_domainEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_domainEnumeratorGetElement(
                     localHandle,
                     enumerator,
                     domain);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_parameterEnumeratorInit(
                     localHandle,
                     domainEnumerator,
                     enumerator);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        OS_ConfigServiceLibrary_parameterEnumeratorClose(localHandle, enumerator);

        result = SEOS_SUCCESS;
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_parameterEnumeratorReset(
                     localHandle,
                     enumerator);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_parameterEnumeratorIncrement(
                     localHandle,
                     enumerator);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_parameterEnumeratorGetElement(localHandle,
                 enumerator, parameter);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


void
OS_ConfigServiceServer_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName)
{
    OS_ConfigServiceLibrary_domainGetName(domain, domainName);
}


seos_err_t
OS_ConfigServiceServer_domainCreateParameterEnumerator(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_domainCreateParameterEnumerator(
                     localHandle,
                     domain,
                     parameterName,
                     parameterEnumerator);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_domainGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_domainGetElement(
                     localHandle,
                     domain,
                     parameterName,
                     parameter);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


void
OS_ConfigServiceServer_parameterGetName(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterName_t* parameterName)
{
    OS_ConfigServiceLibrary_parameterGetName(parameter, parameterName);
}


void
OS_ConfigServiceServer_parameterGetType(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t* parameterType)
{
    OS_ConfigServiceLibrary_parameterGetType(parameter, parameterType);
}


size_t
OS_ConfigServiceServer_parameterGetSize(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter)
{
    return OS_ConfigServiceLibrary_parameterGetSize(parameter);
}


seos_err_t
OS_ConfigServiceServer_parameterGetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    dataport_ptr_t buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        result = OS_ConfigServiceLibrary_parameterGetValue(
                     localHandle,
                     parameter,
                     tmpBuf,
                     bufferLength,
                     bytesCopied);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterGetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_parameterGetValueAsU32(
                     localHandle,
                     parameter,
                     value);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterGetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_parameterGetValueAsU64(
                     localHandle,
                     parameter,
                     value);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterGetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        result = OS_ConfigServiceLibrary_parameterGetValueAsString(
                     localHandle,
                     parameter,
                     tmpBuf, bufferLength);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterGetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        result = OS_ConfigServiceLibrary_parameterGetValueAsBlob(
                     localHandle,
                     parameter,
                     tmpBuf,
                     bufferLength);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterSetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        result = OS_ConfigServiceLibrary_parameterSetValue(
                     localHandle,
                     enumerator,
                     parameterType,
                     tmpBuf,
                     bufferLength);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterSetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_parameterSetValueAsU32(
                     localHandle,
                     enumerator,
                     value);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterSetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        result = OS_ConfigServiceLibrary_parameterSetValueAsU64(
                     localHandle,
                     enumerator,
                     value);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterSetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        result = OS_ConfigServiceLibrary_parameterSetValueAsString(
                     localHandle,
                     enumerator,
                     parameterType,
                     tmpBuf,
                     bufferLength);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterSetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength)
{
    OS_ConfigServiceHandle_t localHandle;
    seos_err_t result;

    if (SEOS_SUCCESS ==
        OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(handle,
                &localHandle))
    {
        void* tmpBuf = dataport_unwrap_ptr(buffer);
        result = OS_ConfigServiceLibrary_parameterSetValueAsBlob(
                     localHandle,
                     enumerator,
                     parameterType,
                     tmpBuf,
                     bufferLength);
    }
    else
    {
        result = SEOS_ERROR_INVALID_PARAMETER;
    }

    return result;
}


seos_err_t
OS_ConfigServiceServer_parameterGetValueFromDomainName(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength,
    size_t* bytesCopied)
{

    OS_ConfigServiceHandle_t localHandle;
    seos_err_t ret = OS_ConfigServiceServer_transformRemoteHandleToLocalHandle(
                         handle,
                         &localHandle);
    if (SEOS_SUCCESS != ret)
    {
        return SEOS_ERROR_INVALID_PARAMETER;
    }

    void* tmpBuf = dataport_unwrap_ptr(buffer);
    return OS_ConfigServiceLibrary_parameterGetValueFromDomainName(
               localHandle,
               domainName,
               parameterName,
               parameterType,
               tmpBuf,
               bufferLength,
               bytesCopied);
}

#endif
