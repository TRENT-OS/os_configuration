/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "stdbool.h"

enum
{
    SEOS_CONFIG_ACCESS_RIGHTS_VECTOR_SIZE = 8,
    SEOS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY = SEOS_CONFIG_ACCESS_RIGHTS_VECTOR_SIZE * 8
};

typedef struct
{
    unsigned char rights[SEOS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY];
} SeosConfigAccessRights;

void SeosConfigAccessRights_ClearAll(SeosConfigAccessRights* accessRights);
void SeosConfigAccessRights_SetAll(SeosConfigAccessRights* accessRights);

void SeosConfigAccessRights_Set(SeosConfigAccessRights* accessRights,
                                unsigned int entryIndex);
void SeosConfigAccessRights_Clear(SeosConfigAccessRights* accessRights,
                                  unsigned int entryIndex);

bool SeosConfigAccessRights_IsSet(SeosConfigAccessRights const* accessRights,
                                  unsigned int entryIndex);
