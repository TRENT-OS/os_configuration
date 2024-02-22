/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
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