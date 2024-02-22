/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @addtogroup AccessRights
 * @{
 *
 * @file OS_ConfigServiceAccessRights.h
 *
 * @brief Collection of functions to manage the access rights for the  module
 */


#pragma once
/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"

/* Exported types/defines/enums ----------------------------------------------*/
enum
{
    OS_CONFIG_ACCESS_RIGHTS_VECTOR_SIZE = 8,
    OS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY = OS_CONFIG_ACCESS_RIGHTS_VECTOR_SIZE * 8
};

typedef struct
{
    unsigned char rights[OS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY];
} OS_ConfigServiceAccessRights_t;

/* Exported functions --------------------------------------------------------*/
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

/** @} */