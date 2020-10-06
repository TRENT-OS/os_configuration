/**
 * Copyright (C) 2020, Hensoldt Cyber GmbH
 *
 * @addtogroup Lib
 * @{
 *
 * @file OS_ConfigServiceLib.h
 *
 * @brief Contains core functions offered by the ConfigService library.
 *
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "OS_ConfigServiceLibTypes.h"
#include "OS_ConfigServiceBackend.h"
#include "OS_ConfigServiceAccessRights.h"

// An instance of OS_ConfigService.
typedef struct
{
    OS_ConfigServiceBackend_t parameterBackend;
    OS_ConfigServiceBackend_t domainBackend;
    OS_ConfigServiceBackend_t stringBackend;
    OS_ConfigServiceBackend_t blobBackend;
}
OS_ConfigServiceLib_t;

/* Exported functions --------------------------------------------------------*/
OS_Error_t
OS_ConfigServiceLib_Init(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceBackend_t const* parameterBackend,
    OS_ConfigServiceBackend_t const* domainBackend,
    OS_ConfigServiceBackend_t const* stringBackend,
    OS_ConfigServiceBackend_t const* blobBackend);

// Note: enumerator points to first domain. There is always at least one domain.
void
OS_ConfigServiceLib_domainEnumeratorInit(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

void
OS_ConfigServiceLib_domainEnumeratorClose(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

// Make the enumerator point to the first domain.
void
OS_ConfigServiceLib_domainEnumeratorReset(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

// Result: 0 if enumerator could be increased. Enumerator is not changed if it could not be increased.
OS_Error_t
OS_ConfigServiceLib_domainEnumeratorIncrement(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

// Note: May fail because of failed data retrieval.
OS_Error_t
OS_ConfigServiceLib_domainEnumeratorGetElement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain);

// On success: enumerator points to the first parameter of the given domain.
// Success indication is required: there may not be a parameter for the given domain, failed data retrieval, ...
OS_Error_t
OS_ConfigServiceLib_parameterEnumeratorInit(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

void
OS_ConfigServiceLib_parameterEnumeratorClose(
    OS_ConfigServiceLib_t const* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

// Set the enumerator to the first paramter of the given domain. May fail if there is no parameter in the given domain.
OS_Error_t
OS_ConfigServiceLib_parameterEnumeratorReset(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

// Result: 0 if enumerator could be increased.
OS_Error_t
OS_ConfigServiceLib_parameterEnumeratorIncrement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

// Note: May fail because of failed data retrieval. Assume parameter enumerator is valid.
OS_Error_t
OS_ConfigServiceLib_parameterEnumeratorGetElement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter);

// Extracts the domain name fromt he given domain.
void
OS_ConfigServiceLib_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName);

// Create a parameter enumerator pointing to the parameter with the given name in the given domain.
OS_Error_t
OS_ConfigServiceLib_domainCreateParameterEnumerator(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator);

// Find parameter by name in given domain.
OS_Error_t
OS_ConfigServiceLib_domainGetElement(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter);

// Get the parameter name.
void
OS_ConfigServiceLib_parameterGetName(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterName_t* parameterName);

// Get the parameter type.
void
OS_ConfigServiceLib_parameterGetType(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t* parameterType);

// Get the parameter size.
size_t
OS_ConfigServiceLib_parameterGetSize(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter);

// Get the parameter value - raw format.
OS_Error_t
OS_ConfigServiceLib_parameterGetValue(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied);

// Get the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterGetValueAsU32(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value);

// Get the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterGetValueAsU64(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value);

// Get the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterGetValueAsString(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    char* buffer,
    size_t bufferLength);

// Get the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterGetValueAsBlob(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    void* buffer,
    size_t bufferLength);

// Set the parameter value - raw format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValue(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength);

// Set the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValueAsU32(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value);

// Set the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValueAsU64(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value);

// Set the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValueAsString(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    char const* buffer,
    size_t bufferLength);

// Set the parameter value - specific format.
OS_Error_t
OS_ConfigServiceLib_parameterSetValueAsBlob(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void const* buffer,
    size_t bufferLength);

OS_Error_t
OS_ConfigServiceLib_parameterGetValueFromDomainName(
    OS_ConfigServiceLib_t* instance,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied);

/** @} */
