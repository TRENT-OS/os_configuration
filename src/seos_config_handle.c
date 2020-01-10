/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#include "seos_config_handle.h"


void
seos_configuration_handle_initRemoteHandle(
    unsigned int instanceId,
    SeosConfigHandle* handle)
{
    handle->handleKind = SEOS_CONFIG_HANDLE_KIND_RPC;
    handle->handle.rpc.id = instanceId;
}


void
seos_configuration_handle_initLocalHandle(
    void* instance,
    SeosConfigHandle* handle)
{
    handle->handleKind = SEOS_CONFIG_HANDLE_KIND_LOCAL;
    handle->handle.local.instance = instance;
}


SeosConfigLib_HandleKind
seos_configuration_handle_getHandleKind(
    SeosConfigHandle handle)
{
    return handle.handleKind;
}


unsigned int
seos_configuration_handle_getRemoteInstance(
    SeosConfigHandle handle)
{
    return handle.handle.rpc.id;
}


void*
seos_configuration_handle_getLocalInstance(
    SeosConfigHandle handle)
{
    return handle.handle.local.instance;
}
