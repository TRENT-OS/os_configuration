/*
 *  Copyright (C) 2019, HENSOLDT Cyber GmbH
 */

/* Includes ------------------------------------------------------------------*/
#include "OS_ConfigServiceAccessRights.h"

/* Exported functions --------------------------------------------------------*/
void OS_ConfigServiceAccessRights_ClearAll(
    OS_ConfigServiceAccessRights_t* accessRights)
{
    for (unsigned int k = 0; k < OS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY; ++k)
    {
        OS_ConfigServiceAccessRights_Clear(accessRights, k);
    }
}

//------------------------------------------------------------------------------
void OS_ConfigServiceAccessRights_SetAll(
    OS_ConfigServiceAccessRights_t* accessRights)
{
    for (unsigned int k = 0; k < OS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY; ++k)
    {
        OS_ConfigServiceAccessRights_Set(accessRights, k);
    }
}

//------------------------------------------------------------------------------
void OS_ConfigServiceAccessRights_Set(
    OS_ConfigServiceAccessRights_t* accessRights,
    unsigned int entryIndex)
{
    if (entryIndex < OS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY)
    {
        unsigned int index = entryIndex >> 3;
        unsigned int bitNumber = entryIndex & 0x03;

        accessRights->rights[index] |= 1u << bitNumber;
    }
    else
    {
    }
}

//------------------------------------------------------------------------------
void OS_ConfigServiceAccessRights_Clear(
    OS_ConfigServiceAccessRights_t* accessRights,
    unsigned int entryIndex)
{
    if (entryIndex < OS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY)
    {
        unsigned int index = entryIndex >> 3;
        unsigned int bitNumber = entryIndex & 0x03;

        accessRights->rights[index] &= ~(1u << bitNumber);
    }
    else
    {
    }
}

//------------------------------------------------------------------------------
bool OS_ConfigServiceAccessRights_IsSet(
    OS_ConfigServiceAccessRights_t const* accessRights,
    unsigned int entryIndex)
{
    if (entryIndex < OS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY)
    {
        unsigned int index = entryIndex >> 3;
        unsigned int bitNumber = entryIndex & 0x03;

        return 0 != (accessRights->rights[index] & (1u << bitNumber));
    }
    else
    {
        return false;
    }
}
