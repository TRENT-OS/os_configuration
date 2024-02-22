/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/* Includes ------------------------------------------------------------------*/
#if defined (OS_CONFIG_SERVICE_CAMKES_SERVER)

#include "OS_ConfigServiceServer.h"
#include "OS_ConfigServiceLibrary.h"

#include "lib_debug/Debug.h"

#include <camkes.h>

/* Local types ---------------------------------------------------------------*/
static OS_ConfigServiceLib_t serverInstance = { 0 };

// Not generated yet by camkes
seL4_Word OS_ConfigServiceServer_get_sender_id(void);

// How many parallel remote instances can we handle? This number can be easily
// increased, for now it is aligned with the limits of other server systems
// which are provided by TRENTOS.
#define MAX_REMOTE_HANDLES 8
static OS_ConfigServiceHandle_t localHandleCopies[MAX_REMOTE_HANDLES];

static OS_ConfigServiceHandle_t*
getLocalCopyOfHandle(void)
{
    seL4_Word id = OS_ConfigServiceServer_get_sender_id();
    Debug_ASSERT(id >= 0 && id < MAX_REMOTE_HANDLES);
    return &localHandleCopies[id];
}

/* Exported functions --------------------------------------------------------*/
OS_ConfigServiceLib_t*
OS_ConfigServiceServer_getInstance(void)
{
    return &serverInstance;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_createHandle(
    intptr_t clientCtx,
    dataport_ptr_t wrappedDp,
    size_t wrappedDpSize,
    OS_ConfigServiceHandle_t* handle)
{
    void* ptr;

    // Make sure that we actually get a dataport here and not some arbitrary
    // memory pointer..
    if ((ptr = dataport_unwrap_ptr(wrappedDp)) == NULL)
    {
        return OS_ERROR_INVALID_PARAMETER;
    }

    // Formerly, the server would init a context and pass it back to the client,
    // which would then send it back so the server knew the dataport etc. for
    // this specific client. Now, we keep the init and sending it back to thew
    // client, but we no longer accept the client sending its context again and
    // again, so we keep a copy of it in case it is needed (when using a dataport)
    OS_ConfigServiceHandle_initRemoteHandle(
        clientCtx,
        ptr,
        wrappedDpSize,
        handle);

    // Every client has its own copy which we keep and index based on their
    // CAmkES badge ID.
    OS_ConfigServiceHandle_initRemoteHandle(
        clientCtx,
        ptr,
        wrappedDpSize,
        getLocalCopyOfHandle());

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_domainEnumeratorInit(
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_domainEnumeratorInit(
               localHandle,
               enumerator);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_domainEnumeratorClose(
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_domainEnumeratorClose(localHandle,
                                                         enumerator);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_domainEnumeratorReset(
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_domainEnumeratorReset(localHandle,
                                                         enumerator);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_domainEnumeratorIncrement(
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_domainEnumeratorIncrement(
               localHandle,
               enumerator);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_domainEnumeratorGetElement(
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_domainEnumeratorGetElement(
               localHandle,
               enumerator,
               domain);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorInit(
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_parameterEnumeratorInit(
               localHandle,
               domainEnumerator,
               enumerator);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorClose(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    OS_ConfigServiceLibrary_parameterEnumeratorClose(
        localHandle,
        enumerator);

    return OS_SUCCESS;
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorReset(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_parameterEnumeratorReset(
               localHandle,
               enumerator);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorIncrement(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_parameterEnumeratorIncrement(
               localHandle,
               enumerator);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorGetElement(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_parameterEnumeratorGetElement(localHandle,
            enumerator, parameter);
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceServer_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName)
{
    OS_ConfigServiceLibrary_domainGetName(domain, domainName);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_domainCreateParameterEnumerator(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_domainCreateParameterEnumerator(
               localHandle,
               domain,
               parameterName,
               parameterEnumerator);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_domainGetElement(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_domainGetElement(
               localHandle,
               domain,
               parameterName,
               parameter);
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceServer_parameterGetName(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterName_t* parameterName)
{
    OS_ConfigServiceLibrary_parameterGetName(parameter, parameterName);
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceServer_parameterGetType(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t* parameterType)
{
    OS_ConfigServiceLibrary_parameterGetType(parameter, parameterType);
}

//------------------------------------------------------------------------------
size_t
OS_ConfigServiceServer_parameterGetSize(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter)
{
    return OS_ConfigServiceLibrary_parameterGetSize(parameter);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterGetValue(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    size_t bufferSize,
    size_t* bytesCopied)
{
    OS_ConfigServiceHandle_t* copyOfRemoteHandle = getLocalCopyOfHandle();
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    if (bufferSize > copyOfRemoteHandle->context.rpc.dataportSize)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    return OS_ConfigServiceLibrary_parameterGetValue(
               localHandle,
               parameter,
               copyOfRemoteHandle->context.rpc.dataport,
               bufferSize,
               bytesCopied);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterGetValueAsU32(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_parameterGetValueAsU32(
               localHandle,
               parameter,
               value);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterGetValueAsU64(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_parameterGetValueAsU64(
               localHandle,
               parameter,
               value);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterGetValueAsString(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    size_t bufferSize)
{
    OS_ConfigServiceHandle_t* copyOfRemoteHandle = getLocalCopyOfHandle();
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    if (bufferSize > copyOfRemoteHandle->context.rpc.dataportSize)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    return OS_ConfigServiceLibrary_parameterGetValueAsString(
               localHandle,
               parameter,
               copyOfRemoteHandle->context.rpc.dataport,
               bufferSize);

}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterGetValueAsBlob(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    size_t bufferSize)
{
    OS_ConfigServiceHandle_t* copyOfRemoteHandle = getLocalCopyOfHandle();
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    if (bufferSize > copyOfRemoteHandle->context.rpc.dataportSize)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    return OS_ConfigServiceLibrary_parameterGetValueAsBlob(
               localHandle,
               parameter,
               copyOfRemoteHandle->context.rpc.dataport,
               bufferSize);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterSetValue(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    size_t bufferSize)
{
    OS_ConfigServiceHandle_t* copyOfRemoteHandle = getLocalCopyOfHandle();
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    if (bufferSize > copyOfRemoteHandle->context.rpc.dataportSize)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    return OS_ConfigServiceLibrary_parameterSetValue(
               localHandle,
               enumerator,
               parameterType,
               copyOfRemoteHandle->context.rpc.dataport,
               bufferSize);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterSetValueAsU32(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_parameterSetValueAsU32(
               localHandle,
               enumerator,
               value);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterSetValueAsU64(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value)
{
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    return OS_ConfigServiceLibrary_parameterSetValueAsU64(
               localHandle,
               enumerator,
               value);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterSetValueAsString(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    size_t bufferSize)
{
    OS_ConfigServiceHandle_t* copyOfRemoteHandle = getLocalCopyOfHandle();
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    if (bufferSize > copyOfRemoteHandle->context.rpc.dataportSize)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    return OS_ConfigServiceLibrary_parameterSetValueAsString(
               localHandle,
               enumerator,
               parameterType,
               copyOfRemoteHandle->context.rpc.dataport,
               bufferSize);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterSetValueAsBlob(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    size_t bufferSize)
{
    OS_ConfigServiceHandle_t* copyOfRemoteHandle = getLocalCopyOfHandle();
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    if (bufferSize > copyOfRemoteHandle->context.rpc.dataportSize)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    return OS_ConfigServiceLibrary_parameterSetValueAsBlob(
               localHandle,
               enumerator,
               parameterType,
               copyOfRemoteHandle->context.rpc.dataport,
               bufferSize);
}

//------------------------------------------------------------------------------
OS_Error_t
OS_ConfigServiceServer_parameterGetValueFromDomainName(
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    size_t bufferSize,
    size_t* bytesCopied)
{
    OS_ConfigServiceHandle_t* copyOfRemoteHandle = getLocalCopyOfHandle();
    OS_ConfigServiceHandle_t localHandle;

    OS_ConfigServiceHandle_initLocalHandle(
        &serverInstance,
        &localHandle);

    if (bufferSize > copyOfRemoteHandle->context.rpc.dataportSize)
    {
        return OS_ERROR_BUFFER_TOO_SMALL;
    }

    return OS_ConfigServiceLibrary_parameterGetValueFromDomainName(
               localHandle,
               domainName,
               parameterName,
               parameterType,
               copyOfRemoteHandle->context.rpc.dataport,
               bufferSize,
               bytesCopied);
}

#endif
