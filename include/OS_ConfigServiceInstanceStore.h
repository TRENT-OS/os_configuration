/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup InstanceStore
 * @{
 *
 * @file OS_ConfigServiceInstanceStore.h
 *
 * @brief Helper module to manage configuration library instances.
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "OS_ConfigServiceLib.h"

// The maximum number of configuration libraries the store can maintain.
typedef enum
{
    OS_CONFIG_INSTANCE_STORE_MAX_INSTANCES = 4
} OS_ConfigServiceInstanceStore_Consts_t;

// The name of a paramter.
typedef struct
{
    OS_ConfigServiceLib_t instances[OS_CONFIG_INSTANCE_STORE_MAX_INSTANCES];
}
OS_ConfigServiceInstanceStore_t;

/* Exported functions --------------------------------------------------------*/
OS_ConfigServiceLib_t*
OS_ConfigServiceInstanceStore_getInstance(
    OS_ConfigServiceInstanceStore_t* self,
    unsigned int instanceIndex);

/** @} */