/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include <unistd.h>

#include "OS_ConfigServiceHandle.h"

/* Exported functions --------------------------------------------------------*/
void
OS_ConfigServiceHandle_initRemoteHandle(
    unsigned int instanceId,
    intptr_t clientCtx,
    void* dataport,
    size_t dataportSize,
    OS_ConfigServiceHandle_t* handle)
{
    handle->handleKind = OS_CONFIG_HANDLE_KIND_RPC;
    handle->context.rpc.id = instanceId;
    handle->context.rpc.dataport = dataport;
    handle->context.rpc.dataportSize = dataportSize;
    handle->context.rpc.clientCtx = clientCtx;
}

//------------------------------------------------------------------------------
void
OS_ConfigServiceHandle_initLocalHandle(
    void* instance,
    OS_ConfigServiceHandle_t* handle)
{
    handle->handleKind = OS_CONFIG_HANDLE_KIND_LOCAL;
    handle->context.local.instance = instance;
}

//------------------------------------------------------------------------------
OS_ConfigServiceHandle_HandleKind_t
OS_ConfigServiceHandle_getHandleKind(
    OS_ConfigServiceHandle_t handle)
{
    return handle.handleKind;
}

//------------------------------------------------------------------------------
intptr_t
OS_ConfigServiceHandle_getRemoteInstance(
    OS_ConfigServiceHandle_t handle)
{
    return handle.context.rpc.id;
}

//------------------------------------------------------------------------------
void*
OS_ConfigServiceHandle_getLocalInstance(
    OS_ConfigServiceHandle_t handle)
{
    return handle.context.local.instance;
}

//------------------------------------------------------------------------------
intptr_t
OS_ConfigServiceHandle_getClientContext(
    OS_ConfigServiceHandle_t* handle
)
{
    if (handle->handleKind != OS_CONFIG_HANDLE_KIND_RPC)
    {
        return (intptr_t) NULL;
    }
    return handle->context.rpc.clientCtx;
}
