/**
 * Copyright (C) 2020, HENSOLDT Cyber GmbH
 *
 * @addtogroup AppIdentifier
 *
 * @{
 *
 * @file OS_ConfigServiceAppIdentifier.h
 *
 * @brief Module to manage the identification of an app.
 *
 */


#pragma once

/* Exported functions --------------------------------------------------------*/
void OS_ConfigServiceAppIdentifier_WhoAmI(
    unsigned int* id,
    unsigned int* maxId);

/** @} */