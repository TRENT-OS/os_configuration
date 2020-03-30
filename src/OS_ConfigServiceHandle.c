/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#include "OS_ConfigServiceHandle.h"


void
OS_ConfigServiceHandle_initRemoteHandle(
    unsigned int instanceId,
    OS_ConfigServiceHandle_t* handle)
{
    handle->handleKind = OS_CONFIG_HANDLE_KIND_RPC;
    handle->handle.rpc.id = instanceId;
}


void
OS_ConfigServiceHandle_initLocalHandle(
    void* instance,
    OS_ConfigServiceHandle_t* handle)
{
    handle->handleKind = OS_CONFIG_HANDLE_KIND_LOCAL;
    handle->handle.local.instance = instance;
}


OS_ConfigServiceHandle_HandleKind_t
OS_ConfigServiceHandle_getHandleKind(
    OS_ConfigServiceHandle_t handle)
{
    return handle.handleKind;
}


unsigned int
OS_ConfigServiceHandle_getRemoteInstance(
    OS_ConfigServiceHandle_t handle)
{
    return handle.handle.rpc.id;
}


void*
OS_ConfigServiceHandle_getLocalInstance(
    OS_ConfigServiceHandle_t handle)
{
    return handle.handle.local.instance;
}
