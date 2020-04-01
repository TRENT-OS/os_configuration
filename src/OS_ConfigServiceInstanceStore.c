/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include "OS_ConfigServiceInstanceStore.h"

/* Exported functions --------------------------------------------------------*/
OS_ConfigServiceLib_t*
OS_ConfigServiceInstanceStore_getInstance(
    OS_ConfigServiceInstanceStore_t* self,
    unsigned int instanceIndex)
{
    if (instanceIndex < OS_CONFIG_INSTANCE_STORE_MAX_INSTANCES)
    {
        return self->instances + instanceIndex;
    }
    else
    {
        return NULL;
    }
}
