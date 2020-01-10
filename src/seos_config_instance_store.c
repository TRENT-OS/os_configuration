/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#include "seos_config_instance_store.h"

SeosConfigLib*
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
