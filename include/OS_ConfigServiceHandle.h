/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @addtogroup Handle
 * @{
 *
 * @file OS_ConfigServiceHandle.h
 *
 * @brief Collection of definitions and functions related to the handles used to
 * interact with specific ConfigService library instances.
 *
 */

#pragma once

#include <stdint.h>

/* Exported types/defines/enums ----------------------------------------------*/

// Distinguishes between server and local handles.
typedef enum
{
    OS_CONFIG_HANDLE_KIND_RPC,
    OS_CONFIG_HANDLE_KIND_LOCAL,
} OS_ConfigServiceHandle_HandleKind_t;

// A handle identifies a configuration library instance locally or remotely (= on the server).
typedef struct
{
    OS_ConfigServiceHandle_HandleKind_t handleKind;
    union
    {
        struct
        {
            void*           dataport;
            size_t          dataportSize;
            intptr_t        clientCtx;
        } rpc;
        struct
        {
            void* instance;
        } local;
    } context;
}
OS_ConfigServiceHandle_t;

/* Exported functions --------------------------------------------------------*/
void
OS_ConfigServiceHandle_initRemoteHandle(
    intptr_t clientCtx,
    void* dataport,
    size_t dataportSize,
    OS_ConfigServiceHandle_t* handle);

void
OS_ConfigServiceHandle_initLocalHandle(
    void* instance,
    OS_ConfigServiceHandle_t* handle);

OS_ConfigServiceHandle_HandleKind_t
OS_ConfigServiceHandle_getHandleKind(
    OS_ConfigServiceHandle_t handle);

void*
OS_ConfigServiceHandle_getLocalInstance(
    OS_ConfigServiceHandle_t handle);

intptr_t
OS_ConfigServiceHandle_getClientContext(
    OS_ConfigServiceHandle_t* handle
);

/** @} */
