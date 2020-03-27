# TRENTOS Configuration


## Overview

Provides the TRENTOS Configuration API that allows to write and retrieve configuration parameters either by using it locally in
a component or as a separate remote server component.
Note however that if the TRENTOS Configuration library is meant to be built as a CAmkES component, an additional
CAmkES interface file and a CAmkES component file will have to be created, configured and integrated into the envisaged system.


### Dependencies

* PartitionManager
* FileSystem


### Build options

The TRENTOS Configuration can be integrated in three different ways, for which you will partially have to set specific flags:

| Build Option        | Flag                       | Header to include in component code |
|:--------------------|:--------------------------:|:-----------------------------------:|
| As a Library        | -                          | "seos_config_library.h"             |
| As a CAmkES Server  | SEOS_CONFIG_CAMKES_SERVER  | "seos_config_server.h"              |
| As a CAmkES Client  | SEOS_CONFIG_CAMKES_CLIENT  | "seos_config_client.h"              |

If one of the first two options from the ones presented above are chosen, it is also necessary to take care of the following two steps:
* include **seos_configuration** lib
* choose a backend option to which the configuration parameters will be written to. Options here are either setting a filesystem backend with **CONFIG_SERVER_BACKEND_FILESYSTEM** or choosing a memory backend with **CONFIG_SERVER_BACKEND_MEMORY**.

If the component is built using a filesystem backend, it is recommended to take a look also at the documentation of the TRENTOS FileSystemCore and
PartitionManager libraries.
Please also note the usage of the **seos_core_api** lib in the next sample code segment. Along with other things, this also brings in the CAmkES
interface file needed to build a client/server component setup. This will be explained further in the next section.
A possible CMakeList configuration for a CAmkES ConfigServer component with a filesystem backend would result in:

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
            -DSEOS_FS_BUILD_AS_LIB_BASIC_HANDLE
            # partition manager
            -DSEOS_PARTITION_MANAGER_BUILD_AS_COMPONENT # uses the PartitionManager as a component
            # config server
            -DCONFIG_SERVER_BACKEND_FILESYSTEM # indicates the use of a filesystem backend
            -DSEOS_CONFIG_CAMKES_SERVER     # indicates a CAmkES configuration server is being built
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
            -DSEOS_CONFIG_CAMKES_CLIENT
        LIBS
            seos_system_config
            seos_core_api
            seos_configuration
    )

#### Integrating the CAmkES interface file when building a server component:
As mentioned above, the TRENTOS SDK already provides the necessary files (**seos_core_api**) to set up an interface for the ConfigServer built as a CAmkES component. Simply import the **<seos_if_configuration.camkes>** interface in the CAmkES component file of the ConfigServer component and make sure to name the procedure **configuration_server_interface** and also define a dataport for the clients to connect to as seen in this example:

    import <seos_if_configuration.camkes>;

    component ConfigServer {

        provides configuration_server_interface server;

        //-------------------------------------------------
        // dataports for clients
        dataport Buf cfg_dataport_buf;
    }

A client would then connect to this server by also integrating the interface and setting a dataport:

    import <seos_if_configuration.camkes>;

    component Client {

        control;

        uses configuration_server_interface server;

        //-------------------------------------------------
        // ConfigServer
        dataport Buf cfg_dataport_buf;
    }


#### Initialization

When initializing the Configuration lib with a filesystem backend, you will also have to first initialize a partition object with the partition ID you
want to write to. The init process of the this can be found in the documentation of the PartitionManager lib and won't be covered in the following example.

    #include "seos_config_server.h"   // use "seos_config_library.h" if you are using the library build option

If you are building the component using the functions offered as a library, you would call library_seos_configuration_getInstance() but the other following
steps are the same:

    SeosConfigInstanceStore* serverInstanceStore =
        server_seos_configuration_getInstances();

    SeosConfigLib* configLib =
        seos_configuration_instance_store_getInstance(serverInstanceStore, 0);


If your partition already contains a complete configuration file, then these are the steps to follow to initialize this file and be able to write and retrieve
configuration parameters from it. Beware however that the defined filenames in the example need to be the same ones that were used to create the config file,
otherwise the init process will not be able to parse the file contents.

    #define PARAMETER_FILE "PARAM.BIN"
    #define DOMAIN_FILE "DOMAIN.BIN"
    #define STRING_FILE "STRING.BIN"
    #define BLOB_FILE "BLOB.BIN"

    hPartition_t phandle;
    SeosConfigBackend parameterBackend;
    SeosConfigBackend domainBackend;
    SeosConfigBackend stringBackend;
    SeosConfigBackend blobBackend;
    SeosConfigBackend_FileName name;

    // small helper function to set the filename
    static
    void initializeName(char* buf, size_t bufSize, char const* name)
    {
        memset(buf, 0, bufSize);
        strncpy(buf, name, bufSize - 1);
    }

    // domain backend
    initializeName(name.buffer, SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, DOMAIN_FILE);
    SeosConfigBackend_initializeFileBackend(&domainBackend, name, phandle);

    // parameter backend
    initializeName(name.buffer, SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, PARAMETER_FILE);
    SeosConfigBackend_initializeFileBackend(&parameterBackend, name, phandle);

    // string parameter backend
    initializeName(name.buffer, SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, STRING_FILE);
    SeosConfigBackend_initializeFileBackend(&stringBackend, name, phandle);

    // blob parameter backend
    initializeName(name.buffer, SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, BLOB_FILE);
    SeosConfigBackend_initializeFileBackend(&blobBackend, name, phandle);


Initializing the Configuration lib with a memory backend does not have any dependencies to the PartitionManager and so you can skip the init process
of this additional layer since you will be writing the parameters directly into memory of the component integrating this library. The init process
is very similar to the one described above for the filesystem but once you start initializing the different sections, you will be calling the following
functions:

    // domain backend
    SeosConfigBackend_initializeMemBackend(&domainBackend, domainBuf, sizeof(domainBuf));

    // parameter backend
    SeosConfigBackend_initializeMemBackend(&parameterBackend, parameterBuf, sizeof(parameterBuf));

    //...


Finally initialize the backend in the config library instance with the different parameter types:

    SeosConfigLib_Init(configLib, &parameterBackend, &domainBackend, &stringBackend, &blobBackend);

The described initialization process refers to components building the TRENTOS Configuration as a library or a server. The initialization for components
acting as clients and connecting to a remote ConfigServer component only require:

    #include "seos_config.h"

    SeosConfigHandle remoteHandle;

    //Open remote handle of the ConfigServer API
    seos_configuration_createHandle(SEOS_CONFIG_HANDLE_KIND_RPC, <HANDLE_ID>, &remoteHandle);

If the ConfigServer is run locally on the client CAmkES component, it is also possible to create a local handle:

    SeosConfigHandle localHandle;

    //Open a local handle of the ConfigServer API
    seos_configuration_createHandle(SEOS_CONFIG_HANDLE_KIND_LOCAL, <HANDLE_ID>, &localHandle);


#### Setting up a Configuration backend

As mentioned in the previous section, the initialization requires an already created configuration file or a memory backend that was already provisioned
to the desired set of parameters to read and write from. This section therefore provides some code examples to set up a configuration backend.
Depending on which parameter backend types are used, the following function has to be called to create a filesystem backend:

    // create the desired number of domains
    initializeName(name.buffer, SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, DOMAIN_FILE);
    SeosConfigBackend_createFileBackend(name, phandle, <NUMBER_OF_DOMAINS>, sizeof(SeosConfigLib_Domain));

    // create the desired number of parameters
    initializeName(name.buffer, SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, PARAMETER_FILE);
    SeosConfigBackend_createFileBackend(name, phandle, <NUMBER_OF_PARAMETERS>, sizeof(SeosConfigLib_Parameter));

    // create the desired number of string parameters
    initializeName(name.buffer, SEOS_CONFIG_BACKEND_MAX_FILE_NAME_LEN, STRING_FILE);
    SeosConfigBackend_createFileBackend(name, phandle, <NUMBER_OF_STRINGS>, SEOS_CONFIG_LIB_PARAMETER_MAX_STRING_LENGTH);

    //...

The same goes for a memory backend:

    // sizes of the buffers are defined to at least cover the total size of the
    // parameters added to the backend
    static char parameterBuf[12000];
    static char domainBuf[2000];

    // create the desired number of domains
    SeosConfigBackend_createMemBackend(domainBuf, sizeof(domainBuf), <NUMBER_OF_DOMAINS>, sizeof(SeosConfigLib_Domain));

    // create the desired number of parameters
    SeosConfigBackend_createMemBackend(parameterBuf, sizeof(parameterBuf), <NUMBER_OF_PARAMETERS>, sizeof(SeosConfigLib_Parameter));

    //...

After creating this backend frame and initializing it you can write parameter and domain value to it as seen in this example:

    // initialize Domain
    SeosConfigLib_Domain domain;
    strncpy(domain->name.name, <DOMAIN_NAME>, SEOS_CONFIG_LIB_DOMAIN_NAME_LEN - 1);
    domain->enumerator.index = 0;

    // write initialized domain name to domainBackend
    SeosConfigBackend_writeRecord(&configLib->domainBackend, 0, &domain, sizeof(domain));

    // initialize a parameter and set access rights for it
    SeosConfigLib_Parameter parameter;
    SeosConfigAccessRights_SetAll(&parameter.readAccess);
    SeosConfigAccessRights_SetAll(&parameter.writeAccess);

    // initialize the parameter name
    strncpy(parameter.parameterName.name, <PARAMETER_NAME>, SEOS_CONFIG_LIB_PARAMETER_NAME_LEN - 1);

    // write an example parameter of type blob
    char largeBlob[2112];
    memcpy(largeBlob, <BLOB_VALUE>, sizeof(<BLOB_VALUE>));
    parameter.parameterType = SEOS_CONFIG_LIB_PARAMETER_TYPE_BLOB;
    parameter.parameterValue.valueBlob.index = 0;
    parameter.parameterValue.valueBlob.numberOfBlocks = 33;  // every block is 64byte, so this would allow to store 2112byte
    parameter.parameterValue.valueBlob.size = sizeof(<BLOB_VALUE>);
    parameter.domain.index = 0;
    SeosConfigBackend_writeRecord(&configLib->parameterBackend,
                                  <PARAMETER_INDEX>, // since it is the first parameter, it would be 0 in this case
                                  &parameter,
                                  sizeof(parameter));

    SeosConfigLib_writeVariableLengthBlob(&configLib->blobBackend,
                                          parameter.parameterValue.valueBlob.index,
                                          parameter.parameterValue.valueBlob.numberOfBlocks,
                                          largeBlob,
                                          sizeof(largeBlob));


### API Reference

    seos_err_t seos_configuration_createHandle(SeosConfigLib_HandleKind handleKind, unsigned int id, out SeosConfigHandle handle);

    seos_err_t seos_configuration_domainEnumeratorInit(SeosConfigHandle handle, out SeosConfigLib_DomainEnumerator enumerator);
    seos_err_t seos_configuration_domainEnumeratorClose(SeosConfigHandle handle, inout SeosConfigLib_DomainEnumerator enumerator);
    seos_err_t seos_configuration_domainEnumeratorReset(SeosConfigHandle handle, inout SeosConfigLib_DomainEnumerator enumerator);
    seos_err_t seos_configuration_domainEnumeratorIncrement(SeosConfigHandle handle, inout SeosConfigLib_DomainEnumerator enumerator);
    seos_err_t seos_configuration_domainEnumeratorGetElement(SeosConfigHandle handle, refin SeosConfigLib_DomainEnumerator enumerator, out SeosConfigLib_Domain domain);

    seos_err_t seos_configuration_parameterEnumeratorInit(SeosConfigHandle handle, refin SeosConfigLib_DomainEnumerator domainEnumerator, out SeosConfigLib_ParameterEnumerator enumerator);
    seos_err_t seos_configuration_parameterEnumeratorClose(SeosConfigHandle handle, inout SeosConfigLib_ParameterEnumerator enumerator);
    seos_err_t seos_configuration_parameterEnumeratorReset(SeosConfigHandle handle, inout SeosConfigLib_ParameterEnumerator enumerator);
    seos_err_t seos_configuration_parameterEnumeratorIncrement(SeosConfigHandle handle, inout SeosConfigLib_ParameterEnumerator enumerator);
    seos_err_t seos_configuration_parameterEnumeratorGetElement(SeosConfigHandle handle, refin SeosConfigLib_ParameterEnumerator enumerator, out SeosConfigLib_Parameter parameter);

    void seos_configuration_domainGetName(refin SeosConfigLib_Domain domain, out SeosConfigLib_DomainName domainName);
    seos_err_t seos_configuration_domainCreateParameterEnumerator(SeosConfigHandle handle, refin SeosConfigLib_Domain domain, refin SeosConfigLib_ParameterName parameterName, out SeosConfigLib_ParameterEnumerator parameterEnumerator);
    seos_err_t seos_configuration_domainGetElement(SeosConfigHandle handle, refin SeosConfigLib_Domain domain, refin SeosConfigLib_ParameterName parameterName, out SeosConfigLib_Parameter parameter);
    void seos_configuration_parameterGetName(refin SeosConfigLib_Parameter parameter, out SeosConfigLib_ParameterName parameterName);
    void seos_configuration_parameterGetType(refin SeosConfigLib_Parameter parameter, out SeosConfigLib_ParameterType parameterType);
    size_t seos_configuration_parameterGetSize(refin SeosConfigLib_Parameter parameter);

    seos_err_t seos_configuration_parameterGetValue(SeosConfigHandle handle, refin SeosConfigLib_Parameter parameter, dataport_ptr_t buffer, size_t bufferLength, out size_t bytesCopied);
    seos_err_t seos_configuration_parameterGetValueAsU32(SeosConfigHandle handle, refin SeosConfigLib_Parameter parameter, out uint32_t value);
    seos_err_t seos_configuration_parameterGetValueAsU64(SeosConfigHandle handle, refin SeosConfigLib_Parameter parameter, out uint64_t value);
    seos_err_t seos_configuration_parameterGetValueAsString(SeosConfigHandle handle, refin SeosConfigLib_Parameter parameter, dataport_ptr_t buffer, size_t bufferLength);
    seos_err_t seos_configuration_parameterGetValueAsBlob(SeosConfigHandle handle, refin SeosConfigLib_Parameter parameter, dataport_ptr_t buffer, size_t bufferLength);

    seos_err_t seos_configuration_parameterSetValue(SeosConfigHandle handle, refin SeosConfigLib_ParameterEnumerator enumerator, SeosConfigLib_ParameterType parameterType, dataport_ptr_t buffer, size_t bufferLength);
    seos_err_t seos_configuration_parameterSetValueAsU32(SeosConfigHandle handle, refin SeosConfigLib_ParameterEnumerator enumerator, uint32_t value);
    seos_err_t seos_configuration_parameterSetValueAsU64(SeosConfigHandle handle, refin SeosConfigLib_ParameterEnumerator enumerator, uint64_t value);
    seos_err_t seos_configuration_parameterSetValueAsString(SeosConfigHandle handle, refin SeosConfigLib_ParameterEnumerator enumerator, SeosConfigLib_ParameterType parameterType, dataport_ptr_t buffer, size_t bufferLength);
    seos_err_t seos_configuration_parameterSetValueAsBlob(SeosConfigHandle handle, refin SeosConfigLib_ParameterEnumerator enumerator, SeosConfigLib_ParameterType parameterType, dataport_ptr_t buffer, size_t bufferLength);

    seos_err_t seos_configuration_parameterGetValueFromDomainName(SeosConfigHandle handle, refin SeosConfigLib_DomainName domainName, refin SeosConfigLib_ParameterName parameterName, SeosConfigLib_ParameterType parameterType, dataport_ptr_t buffer, size_t bufferLength, out size_t bytesCopied);

### Tests
You can find an example test systems at:
* https://bitbucket.hensoldt-cyber.systems/projects/SS/repos/test_config_server_fs_backend/browse
* https://bitbucket.hensoldt-cyber.systems/projects/SS/repos/test_config_server/browse


### Further documentation
TBD

### License
TBD