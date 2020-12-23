#
# OS Configuration library
#
# Copyright (C) 2019-2020, Hensoldt Cyber GmbH
#

cmake_minimum_required(VERSION 3.13.0)

# enable new behavior introduced in CMake 3.13, where relative paths will be
# converted into absolute paths in target_sources(INTERFACE ....)
cmake_policy(SET CMP0076 NEW)

#------------------------------------------------------------------------------

project(OS_ConfigService_api C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)

#------------------------------------------------------------------------------

project(os_configuration C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        include
)

target_sources(${PROJECT_NAME}
    INTERFACE
        src/OS_ConfigServiceAccessRights.c
        src/OS_ConfigServiceAppIdentifier.c
        src/OS_ConfigServiceBackend.c
        src/OS_ConfigService.c
        src/OS_ConfigServiceHandle.c
        src/OS_ConfigServiceLib.c
        src/OS_ConfigServiceLibrary.c
        src/OS_ConfigServiceServer.c
)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        lib_debug
)
