# TRENTOS ConfigService

## Overview

Provides the TRENTOS ConfigService API that allows to write and retrieve
configuration parameters either by using it locally in
a component or as a separate remote server component.

### Dependencies

* TRENTOS FileSystem

### Build options

The TRENTOS ConfigService can be integrated in three different ways, for
which you will partially have to set specific flags:

| Build Option        | Flag                             |
|:--------------------|:--------------------------------:|
| As a Library        | -                                |
| As a CAmkES Server  | OS_CONFIG_SERVICE_CAMKES_SERVER  |
| As a CAmkES Client  | OS_CONFIG_SERVICE_CAMKES_CLIENT  |

If one of the first two options from the ones presented above are chosen, it is
also necessary to choose a backend option from which the configuration
parameters will be managed. Options here are either setting a filesystem
backend with **OS_CONFIG_SERVICE_BACKEND_FILESYSTEM** or choosing a memory
backend with **OS_CONFIG_SERVICE_BACKEND_MEMORY**.

If the component is built using a filesystem backend, it is recommended to take
a look also at the documentation of the TRENTOS FileSystem in the handbook.
Please also note the usage of the **os_core_api** lib in the next sample code
segment. Along with other things, this also brings in the CAmkES
interface file needed to build a client/server component setup. This will be
explained further in the next section.
A possible CMakeList configuration for a CAmkES ConfigServer component with a
filesystem backend would result in:

    #-------------------------------------------------------------------------------
    # Component ConfigServer
    #-------------------------------------------------------------------------------
    DeclareCAmkESComponent(ConfigServer
        SOURCES
            ...
        C_FLAGS
            ...
            -DOS_CONFIG_SERVICE_BACKEND_FILESYSTEM
            -DOS_CONFIG_SERVICE_CAMKES_SERVER
        LIBS
            ...
            os_libs
            os_core_api
            os_configuration
            os_filesystem
    )

A client component will have to only include the relevant lib and set the correct define:

    #-------------------------------------------------------------------------------
    # Component ExampleClient
    #-------------------------------------------------------------------------------
    DeclareCAmkESComponent(ExampleClient
        SOURCES
            ...
        C_FLAGS
            ...
            -DOS_CONFIG_SERVICE_CAMKES_CLIENT
        LIBS
            ...
            os_core_api
            os_configuration
    )

#### Integrating the CAmkES interface file when building a server component

As mentioned above, the TRENTOS SDK already provides the necessary files
(**os_core_api**) to set up an interface for the ConfigServer built as a
CAmkES component. Simply import the **<if_OS_ConfigService.camkes>** interface
in the CAmkES component file of the ConfigServer component and make sure to name
the procedure **if_OS_ConfigService** and also define a dataport for the clients
to connect to as seen in this example:

    import <if_OS_ConfigService.camkes>;

    component ConfigServer {

        provides if_OS_ConfigService OS_ConfigServiceServer;

        //-------------------------------------------------
        // dataports for clients
        dataport Buf cfg_dataport_buf;
    }

A client would then connect to this server by also integrating the interface and
setting a dataport:

    import <if_OS_ConfigService.camkes>;

    component Client {
        control;

        uses if_OS_ConfigService OS_ConfigServiceServer;
        dataport Buf cfg_dataport_buf;
    }

#### Initialization

Using the ConfigService API requires to include only one header:

    #include "OS_ConfigService.h"

To initialize the local configuration Library in a Server or Library setup, the first step is to obtain the pointer to the local instance.

    OS_ConfigServiceLib_t* configLib =
        OS_ConfigService_getInstance();

If your storage partition already contains a complete configuration file (e.g by
utilizing the Configuration Provisioning Tool), then these are
the steps to follow to initialize this file and be able to and retrieve
configuration values from it. Beware however that the defined filenames in
the example need to be the same ones that were used to create the config file,
otherwise the init process will not be able to parse the file contents. When initializing the Configuration lib with a filesystem backend, you will have
to provide an initialized filesystem handle. The initialization process for this
handle is not in the scope of this guide but can be found in the
FileSystem documentation.

    #define PARAMETER_FILE "PARAM.BIN"
    #define DOMAIN_FILE "DOMAIN.BIN"
    #define STRING_FILE "STRING.BIN"
    #define BLOB_FILE "BLOB.BIN"

    OS_FileSystem_Handle_t hFs;
    OS_ConfigServiceBackend_t parameterBackend;
    OS_ConfigServiceBackend_t domainBackend;
    OS_ConfigServiceBackend_t stringBackend;
    OS_ConfigServiceBackend_t blobBackend;
    OS_ConfigServiceBackend_FileName_t name;

    // small helper function to set the filename
    static
    void initializeName(char* buf, size_t bufSize, char const* name)
    {
        memset(buf, 0, bufSize);
        strncpy(buf, name, bufSize - 1);
    }

    // domain backend
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, DOMAIN_FILE);
    OS_ConfigServiceBackend_initializeFileBackend(&domainBackend, name, hFs);

    // parameter backend
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, PARAMETER_FILE);
    OS_ConfigServiceBackend_initializeFileBackend(&parameterBackend, name, hFs);

    // string parameter backend
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, STRING_FILE);
    OS_ConfigServiceBackend_initializeFileBackend(&stringBackend, name, hFs);

    // blob parameter backend
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, BLOB_FILE);
    OS_ConfigServiceBackend_initializeFileBackend(&blobBackend, name, hFs);

Initializing the Configuration lib with a memory backend does not utilize any
filesystem and you can skip the init process for the filesystem handle. Apart
from that, the
init process is very similar to the one described above for the filesystem but once you start
initializing the different sections, you will be calling the following
functions:

    // domain backend
    OS_ConfigServiceBackend_initializeMemBackend(&domainBackend, domainBuf, sizeof(domainBuf));

    // parameter backend
    OS_ConfigServiceBackend_initializeMemBackend(&parameterBackend, parameterBuf, sizeof(parameterBuf));

    //...

Finally initialize the backend in the config library instance with the different
parameter types:

    OS_ConfigServiceLib_Init(configLib, &parameterBackend, &domainBackend, &stringBackend, &blobBackend);

The described initialization process refers to components building the TRENTOS
Configuration as a library or a server. The initialization for components
acting as clients and connecting to a remote ConfigServer component only require:

    #include "OS_ConfigService.h"

    OS_ConfigService_ClientCtx_t ctx =
    {
        .dataport = OS_DATAPORT_ASSIGN(cfg_dataport_buf)
    };

    OS_ConfigServiceHandle_t configServiceHandle;

    //Create remote handle of the ConfigServer API.
    OS_ConfigService_createHandleRemote(&ctx, &configServiceHandle);

If the ConfigServer is run locally on the client CAmkES component, it is also
possible to create a local handle:

    OS_ConfigServiceHandle_t localHandle;

    //Open a local handle of the ConfigServer API.
    OS_ConfigService_createHandleLocal(&localHandle);

### API Reference

    OS_ConfigServiceLib_t* OS_ConfigService_getInstance(void);

    OS_Error_t OS_ConfigService_createHandleLocal(out OS_ConfigServiceHandle_t handle);
    OS_Error_t OS_ConfigService_createHandleRemote(OS_ConfigService_ClientCtx_t* clientCtx,, out OS_ConfigServiceHandle_t handle);

    OS_Error_t OS_ConfigService_domainEnumeratorInit(OLocalS_ConfigServiceHandle_t handle, out OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator);
    OS_Error_t OS_ConfigService_domainEnumeratorClose(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator);
    OS_Error_t OS_ConfigService_domainEnumeratorReset(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator);
    OS_Error_t OS_ConfigService_domainEnumeratorIncrement(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator);
    OS_Error_t OS_ConfigService_domainEnumeratorGetElement(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator, out OS_ConfigServiceLibTypes_Domain_t domain);

    OS_Error_t OS_ConfigService_parameterEnumeratorInit(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_DomainEnumerator_t domainEnumerator, out OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator);
    OS_Error_t OS_ConfigService_parameterEnumeratorClose(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator);
    OS_Error_t OS_ConfigService_parameterEnumeratorReset(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator);
    OS_Error_t OS_ConfigService_parameterEnumeratorIncrement(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator);
    OS_Error_t OS_ConfigService_parameterEnumeratorGetElement(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, out OS_ConfigServiceLibTypes_Parameter_t parameter);

    void OS_ConfigService_domainGetName(refin OS_ConfigServiceLibTypes_Domain_t domain, out OS_ConfigServiceLibTypes_DomainName_t domainName);
    OS_Error_t OS_ConfigService_domainCreateParameterEnumerator(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Domain_t domain, refin OS_ConfigServiceLibTypes_ParameterName_t parameterName, out OS_ConfigServiceLibTypes_ParameterEnumerator_t parameterEnumerator);
    OS_Error_t OS_ConfigService_domainGetElement(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Domain_t domain, refin OS_ConfigServiceLibTypes_ParameterName_t parameterName, out OS_ConfigServiceLibTypes_Parameter_t parameter);
    void OS_ConfigService_parameterGetName(refin OS_ConfigServiceLibTypes_Parameter_t parameter, out OS_ConfigServiceLibTypes_ParameterName_t parameterName);
    void OS_ConfigService_parameterGetType(refin OS_ConfigServiceLibTypes_Parameter_t parameter, out OS_ConfigServiceLibTypes_ParameterType_t parameterType);
    size_t OS_ConfigService_parameterGetSize(refin OS_ConfigServiceLibTypes_Parameter_t parameter);

    OS_Error_t OS_ConfigService_parameterGetValue(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, size_t bufferLength, out size_t bytesCopied);
    OS_Error_t OS_ConfigService_parameterGetValueAsU32(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, out uint32_t value);
    OS_Error_t OS_ConfigService_parameterGetValueAsU64(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, out uint64_t value);
    OS_Error_t OS_ConfigService_parameterGetValueAsString(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, size_t bufferLength);
    OS_Error_t OS_ConfigService_parameterGetValueAsBlob(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, size_t bufferLength);

    OS_Error_t OS_ConfigService_parameterSetValue(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, OS_ConfigServiceLibTypes_ParameterType_t parameterType, size_t bufferLength);
    OS_Error_t OS_ConfigService_parameterSetValueAsU32(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, uint32_t value);
    OS_Error_t OS_ConfigService_parameterSetValueAsU64(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, uint64_t value);
    OS_Error_t OS_ConfigService_parameterSetValueAsString(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, OS_ConfigServiceLibTypes_ParameterType_t parameterType, size_t bufferLength);
    OS_Error_t OS_ConfigService_parameterSetValueAsBlob(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, OS_ConfigServiceLibTypes_ParameterType_t parameterType, size_t bufferLength);

    OS_Error_t OS_ConfigService_parameterGetValueFromDomainName(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_DomainName_t domainName, refin OS_ConfigServiceLibTypes_ParameterName_t parameterName, OS_ConfigServiceLibTypes_ParameterType_t parameterType, size_t bufferLength, out size_t bytesCopied);
