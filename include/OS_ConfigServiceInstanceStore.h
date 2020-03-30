/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

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


OS_ConfigServiceLib_t*
OS_ConfigServiceInstanceStore_getInstance(
    OS_ConfigServiceInstanceStore_t* self,
    unsigned int instanceIndex);
