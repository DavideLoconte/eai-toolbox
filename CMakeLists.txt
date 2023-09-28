cmake_minimum_required(VERSION 3.20)
project(eai_toolbox C)

set(CMAKE_C_STANDARD 11)

set(EAI_TOOLBOX_VERSION_MAJOR 0)
set(EAI_TOOLBOX_VERSION_MINOR 1)
set(EAI_TOOLBOX_VERSION_PATCH 1)
set(EAI_TOOLBOX_VERSION "${EAI_TOOLBOX_VERSION_MAJOR}.${EAI_TOOLBOX_VERSION_MINOR}.${EAI_TOOLBOX_VERSION_PATCH}")

set(EAI_TOOLBOX_AUTHOR "Davide Loconte")
set(EAI_TOOLBOX_COPYRIGHT_YEAR "2022-2023")

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if (result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Dependencies (ulib)

if (NOT TARGET ulib)
    include(FetchContent)
    message("-- Fetching ulib")
    FetchContent_Declare(
            ulib
            GIT_REPOSITORY https://github.com/IvanoBilenchi/ulib.git
            GIT_TAG 51883d97292e3db09518d8f75add89a2debfcbfb
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
    set(EAI_TOOLBOX_COMPILE_OPTS /W3)
else ()
    set(EAI_TOOLBOX_COMPILE_OPTS -Wall -Wextra -Wformat=2)
    set(EAI_TOOLBOX_ASAN_COMPILE_OPTS -fsanitize=address -fno-omit-frame-pointer -fno-common -g -O0)
endif ()

# TARGETS

# Library
add_library(eai-toolbox ${EAI_TOOLBOX_SOURCES})
target_link_libraries(eai-toolbox PUBLIC ulib)
target_include_directories(eai-toolbox PUBLIC ${EAI_TOOLBOX_HEADERS_DIR})
target_compile_options(eai-toolbox PUBLIC ${EAI_TOOLBOX_COMPILE_OPTS})

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

