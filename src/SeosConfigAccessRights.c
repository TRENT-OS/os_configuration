/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#include "SeosConfigAccessRights.h"

void SeosConfigAccessRights_ClearAll(SeosConfigAccessRights* accessRights)
{
    for (unsigned int k = 0; k < SEOS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY; ++k)
    {
        SeosConfigAccessRights_Clear(accessRights, k);
    }
}


void SeosConfigAccessRights_SetAll(SeosConfigAccessRights* accessRights)
{
    for (unsigned int k = 0; k < SEOS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY; ++k)
    {
        SeosConfigAccessRights_Set(accessRights, k);
    }
}


void SeosConfigAccessRights_Set(SeosConfigAccessRights* accessRights,
                                unsigned int entryIndex)
{
    if (entryIndex < SEOS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY)
    {
        unsigned int index = entryIndex >> 3;
        unsigned int bitNumber = entryIndex & 0x03;

        accessRights->rights[index] |= 1u << bitNumber;
    }
    else
    {
    }
}


void SeosConfigAccessRights_Clear(SeosConfigAccessRights* accessRights,
                                  unsigned int entryIndex)
{
    if (entryIndex < SEOS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY)
    {
        unsigned int index = entryIndex >> 3;
        unsigned int bitNumber = entryIndex & 0x03;

        accessRights->rights[index] &= ~(1u << bitNumber);
    }
    else
    {
    }
}


bool SeosConfigAccessRights_IsSet(SeosConfigAccessRights const* accessRights,
                                  unsigned int entryIndex)
{
    if (entryIndex < SEOS_CONFIG_ACCESS_RIGHTS_MAX_ENTRY)
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
