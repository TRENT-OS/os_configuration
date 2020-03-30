/**
 * Copyright (C) 2019, Hensoldt Cyber GmbH
 */

#pragma once

#include "camkes.h"

#include "SeosError.h"

#include "OS_ConfigServiceDataTypes.h"
#include "OS_ConfigServiceInstanceStore.h"

OS_ConfigServiceInstanceStore_t*
OS_ConfigServiceServer_getInstances(void);

seos_err_t
OS_ConfigServiceServer_createHandle(
    OS_ConfigServiceHandle_HandleKind_t handleKind,
    unsigned int id,
    OS_ConfigServiceHandle_t* handle);

seos_err_t
OS_ConfigServiceServer_domainEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

seos_err_t
OS_ConfigServiceServer_domainEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

seos_err_t
OS_ConfigServiceServer_domainEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);


seos_err_t
OS_ConfigServiceServer_domainEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

seos_err_t
OS_ConfigServiceServer_domainEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain);


seos_err_t
OS_ConfigServiceServer_parameterEnumeratorInit(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

seos_err_t
OS_ConfigServiceServer_parameterEnumeratorClose(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

seos_err_t
OS_ConfigServiceServer_parameterEnumeratorReset(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

seos_err_t
OS_ConfigServiceServer_parameterEnumeratorIncrement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

seos_err_t
OS_ConfigServiceServer_parameterEnumeratorGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter);

void
OS_ConfigServiceServer_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName);

seos_err_t
OS_ConfigServiceServer_domainCreateParameterEnumerator(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator);

seos_err_t
OS_ConfigServiceServer_domainGetElement(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_Parameter_t* parameter);

void
OS_ConfigServiceServer_parameterGetName(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterName_t* parameterName);

void
OS_ConfigServiceServer_parameterGetType(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    OS_ConfigServiceLibTypes_ParameterType_t* parameterType);

size_t
OS_ConfigServiceServer_parameterGetSize(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter);

seos_err_t
OS_ConfigServiceServer_parameterGetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    dataport_ptr_t buffer,
    size_t bufferLength,
    size_t* bytesCopied);

seos_err_t
OS_ConfigServiceServer_parameterGetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value);

seos_err_t
OS_ConfigServiceServer_parameterGetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value);

seos_err_t
OS_ConfigServiceServer_parameterGetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    dataport_ptr_t buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigServiceServer_parameterGetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    dataport_ptr_t buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigServiceServer_parameterSetValue(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigServiceServer_parameterSetValueAsU32(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value);

seos_err_t
OS_ConfigServiceServer_parameterSetValueAsU64(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value);

seos_err_t
OS_ConfigServiceServer_parameterSetValueAsString(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigServiceServer_parameterSetValueAsBlob(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength);

seos_err_t
OS_ConfigServiceServer_parameterGetValueFromDomainName(
    OS_ConfigServiceHandle_t handle,
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    dataport_ptr_t buffer,
    size_t bufferLength,
    size_t* bytesCopied);
