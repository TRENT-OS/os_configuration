# TRENTOS ConfigService


## Overview

Provides the TRENTOS ConfigService API that allows to write and retrieve
configuration parameters either by using it locally in
a component or as a separate remote server component.


### Dependencies

* PartitionManager
* FileSystem


### Build options

The TRENTOS ConfigService can be integrated in three different ways, for
which you will partially have to set specific flags:

| Build Option        | Flag                             |
|:--------------------|:--------------------------------:|
| As a Library        | -                                |
| As a CAmkES Server  | OS_CONFIG_SERVICE_CAMKES_SERVER  |
| As a CAmkES Client  | OS_CONFIG_SERVICE_CAMKES_CLIENT  |

If one of the first two options from the ones presented above are chosen, it is
also necessary to choose a backend option to which the configuration
parameters will be written to. Options here are either setting a filesystem
backend with **OS_CONFIG_SERVICE_BACKEND_FILESYSTEM** or choosing a memory
backend with **OS_CONFIG_SERVICE_BACKEND_MEMORY**.

If the component is built using a filesystem backend, it is recommended to take
a look also at the documentation of the TRENTOS FileSystemCore and
PartitionManager libraries.
Please also note the usage of the **seos_core_api** lib in the next sample code
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
            components/ConfigServer/src/ConfigServer.c
        C_FLAGS
            -Wall
            -Werror
            # filesystem
            -DOS_FS_BUILD_AS_LIB_BASIC_HANDLE
            # partition manager
            -DOS_PARTITION_MANAGER_BUILD_AS_COMPONENT # uses the PartitionManager as a component
            # config server
            -DOS_CONFIG_SERVICE_BACKEND_FILESYSTEM # indicates the use of a filesystem backend
            -DOS_CONFIG_SERVICE_CAMKES_SERVER     # indicates a CAmkES configuration server is being built
        LIBS
            seos_system_config
            seos_libs
            seos_core_api
            seos_configuration
            seos_partition_manager_api
            seos_filesystem_core
            seos_filesystem_fat
            seos_filesystem_spiffs
    )

A client component will have to only include the relevant lib and set the correct define:

    #-------------------------------------------------------------------------------
    # Component ExampleClient
    #-------------------------------------------------------------------------------
    DeclareCAmkESComponent(ExampleClient
        SOURCES
            components/ExampleClient/src/ExampleClient.c
        C_FLAGS
            -Wall
            -Werror
            -DOS_CONFIG_SERVICE_CAMKES_CLIENT
        LIBS
            seos_system_config
            seos_core_api
            seos_configuration
    )

#### Integrating the CAmkES interface file when building a server component:
As mentioned above, the TRENTOS SDK already provides the necessary files
(**seos_core_api**) to set up an interface for the ConfigServer built as a
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

When initializing the Configuration lib with a filesystem backend, you will also
have to first initialize a partition object with the partition ID you
want to write to. The init process of the this can be found in the documentation
of the PartitionManager lib and won't be covered in the following example.

    #include "OS_ConfigServiceServer.h"   // use "OS_ConfigServiceLibrary.h" if you are using the library build option

If you are building the component using the functions offered as a library, you
would call OS_ConfigServiceLibrary_getInstance() but the other following
steps are the same:

    OS_ConfigServiceInstanceStore_t* serverInstanceStore =
        OS_ConfigServiceServer_getInstances();

    OS_ConfigServiceLib_t* configLib =
        OS_ConfigServiceInstanceStore_getInstance(serverInstanceStore, 0);


If your partition already contains a complete configuration file, then these are
the steps to follow to initialize this file and be able to write and retrieve
configuration parameters from it. Beware however that the defined filenames in
the example need to be the same ones that were used to create the config file,
otherwise the init process will not be able to parse the file contents.

    #define PARAMETER_FILE "PARAM.BIN"
    #define DOMAIN_FILE "DOMAIN.BIN"
    #define STRING_FILE "STRING.BIN"
    #define BLOB_FILE "BLOB.BIN"

    hPartition_t phandle;
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
    OS_ConfigServiceBackend_initializeFileBackend(&domainBackend, name, phandle);

    // parameter backend
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, PARAMETER_FILE);
    OS_ConfigServiceBackend_initializeFileBackend(&parameterBackend, name, phandle);

    // string parameter backend
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, STRING_FILE);
    OS_ConfigServiceBackend_initializeFileBackend(&stringBackend, name, phandle);

    // blob parameter backend
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, BLOB_FILE);
    OS_ConfigServiceBackend_initializeFileBackend(&blobBackend, name, phandle);


Initializing the Configuration lib with a memory backend does not have any
dependencies to the PartitionManager and so you can skip the init process
of this additional layer since you will be writing the parameters directly into
memory of the component integrating this library. The init process
is very similar to the one described above for the filesystem but once you start
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

    OS_ConfigServiceHandle_t remoteHandle;

    //Open remote handle of the ConfigServer API
    OS_ConfigService_createHandle(OS_CONFIG_HANDLE_KIND_RPC, <HANDLE_ID>, &remoteHandle);

If the ConfigServer is run locally on the client CAmkES component, it is also
possible to create a local handle:

    OS_ConfigServiceHandle_t localHandle;

    //Open a local handle of the ConfigServer API
    OS_ConfigService_createHandle(OS_CONFIG_HANDLE_KIND_LOCAL, <HANDLE_ID>, &localHandle);


#### Setting up a Configuration backend

As mentioned in the previous section, the initialization requires an already
created configuration file or a memory backend that was already provisioned
to the desired set of parameters to read and write from. This section therefore
provides some code examples to set up a configuration backend.
Depending on which parameter backend types are used, the following function has
to be called to create a filesystem backend:

    // create the desired number of domains
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, DOMAIN_FILE);
    OS_ConfigServiceBackend_createFileBackend(name, phandle, <NUMBER_OF_DOMAINS>, sizeof(OS_ConfigServiceLibTypes_Domain_t));

    // create the desired number of parameters
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, PARAMETER_FILE);
    OS_ConfigServiceBackend_createFileBackend(name, phandle, <NUMBER_OF_PARAMETERS>, sizeof(OS_ConfigServiceLibTypes_Parameter_t));

    // create the desired number of string parameters
    initializeName(name.buffer, OS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, STRING_FILE);
    OS_ConfigServiceBackend_createFileBackend(name, phandle, <NUMBER_OF_STRINGS>, OS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH);

    //...

The same goes for a memory backend:

    // sizes of the buffers are defined to at least cover the total size of the
    // parameters added to the backend
    static char parameterBuf[12000];
    static char domainBuf[2000];

    // create the desired number of domains
    OS_ConfigServiceBackend_createMemBackend(domainBuf, sizeof(domainBuf), <NUMBER_OF_DOMAINS>, sizeof(OS_ConfigServiceLibTypes_Domain_t));

    // create the desired number of parameters
    OS_ConfigServiceBackend_createMemBackend(parameterBuf, sizeof(parameterBuf), <NUMBER_OF_PARAMETERS>, sizeof(OS_ConfigServiceLibTypes_Parameter_t));

    //...

After creating this backend frame and initializing it you can write parameter
and domain value to it as seen in this example:

    // initialize Domain
    OS_ConfigServiceLibTypes_Domain_t domain;
    strncpy(domain->name.name, <DOMAIN_NAME>, OS_CONFIG_LIB_DOMAIN_NAME_LEN - 1);
    domain->enumerator.index = 0;

    // write initialized domain name to domainBackend
    OS_ConfigServiceBackend_writeRecord(
        &configLib->domainBackend,
        0,
        &domain,
        sizeof(domain));

    // initialize a parameter and set access rights for it
    OS_ConfigServiceLibTypes_Parameter_t parameter;
    OS_ConfigServiceAccessRights_SetAll(&parameter.readAccess);
    OS_ConfigServiceAccessRights_SetAll(&parameter.writeAccess);

    // initialize the parameter name
    strncpy(parameter.parameterName.name, <PARAMETER_NAME>, OS_CONFIG_LIB_PARAMETER_NAME_LEN - 1);

    // write an example parameter of type blob
    char largeBlob[2112];
    memcpy(largeBlob, <BLOB_VALUE>, sizeof(<BLOB_VALUE>));
    parameter.parameterType = OS_CONFIG_LIB_PARAMETER_TYPE_BLOB;
    parameter.parameterValue.valueBlob.index = 0;
    parameter.parameterValue.valueBlob.numberOfBlocks = 33;  // every block is 64byte, so this would allow to store 2112byte
    parameter.parameterValue.valueBlob.size = sizeof(<BLOB_VALUE>);
    parameter.domain.index = 0;
    OS_ConfigServiceBackend_writeRecord(
        &configLib->parameterBackend,
        <PARAMETER_INDEX>, // since it is the first parameter, it would be 0 in this case
        &parameter,
        sizeof(parameter));

    OS_ConfigService_writeVariableLengthBlob(
        &configLib->blobBackend,
        parameter.parameterValue.valueBlob.index,
        parameter.parameterValue.valueBlob.numberOfBlocks,
        largeBlob,
        sizeof(largeBlob));


### API Reference

    seos_err_t OS_ConfigService_createHandle(OS_ConfigServiceHandle_HandleKind_t handleKind, unsigned int id, out OS_ConfigServiceHandle_t handle);

    seos_err_t OS_ConfigService_domainEnumeratorInit(OS_ConfigServiceHandle_t handle, out OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator);
    seos_err_t OS_ConfigService_domainEnumeratorClose(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator);
    seos_err_t OS_ConfigService_domainEnumeratorReset(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator);
    seos_err_t OS_ConfigService_domainEnumeratorIncrement(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator);
    seos_err_t OS_ConfigService_domainEnumeratorGetElement(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_DomainEnumerator_t enumerator, out OS_ConfigServiceLibTypes_Domain_t domain);

    seos_err_t OS_ConfigService_parameterEnumeratorInit(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_DomainEnumerator_t domainEnumerator, out OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator);
    seos_err_t OS_ConfigService_parameterEnumeratorClose(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator);
    seos_err_t OS_ConfigService_parameterEnumeratorReset(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator);
    seos_err_t OS_ConfigService_parameterEnumeratorIncrement(OS_ConfigServiceHandle_t handle, inout OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator);
    seos_err_t OS_ConfigService_parameterEnumeratorGetElement(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, out OS_ConfigServiceLibTypes_Parameter_t parameter);

    void OS_ConfigService_domainGetName(refin OS_ConfigServiceLibTypes_Domain_t domain, out OS_ConfigServiceLibTypes_DomainName_t domainName);
    seos_err_t OS_ConfigService_domainCreateParameterEnumerator(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Domain_t domain, refin OS_ConfigServiceLibTypes_ParameterName_t parameterName, out OS_ConfigServiceLibTypes_ParameterEnumerator_t parameterEnumerator);
    seos_err_t OS_ConfigService_domainGetElement(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Domain_t domain, refin OS_ConfigServiceLibTypes_ParameterName_t parameterName, out OS_ConfigServiceLibTypes_Parameter_t parameter);
    void OS_ConfigService_parameterGetName(refin OS_ConfigServiceLibTypes_Parameter_t parameter, out OS_ConfigServiceLibTypes_ParameterName_t parameterName);
    void OS_ConfigService_parameterGetType(refin OS_ConfigServiceLibTypes_Parameter_t parameter, out OS_ConfigServiceLibTypes_ParameterType_t parameterType);
    size_t OS_ConfigService_parameterGetSize(refin OS_ConfigServiceLibTypes_Parameter_t parameter);

    seos_err_t OS_ConfigService_parameterGetValue(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, dataport_ptr_t buffer, size_t bufferLength, out size_t bytesCopied);
    seos_err_t OS_ConfigService_parameterGetValueAsU32(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, out uint32_t value);
    seos_err_t OS_ConfigService_parameterGetValueAsU64(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, out uint64_t value);
    seos_err_t OS_ConfigService_parameterGetValueAsString(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, dataport_ptr_t buffer, size_t bufferLength);
    seos_err_t OS_ConfigService_parameterGetValueAsBlob(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_Parameter_t parameter, dataport_ptr_t buffer, size_t bufferLength);

    seos_err_t OS_ConfigService_parameterSetValue(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, OS_ConfigServiceLibTypes_ParameterType_t parameterType, dataport_ptr_t buffer, size_t bufferLength);
    seos_err_t OS_ConfigService_parameterSetValueAsU32(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, uint32_t value);
    seos_err_t OS_ConfigService_parameterSetValueAsU64(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, uint64_t value);
    seos_err_t OS_ConfigService_parameterSetValueAsString(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, OS_ConfigServiceLibTypes_ParameterType_t parameterType, dataport_ptr_t buffer, size_t bufferLength);
    seos_err_t OS_ConfigService_parameterSetValueAsBlob(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_ParameterEnumerator_t enumerator, OS_ConfigServiceLibTypes_ParameterType_t parameterType, dataport_ptr_t buffer, size_t bufferLength);

    seos_err_t OS_ConfigService_parameterGetValueFromDomainName(OS_ConfigServiceHandle_t handle, refin OS_ConfigServiceLibTypes_DomainName_t domainName, refin OS_ConfigServiceLibTypes_ParameterName_t parameterName, OS_ConfigServiceLibTypes_ParameterType_t parameterType, dataport_ptr_t buffer, size_t bufferLength, out size_t bytesCopied);

### Tests
You can find an example test systems at:
* https://bitbucket.hensoldt-cyber.systems/projects/SS/repos/test_config_server_fs_backend/browse
* https://bitbucket.hensoldt-cyber.systems/projects/SS/repos/test_config_server/browse


### Further documentation
TBD

### License
TBD