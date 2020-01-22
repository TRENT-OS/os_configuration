/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "SeosConfigLibTypes.h"
#include "SeosConfigBackend.h"
#include "SeosConfigAccessRights.h"

// An instance of SeosConfig.
typedef struct SeosConfigLib
{
    SeosConfigBackend parameterBackend;
    SeosConfigBackend domainBackend;
    SeosConfigBackend stringBackend;
    SeosConfigBackend blobBackend;
}
SeosConfigLib;

int
SeosConfigLib_Init(
    SeosConfigLib* instance,
    SeosConfigBackend const* parameterBackend,
    SeosConfigBackend const* domainBackend,
    SeosConfigBackend const* stringBackend,
    SeosConfigBackend const* blobBackend);

// Note: enumerator points to first domain. There is always at least one domain.
void
SeosConfigLib_domainEnumeratorInit(SeosConfigLib const* instance,
                                   SeosConfigLib_DomainEnumerator* enumerator);

void
SeosConfigLib_domainEnumeratorClose(SeosConfigLib const* instance,
                                    SeosConfigLib_DomainEnumerator* enumerator);

// Make the enumerator point to the first domain.
void
SeosConfigLib_domainEnumeratorReset(SeosConfigLib const* instance,
                                    SeosConfigLib_DomainEnumerator* enumerator);

// Result: 0 if enumerator could be increased. Enumerator is not changed if it could not be increased.
seos_err_t
SeosConfigLib_domainEnumeratorIncrement(SeosConfigLib const* instance,
                                        SeosConfigLib_DomainEnumerator* enumerator);

// Note: May fail because of failed data retrieval.
seos_err_t
SeosConfigLib_domainEnumeratorGetElement(
    SeosConfigLib* instance,
    SeosConfigLib_DomainEnumerator const* enumerator,
    SeosConfigLib_Domain* domain);


// On success: enumerator points to the first parameter of the given domain.
// Success indication is required: there may not be a parameter for the given domain, failed data retrieval, ...
seos_err_t
SeosConfigLib_parameterEnumeratorInit(
    SeosConfigLib* instance,
    SeosConfigLib_DomainEnumerator const* domainEnumerator,
    SeosConfigLib_ParameterEnumerator* enumerator);

void
SeosConfigLib_parameterEnumeratorClose(SeosConfigLib const* instance,
                                       SeosConfigLib_ParameterEnumerator* enumerator);

// Set the enumerator to the first paramter of the given domain. May fail if there is no parameter in the given domain.
seos_err_t
SeosConfigLib_parameterEnumeratorReset(SeosConfigLib* instance,
                                       SeosConfigLib_ParameterEnumerator* enumerator);

// Result: 0 if enumerator could be increased.
seos_err_t
SeosConfigLib_parameterEnumeratorIncrement(SeosConfigLib* instance,
                                           SeosConfigLib_ParameterEnumerator* enumerator);

// Note: May fail because of failed data retrieval. Assume parameter enumerator is valid.
seos_err_t
SeosConfigLib_parameterEnumeratorGetElement(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_Parameter* parameter);

// Extracts the domain name fromt he given domain.
void
SeosConfigLib_domainGetName(
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_DomainName* domainName);

// Create a parameter enumerator pointing to the parameter with the given name in the given domain.
seos_err_t
SeosConfigLib_domainCreateParameterEnumerator(
    SeosConfigLib* instance,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_ParameterEnumerator* parameterEnumerator);


// Find parameter by name in given domain.
seos_err_t
SeosConfigLib_domainGetElement(
    SeosConfigLib* instance,
    SeosConfigLib_Domain const* domain,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_Parameter* parameter);

// Get the parameter name.
void
SeosConfigLib_parameterGetName(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterName* parameterName);

// Get the parameter type.
void
SeosConfigLib_parameterGetType(
    SeosConfigLib_Parameter const* parameter,
    SeosConfigLib_ParameterType* parameterType);

// Get the parameter size.
size_t
SeosConfigLib_parameterGetSize(
    SeosConfigLib_Parameter const* parameter);

// Get the parameter value - raw format.
seos_err_t
SeosConfigLib_parameterGetValue(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied);

// Get the parameter value - specific format.
seos_err_t
SeosConfigLib_parameterGetValueAsU32(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    uint32_t* value);

// Get the parameter value - specific format.
seos_err_t
SeosConfigLib_parameterGetValueAsU64(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    uint64_t* value);

// Get the parameter value - specific format.
seos_err_t
SeosConfigLib_parameterGetValueAsString(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    char* buffer,
    size_t bufferLength);

// Get the parameter value - specific format.
seos_err_t
SeosConfigLib_parameterGetValueAsBlob(
    SeosConfigLib* instance,
    SeosConfigLib_Parameter const* parameter,
    void* buffer,
    size_t bufferLength);

// Set the parameter value - raw format.
seos_err_t
SeosConfigLib_parameterSetValue(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    void const* buffer,
    size_t bufferLength);

// Set the parameter value - specific format.
seos_err_t
SeosConfigLib_parameterSetValueAsU32(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint32_t value);

// Set the parameter value - specific format.
seos_err_t
SeosConfigLib_parameterSetValueAsU64(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint64_t value);

// Set the parameter value - specific format.
seos_err_t
SeosConfigLib_parameterSetValueAsString(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    char const* buffer,
    size_t bufferLength);

// Set the parameter value - specific format.
seos_err_t
SeosConfigLib_parameterSetValueAsBlob(
    SeosConfigLib* instance,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    SeosConfigLib_ParameterType parameterType,
    void const* buffer,
    size_t bufferLength);

seos_err_t
SeosConfigLib_parameterGetValueFromDomainName(
    SeosConfigLib* instance,
    SeosConfigLib_DomainName const* domainName,
    SeosConfigLib_ParameterName const* parameterName,
    SeosConfigLib_ParameterType parameterType,
    void* buffer,
    size_t bufferLength,
    size_t* bytesCopied);
