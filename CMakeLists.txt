cmake_minimum_required(VERSION 3.20)
project(eai_toolbox C)

set(CMAKE_C_STANDARD 11)

set(EAI_TOOLBOX_VERSION_MAJOR 0)
set(EAI_TOOLBOX_VERSION_MINOR 2)
set(EAI_TOOLBOX_VERSION_PATCH 1)
set(EAI_TOOLBOX_VERSION "${EAI_TOOLBOX_VERSION_MAJOR}.${EAI_TOOLBOX_VERSION_MINOR}.${EAI_TOOLBOX_VERSION_PATCH}")

set(EAI_TOOLBOX_AUTHOR "Davide Loconte")
set(EAI_TOOLBOX_COPYRIGHT_YEAR "2022-2024")

option(EAI_TOOLBOX_LTO "Enable link-time optimization, if available" ON)
option(EAI_TOOLBOX_MATH "Use standard math library in eai-toolbox" ON)

if(EAI_TOOLBOX_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT EAI_TOOLBOX_LTO_ENABLED OUTPUT REASON)
    if(NOT EAI_TOOLBOX_LTO_ENABLED)
        message("-- LTO disabled for eai-toolbox. Reason: ${REASON}")
    else()
        message("-- LTO enabled for eai-toolbox")
    endif()
else()
    message("-- LTO disabled for eai-toolbox. Reason: Disabled by user")
endif()

message( "-- EAI Toolbox Build type: ${CMAKE_BUILD_TYPE}")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Dependencies (ulib)
if (NOT TARGET ulib)

    if (EAI_TOOLBOX_LTO_ENABLED)
        set(ULIB_LTO ON CACHE BOOL "" FORCE)
    endif()

    include(FetchContent)
    message("-- Fetching ulib for eai-toolbox")
    FetchContent_Declare(
            ulib
            GIT_REPOSITORY https://github.com/IvanoBilenchi/ulib.git
            GIT_TAG c89c7b8e356bbedff78a08ef0da27275cffcce35
    )
    FetchContent_MakeAvailable(ulib)
    set(ULIB_HEADERS_DIR ${ULIB_PUBLIC_HEADERS_DIR})

endif()

# eai-toolbox
set(EAI_TOOLBOX_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(EAI_TOOLBOX_HEADERS_DIR "${CMAKE_CURRENT_LIST_DIR}/include" ${ULIB_HEADERS_DIR})
set(EAI_TOOLBOX_TEST_DIR "${CMAKE_CURRENT_LIST_DIR}/test")

# sources
file(GLOB EAI_TOOLBOX_SOURCES "${EAI_TOOLBOX_SOURCE_DIR}/*.c")
file(GLOB EAI_TOOLBOX_TEST_SOURCES ${EAI_TOOLBOX_TEST_DIR}/*.c ${EAI_TOOLBOX_TEST_DIR}/tests/*.c)
file(GLOB EAI_TOOLBOX_CLI_SOURCES "${EAI_TOOLBOX_CLI_DIR}/*.c")

# compile opts
if (MSVC)
    set(EAI_TOOLBOX_COMPILE_OPTS /W4)
    set(EAI_TOOLBOX_LIBS ulib)
else ()
    set(EAI_TOOLBOX_COMPILE_OPTS -Wall -Wextra)
    set(EAI_TOOLBOX_ASAN_COMPILE_OPTS -fsanitize=address -fno-omit-frame-pointer -fno-common -g -O0)
    if (EAI_TOOLBOX_MATH)
        set(EAI_TOOLBOX_LIBS ulib m)
        message("-- Linking math library into eai-toolbox")
    else()
        set(EAI_TOOLBOX_LIBS ulib)
        message("-- Using built-in eai-toolbox math functions")
    endif()
endif ()

# TARGETS

# Library
add_library(eai-toolbox ${EAI_TOOLBOX_SOURCES})
target_link_libraries(eai-toolbox PUBLIC ${EAI_TOOLBOX_LIBS})
target_include_directories(eai-toolbox PUBLIC ${EAI_TOOLBOX_HEADERS_DIR})
target_compile_options(eai-toolbox PUBLIC ${EAI_TOOLBOX_COMPILE_OPTS})

if (EAI_TOOLBOX_MATH)
    target_compile_definitions(eai-toolbox PUBLIC EAI_TOOLBOX_MATH=1)
else ()
    target_compile_definitions(eai-toolbox PUBLIC EAI_TOOLBOX_MATH=0)
endif ()

# Headers

# Test executable
add_executable(eai-toolbox-test ${EAI_TOOLBOX_TEST_SOURCES})
target_include_directories(eai-toolbox-test PUBLIC ${EAI_TOOLBOX_HEADERS_DIR})
target_link_libraries(eai-toolbox-test PUBLIC eai-toolbox)
target_compile_options(eai-toolbox-test PUBLIC ${EAI_TOOLBOX_COMPILE_OPTS})
set_target_properties(eai-toolbox-test PROPERTIES EXCLUDE_FROM_ALL True)

# Asan test
if (NOT MSVC)
    add_executable(eai-toolbox-test-asan ${EAI_TOOLBOX_TEST_SOURCES})
    set_target_properties(eai-toolbox-test-asan PROPERTIES EXCLUDE_FROM_ALL True)
    target_include_directories(eai-toolbox-test-asan PUBLIC ${EAI_TOOLBOX_HEADERS_DIR})
    target_link_libraries(eai-toolbox-test-asan PUBLIC eai-toolbox)
    target_compile_options(eai-toolbox-test-asan PRIVATE ${EAI_TOOLBOX_ASAN_COMPILE_OPTS})
    set_target_properties(eai-toolbox-test-asan PROPERTIES EXCLUDE_FROM_ALL True)
    target_link_options(eai-toolbox-test-asan PRIVATE -fsanitize=address)
endif()

# IPO
if (EAI_TOOLBOX_LTO)
    set_property(TARGET eai-toolbox PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET eai-toolbox-test PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()
