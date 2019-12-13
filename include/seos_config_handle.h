/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once


// Distinguishes between server and local handles.
typedef enum
{
    SEOS_CONFIG_HANDLE_KIND_RPC,
    SEOS_CONFIG_HANDLE_KIND_LOCAL,
} SeosConfigLib_HandleKind;


// A handle identifies a configuration library instance locally or remotely (= on the server).
typedef struct
{
    SeosConfigLib_HandleKind handleKind;
    union
    {
        struct
        {
            unsigned int id;
        } rpc;
        
        struct
        {
            void* instance;
        } local;
    } handle;
}
SeosConfigHandle;


inline
static void
seos_configuration_handle_initRemoteHandle(
    unsigned int instanceId,
    SeosConfigHandle* handle)
{
    handle->handleKind = SEOS_CONFIG_HANDLE_KIND_RPC;
    handle->handle.rpc.id = instanceId;
}


inline
static void
seos_configuration_handle_initLocalHandle(
    void* instance,
    SeosConfigHandle* handle)
{
    handle->handleKind = SEOS_CONFIG_HANDLE_KIND_LOCAL;
    handle->handle.local.instance = instance;
}


inline
static SeosConfigLib_HandleKind
seos_configuration_handle_getHandleKind(
    SeosConfigHandle handle)
{
    return handle.handleKind;
}


inline
static unsigned int
seos_configuration_handle_getRemoteInstance(
    SeosConfigHandle handle)
{
    return handle.handle.rpc.id;
}


inline
static void*
seos_configuration_handle_getLocalInstance(
    SeosConfigHandle handle)
{
    return handle.handle.local.instance;
}
