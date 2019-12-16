/*
   *  Temp. header "seos.h" for filesystem demo header.
   *
   *  Copyright (C) 2019, Hensoldt Cyber GmbH
*/


#pragma once


// filesystem
#if defined(FS_SUPPORT)
#include "seos_fs.h"    // include path to fs-core must be set in cmakelists.txt
#endif


#include "SeosError.h"

#if 0
// partition manager
#include "seos_pm.h"    // include path to partition manager must be set in cmakelists.txt
#endif