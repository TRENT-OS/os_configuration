/*
 *  Copyright (C) 2019, HENSOLDT Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include "OS_ConfigServiceAccessRights.h"

/* Exported functions --------------------------------------------------------*/
void OS_ConfigServiceAppIdentifier_WhoAmI(unsigned int* id, unsigned int* maxId)
{
    *id = 0;
    *maxId = 255;
}
