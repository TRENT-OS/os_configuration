/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "SeosError.h"

#include "seos_config_data_types.h"
#include "seos_config_instance_store.h"

SeosConfigInstanceStore*
seos_configuration_getInstances(void);

seos_err_t
seos_configuration_createHandle(
    SeosConfigLib_HandleKind  handleKind,
    unsigned int              id,
    SeosConfigHandle*         handle);

seos_err_t
seos_configuration_domainEnumeratorInit(
    SeosConfigHandle                 handle,
    SeosConfigLib_DomainEnumerator*  enumerator);

seos_err_t
seos_configuration_domainEnumeratorClose(
    SeosConfigHandle                 handle,
    SeosConfigLib_DomainEnumerator*  enumerator);

seos_err_t
seos_configuration_domainEnumeratorReset(
    SeosConfigHandle                 handle,
    SeosConfigLib_DomainEnumerator*  enumerator);


seos_err_t
seos_configuration_domainEnumeratorIncrement(
    SeosConfigHandle                 handle,
    SeosConfigLib_DomainEnumerator*  enumerator);

seos_err_t
seos_configuration_domainEnumeratorGetElement(
    SeosConfigHandle                       handle,
    SeosConfigLib_DomainEnumerator const*  enumerator,
    SeosConfigLib_Domain*                  domain);


seos_err_t
seos_configuration_parameterEnumeratorInit(
    SeosConfigHandle                       handle,
    SeosConfigLib_DomainEnumerator const*  domainEnumerator,
    SeosConfigLib_ParameterEnumerator*     enumerator);

seos_err_t
seos_configuration_parameterEnumeratorClose(
    SeosConfigHandle                    handle,
    SeosConfigLib_ParameterEnumerator*  enumerator);

seos_err_t
seos_configuration_parameterEnumeratorReset(
    SeosConfigHandle                    handle,
    SeosConfigLib_ParameterEnumerator*  enumerator);

seos_err_t
seos_configuration_parameterEnumeratorIncrement(
    SeosConfigHandle                    handle,
    SeosConfigLib_ParameterEnumerator*  enumerator);

seos_err_t
seos_configuration_parameterEnumeratorGetElement(
    SeosConfigHandle                          handle,
    SeosConfigLib_ParameterEnumerator const*  enumerator,
    SeosConfigLib_Parameter*                  parameter);

void
seos_configuration_domainGetName(
    SeosConfigLib_Domain const*  domain,
    SeosConfigLib_DomainName*    domainName);

seos_err_t
seos_configuration_domainCreateParameterEnumerator(
    SeosConfigHandle                    handle,
    SeosConfigLib_Domain const*         domain,
    SeosConfigLib_ParameterName const*  parameterName,
    SeosConfigLib_ParameterEnumerator*  parameterEnumerator);

seos_err_t
seos_configuration_domainGetElement(
    SeosConfigHandle                    handle,
    SeosConfigLib_Domain const*         domain,
    SeosConfigLib_ParameterName const*  parameterName,
    SeosConfigLib_Parameter*            parameter);

void
seos_configuration_parameterGetName(
    SeosConfigLib_Parameter const*  parameter,
    SeosConfigLib_ParameterName*    parameterName);

void
seos_configuration_parameterGetType(
    SeosConfigLib_Parameter const*  parameter,
    SeosConfigLib_ParameterType*    parameterType);

size_t
seos_configuration_parameterGetSize(
    SeosConfigLib_Parameter const*  parameter);

seos_err_t
seos_configuration_parameterGetValue(
    SeosConfigHandle                handle,
    SeosConfigLib_Parameter const*  parameter,
    void*                           buffer,
    size_t                          bufferLength,
    size_t*                         bytesCopied);

seos_err_t
seos_configuration_parameterGetValueAsU32(
    SeosConfigHandle                handle,
    SeosConfigLib_Parameter const*  parameter,
    uint32_t*                       value);

seos_err_t
seos_configuration_parameterGetValueAsU64(
    SeosConfigHandle                handle,
    SeosConfigLib_Parameter const*  parameter,
    uint64_t*                       value);

seos_err_t
seos_configuration_parameterGetValueAsString(
    SeosConfigHandle                handle,
    SeosConfigLib_Parameter const*  parameter,
    char*                           buffer,
    size_t                          bufferLength);

seos_err_t
seos_configuration_parameterGetValueAsBlob(
    SeosConfigHandle                handle,
    SeosConfigLib_Parameter const*  parameter,
    void*                           buffer,
    size_t                          bufferLength);

seos_err_t
seos_configuration_parameterSetValue(
    SeosConfigHandle                          handle,
    SeosConfigLib_ParameterEnumerator const*  enumerator,
    SeosConfigLib_ParameterType               parameterType,
    void const*                               buffer,
    size_t                                    bufferLength);

seos_err_t
seos_configuration_parameterSetValueAsU32(
    SeosConfigHandle                          handle,
    SeosConfigLib_ParameterEnumerator const*  enumerator,
    uint32_t                                  value);

seos_err_t
seos_configuration_parameterSetValueAsU64(
    SeosConfigHandle                          handle,
    SeosConfigLib_ParameterEnumerator const* enumerator,
    uint64_t                          value);

seos_err_t
seos_configuration_parameterSetValueAsString(
    SeosConfigHandle                          handle,
    SeosConfigLib_ParameterEnumerator const*  enumerator,
    SeosConfigLib_ParameterType               parameterType,
    char const*                               buffer,
    size_t                                    bufferLength);

seos_err_t
seos_configuration_parameterSetValueAsBlob(
    SeosConfigHandle                          handle,
    SeosConfigLib_ParameterEnumerator const*  enumerator,
    SeosConfigLib_ParameterType               parameterType,
    void const*                               buffer,
    size_t                                    bufferLength);
