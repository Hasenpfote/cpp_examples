cmake_minimum_required(VERSION 3.1)

project(cpp_examples)

### Debug
#message("CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
#message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
#message("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
#message("CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

### Additional cmake files.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
#message("CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

### Check if supported platforms and compilers.
if(WIN32)
    if(MSVC)
    elseif()
        message(FATAL_ERROR "You are using an unsupported Windows compiler!")
    endif()
elseif(UNIX)
    option(FORCE_32BIT_BUILD "Force a 32bit compile on 64bit" OFF)
else()
    message(FATAL_ERROR "You are on an unsupported platform!")
endif()

###
option(BUILD_TESTS "Set to ON to build tests" OFF)

### External
#add_subdirectory(external)

### googletest
if(${BUILD_TESTS})
    if(CMAKE_VERSION VERSION_LESS 3.2)
        set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
    else()
        set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
    endif()

    include(DownloadProject/DownloadProject)
    download_project(PROJ                googletest
                     GIT_REPOSITORY      https://github.com/google/googletest.git
                     GIT_TAG             master
                     ${UPDATE_DISCONNECTED_IF_AVAILABLE}
    )
    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

### Examples
set(examples
    "00_example"
    "circular_buffer"
    # Add a new example here.
)

foreach(example IN LISTS examples)
    message(STATUS "example: ${example}")
    add_subdirectory(${example})
endforeach()