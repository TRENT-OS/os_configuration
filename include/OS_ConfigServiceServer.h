/*
 * Copyright (C) 2020-2024, HENSOLDT Cyber GmbH
 * 
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * For commercial licensing, contact: info.cyber@hensoldt.net
 */

/**
 * @addtogroup ServerWrapper
 * @{
 *
 * @file OS_ConfigServiceServer.h
 *
 * @brief Contains wrapper functions to be called if the ConfigService is built
 * as a server component.
 */

#pragma once

/* Includes ------------------------------------------------------------------*/
#include "camkes/dataport.h"

#include "OS_Error.h"

#include "OS_ConfigServiceDataTypes.h"

/* Exported functions --------------------------------------------------------*/
OS_Error_t
OS_ConfigServiceServer_createHandle(
    intptr_t clientCtx,
    dataport_ptr_t dataport,
    size_t dataportSize,
    OS_ConfigServiceHandle_t* handle);

OS_ConfigServiceLib_t*
OS_ConfigServiceServer_getInstance(void);

OS_Error_t
OS_ConfigServiceServer_domainEnumeratorInit(
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

OS_Error_t
OS_ConfigServiceServer_domainEnumeratorClose(
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

OS_Error_t
OS_ConfigServiceServer_domainEnumeratorReset(
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

OS_Error_t
OS_ConfigServiceServer_domainEnumeratorIncrement(
    OS_ConfigServiceLibTypes_DomainEnumerator_t* enumerator);

OS_Error_t
OS_ConfigServiceServer_domainEnumeratorGetElement(
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Domain_t* domain);

OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorInit(
    OS_ConfigServiceLibTypes_DomainEnumerator_t const* domainEnumerator,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorClose(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorReset(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorIncrement(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* enumerator);

OS_Error_t
OS_ConfigServiceServer_parameterEnumeratorGetElement(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_Parameter_t* parameter);

void
OS_ConfigServiceServer_domainGetName(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_DomainName_t* domainName);

OS_Error_t
OS_ConfigServiceServer_domainCreateParameterEnumerator(
    OS_ConfigServiceLibTypes_Domain_t const* domain,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterEnumerator_t* parameterEnumerator);

OS_Error_t
OS_ConfigServiceServer_domainGetElement(
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

OS_Error_t
OS_ConfigServiceServer_parameterGetValue(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    size_t bufferSize,
    size_t* bytesCopied);

OS_Error_t
OS_ConfigServiceServer_parameterGetValueAsU32(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint32_t* value);

OS_Error_t
OS_ConfigServiceServer_parameterGetValueAsU64(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    uint64_t* value);

OS_Error_t
OS_ConfigServiceServer_parameterGetValueAsString(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    size_t bufferSize);

OS_Error_t
OS_ConfigServiceServer_parameterGetValueAsBlob(
    OS_ConfigServiceLibTypes_Parameter_t const* parameter,
    size_t bufferSize);

OS_Error_t
OS_ConfigServiceServer_parameterSetValue(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    size_t bufferSize);

OS_Error_t
OS_ConfigServiceServer_parameterSetValueAsU32(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint32_t value);

OS_Error_t
OS_ConfigServiceServer_parameterSetValueAsU64(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    uint64_t value);

OS_Error_t
OS_ConfigServiceServer_parameterSetValueAsString(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    size_t bufferSize);

OS_Error_t
OS_ConfigServiceServer_parameterSetValueAsBlob(
    OS_ConfigServiceLibTypes_ParameterEnumerator_t const* enumerator,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    size_t bufferSize);

OS_Error_t
OS_ConfigServiceServer_parameterGetValueFromDomainName(
    OS_ConfigServiceLibTypes_DomainName_t const* domainName,
    OS_ConfigServiceLibTypes_ParameterName_t const* parameterName,
    OS_ConfigServiceLibTypes_ParameterType_t parameterType,
    size_t bufferSize,
    size_t* bytesCopied);

/** @} */
