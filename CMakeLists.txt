cmake_minimum_required(VERSION 3.13.0)

# enable new behavior introduced in CMake 3.13, where relative paths will be
# converted into absolute paths in target_sources(INTERFACE ....)
cmake_policy(SET CMP0076 NEW)

#------------------------------------------------------------------------------

project(seos_configuration_api C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)

#------------------------------------------------------------------------------

project(seos_configuration C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)

target_sources(${PROJECT_NAME}
    INTERFACE
        src/SeosConfigAccessRights.c
        src/SeosConfigAppIdentifier.c
        src/SeosConfigBackend.c
        src/SeosConfigLib.c
        src/seos_config_client.c
        src/seos_config_library.c
        src/seos_config_server.c
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        seos_libs
)

#------------------------------------------------------------------------------
