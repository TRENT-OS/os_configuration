/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Lib
 * @{
 *
 * @file OS_ConfigServiceLibTypes.h
 *
 * @brief Collection of data structures defined for the ConfigService.
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stddef.h"

#include "OS_ConfigServiceAccessRights.h"

/* Exported types/defines/enums ----------------------------------------------*/

// Limits the user of OS_ConfigService has to know.
// Important: these limits have to be small enough to be allocated on the stack.
typedef enum
{
    OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH = 32,
    OS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH = 64,
    OS_CONFIG_LIB_PARAMETER_NAME_LEN = 32,
    OS_CONFIG_LIB_DOMAIN_NAME_LEN = 32,
} OS_ConfigServiceLibTypes_Limits_t;


// Enumeration of the defined types of configuration parameters.
typedef enum
{
    OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32,
    OS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64,
    OS_CONFIG_LIB_PARAMETER_TYPE_STRING,
    OS_CONFIG_LIB_PARAMETER_TYPE_BLOB,
} OS_ConfigServiceLibTypes_ParameterType_t;


// The name of a paramter.
typedef struct
{
    char name[OS_CONFIG_LIB_PARAMETER_NAME_LEN];
}
OS_ConfigServiceLibTypes_ParameterName_t;


// The value of a string paramter. The index points into a dedicated record backend.
typedef struct
{
    // The storage layout of these structs is required to be the same accross
    // different platforms so the int size needs to be explicit.
    uint32_t size;
    uint32_t index;
}
__attribute__((packed)) OS_ConfigServiceLibTypes_ParameterString_t;


// The value of a string paramter. The index points into a dedicated record backend.
typedef struct
{
    // The storage layout of these structs is required to be the same accross
    // different platforms so the int size needs to be explicit.
    uint32_t size;
    uint32_t index;
    uint32_t numberOfBlocks;
}
__attribute__((packed)) OS_ConfigServiceLibTypes_ParameterBlob_t;


// Internal: a paramter value.
// Note for future extensions: this data structure's size should be kept
// reasonable = it should be no problem to allocate it on the stack.
typedef union
{
    // The storage layout of these structs is required to be the same accross
    // different platforms so the int size needs to be explicit.
    // private (= only to be used by OS_ConfigService implementation)
    uint32_t valueInteger32;
    uint64_t valueInteger64;
    OS_ConfigServiceLibTypes_ParameterString_t valueString;
    OS_ConfigServiceLibTypes_ParameterBlob_t valueBlob;
}
__attribute__((packed)) OS_ConfigServiceLibTypes_ParameterValue_t;


// Points to a domain (contained in a dedicated record backend).
// Will always point to an existing domain.
typedef struct
{
    // The storage layout of these structs is required to be the same accross
    // different platforms so the int size needs to be explicit.
    // private (= only to be used by OS_ConfigService implementation)
    uint32_t index;
}
__attribute__((packed)) OS_ConfigServiceLibTypes_DomainEnumerator_t;


// Points to a parameter (contained in a dedicated record backend).
// Will always point to an existing paramter. It will always point to a parameter
// of the given domain if a) the domain does actually have parameters which b)
// the current user is allowed to see.
typedef struct
{
    // The storage layout of these structs is required to be the same accross
    // different platforms so the int size needs to be explicit.
    // private (= only to be used by OS_ConfigService implementation)
    uint32_t index;
    OS_ConfigServiceLibTypes_DomainEnumerator_t domainEnumerator;
}
OS_ConfigServiceLibTypes_ParameterEnumerator_t;


// A parameter.
typedef struct
{
    // private (= only to be used by OS_ConfigService implementation)
    OS_ConfigServiceLibTypes_ParameterType_t parameterType;
    OS_ConfigServiceLibTypes_ParameterName_t parameterName;
    OS_ConfigServiceLibTypes_DomainEnumerator_t domain;
    OS_ConfigServiceLibTypes_ParameterValue_t parameterValue;
    OS_ConfigServiceAccessRights_t readAccess;
    OS_ConfigServiceAccessRights_t writeAccess;
}
OS_ConfigServiceLibTypes_Parameter_t;


// Name of a domain.
typedef struct
{
    char name[OS_CONFIG_LIB_DOMAIN_NAME_LEN];
}
OS_ConfigServiceLibTypes_DomainName_t;


// A domain. Its name is cached here - it can always be retrieved from the domain backend.
typedef struct
{
    // private (= only to be used by OS_ConfigService implementation)
    OS_ConfigServiceLibTypes_DomainName_t name;
    OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator;
}
OS_ConfigServiceLibTypes_Domain_t;

/** @} */