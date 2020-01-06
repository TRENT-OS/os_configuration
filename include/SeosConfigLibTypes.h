/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "stdint.h"
#include "stddef.h"

#include "SeosConfigAccessRights.h"

// Limits the user of SeosConfig has to know.
// Important: these limits have to be small enough to be allocated on the stack.
typedef enum
{
    SEOS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH = 128,
    SEOS_CONFIG_LIB_PARAMETER_MAX_BLOB_BLOCK_LENGTH = 3000,
    SEOS_CONFIG_LIB_PARAMETER_NAME_LEN = 32,
    SEOS_CONFIG_LIB_DOMAIN_NAME_LEN = 32,
} SeosConfigLib_Limits;

// Enumeration of the defined types of configuration parameters.
typedef enum
{
    SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER32,
    SEOS_CONFIG_LIB_PARAMETER_TYPE_INTEGER64,
    SEOS_CONFIG_LIB_PARAMETER_TYPE_STRING,
    SEOS_CONFIG_LIB_PARAMETER_TYPE_BLOB,
} SeosConfigLib_ParameterType;

// The name of a paramter.
typedef struct SeosConfigLib_ParameterName
{
    char name[SEOS_CONFIG_LIB_PARAMETER_NAME_LEN];
}
SeosConfigLib_ParameterName;

// The value of a string paramter. The index points into a dedicated record backend.
typedef struct SeosConfigLib_ParameterString
{
    size_t size;
    uint32_t index;
}
SeosConfigLib_ParameterString;

// The value of a string paramter. The index points into a dedicated record backend.
typedef struct SeosConfigLib_ParameterBlob
{
    size_t size;
    uint32_t index;
    uint32_t numberOfBlocks;
}
SeosConfigLib_ParameterBlob;

// Internal: a paramter value.
// Note for future extensions: this data structure's size should be kept
// reasonable = it should be no problem to allocate it on the stack.
typedef union SeosConfigLib_ParameterValue
{
    // private (= only to be used by SeosConfig implementation)
    uint32_t valueInteger32;
    uint64_t valueInteger64;
    SeosConfigLib_ParameterString valueString;
    SeosConfigLib_ParameterBlob valueBlob;
}
SeosConfigLib_ParameterValue;

// Points to a domain (contained in a dedicated record backend).
// Will always point to an existing domain.
typedef struct SeosConfigLib_DomainEnumerator
{
    // private (= only to be used by SeosConfig implementation)
    unsigned int index;
}
SeosConfigLib_DomainEnumerator;

// Points to a parameter (contained in a dedicated record backend).
// Will always point to an existing paramter. It will always point to a parameter
// of the given domain if a) the domain does actually have parameters which b)
// the current user is allowed to see.
typedef struct SeosConfigLib_ParameterEnumerator
{
    // private (= only to be used by SeosConfig implementation)
    unsigned int index;
    SeosConfigLib_DomainEnumerator domainEnumerator;
}
SeosConfigLib_ParameterEnumerator;

// A parameter.
typedef struct SeosConfigLib_Parameter
{
    // private (= only to be used by SeosConfig implementation)
    SeosConfigLib_ParameterType parameterType;
    SeosConfigLib_ParameterName parameterName;
    SeosConfigLib_DomainEnumerator domain;
    SeosConfigLib_ParameterValue parameterValue;
    SeosConfigAccessRights readAccess;
    SeosConfigAccessRights writeAccess;
}
SeosConfigLib_Parameter;

// Name of a domain.
typedef struct SeosConfigLib_DomainName
{
    char name[SEOS_CONFIG_LIB_DOMAIN_NAME_LEN];
}
SeosConfigLib_DomainName;

// A domain. Its name is cached here - it can always be retrieved from the domain backend.
typedef struct SeosConfigLib_Domain
{
    // private (= only to be used by SeosConfig implementation)
    SeosConfigLib_DomainName name;
    SeosConfigLib_DomainEnumerator enumerator;
}
SeosConfigLib_Domain;


