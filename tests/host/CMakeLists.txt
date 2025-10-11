cmake_minimum_required(VERSION 3.16)

set(LIBNAME embedlibs)
set(TESTLIBNAME embedlibs_tests)

project(${TESTLIBNAME} C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(AddUnityTest)

include(FetchContent)

FetchContent_Declare(
    Unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.1
)
FetchContent_MakeAvailable(Unity)

if(NOT TARGET unity)
    add_library(unity STATIC ${Unity_SOURCE_DIR}/src/unity.c)
    target_include_directories(unity PUBLIC ${Unity_SOURCE_DIR}/src)
endif()

enable_testing()

add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/../../ ${CMAKE_BINARY_DIR}/${LIBNAME})

add_unity_test(${TESTLIBNAME}
    SRCS
        test_hash.c
    LIBS ${LIBNAME}
)
