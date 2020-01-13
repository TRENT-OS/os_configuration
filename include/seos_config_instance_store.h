/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include <stdlib.h>

#include "SeosConfigLib.h"

// The maximum number of configuration libraries the store can maintain.
typedef enum
{
    SEOS_CONFIG_INSTANCE_STORE_MAX_INSTANCES = 4
} SeosConfigInstanceStore_Consts;

// The name of a paramter.
typedef struct
{
    SeosConfigLib instances[SEOS_CONFIG_INSTANCE_STORE_MAX_INSTANCES];
}
SeosConfigInstanceStore;


#include "seos_config_data_types.h"

inline
static SeosConfigLib*
seos_configuration_instance_store_getInstance(SeosConfigInstanceStore* self,
                                              unsigned int instanceIndex)
{
    if (instanceIndex < SEOS_CONFIG_INSTANCE_STORE_MAX_INSTANCES)
    {
        return self->instances + instanceIndex;
    }
    else
    {
        return NULL;
    }
}
