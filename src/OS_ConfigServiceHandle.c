/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/* Includes ------------------------------------------------------------------*/
#include <unistd.h>

#include "OS_ConfigServiceHandle.h"

/* Exported functions --------------------------------------------------------*/
void
OS_ConfigServiceHandle_initRemoteHandle(
    intptr_t clientCtx,
    void* dataport,
    size_t dataportSize,
    OS_ConfigServiceHandle_t* handle)
{
    handle->handleKind = OS_CONFIG_HANDLE_KIND_RPC;
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
