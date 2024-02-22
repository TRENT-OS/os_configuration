/*
 * Copyright (C) 2019-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/* Includes ------------------------------------------------------------------*/
#include "OS_ConfigServiceAccessRights.h"

/* Exported functions --------------------------------------------------------*/
void OS_ConfigServiceAppIdentifier_WhoAmI(unsigned int* id, unsigned int* maxId)
{
    *id = 0;
    *maxId = 255;
}
