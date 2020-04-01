/*
 *  Copyright (C) 2019, Hensoldt Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include "OS_ConfigServiceAccessRights.h"

/* Exported functions --------------------------------------------------------*/
void OS_ConfigServiceAppIdentifier_WhoAmI(unsigned int* id, unsigned int* maxId)
{
    *id = 0;
    *maxId = 255;
}
