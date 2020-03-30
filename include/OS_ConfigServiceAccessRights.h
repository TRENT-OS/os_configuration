/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "stdbool.h"

enum
{
    OS_CONFIG_ACCESS_RIGHTS_VECTOR_SIZE = 8,
    OS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY = OS_CONFIG_ACCESS_RIGHTS_VECTOR_SIZE * 8
};

typedef struct
{
    unsigned char rights[OS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY];
} OS_ConfigServiceAccessRights_t;

void OS_ConfigServiceAccessRights_ClearAll(
    OS_ConfigServiceAccessRights_t* accessRights);

void OS_ConfigServiceAccessRights_SetAll(
    OS_ConfigServiceAccessRights_t* accessRights);

void OS_ConfigServiceAccessRights_Set(
    OS_ConfigServiceAccessRights_t* accessRights,
    unsigned int entryIndex);

void OS_ConfigServiceAccessRights_Clear(
    OS_ConfigServiceAccessRights_t* accessRights,
    unsigned int entryIndex);

bool OS_ConfigServiceAccessRights_IsSet(
    OS_ConfigServiceAccessRights_t const* accessRights,
    unsigned int entryIndex);
