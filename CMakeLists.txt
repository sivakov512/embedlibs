cmake_minimum_required(VERSION 3.16)

set(PUBLIC_HEADERS ${CMAKE_CURRENT_LIST_DIR}/include)
set(ESP_PUBLIC_HEADERS ${CMAKE_CURRENT_LIST_DIR}/include_esp)
set(SRCS src/ringbuf.c)

if(COMMAND idf_component_register)
    idf_component_register(
        SRCS ${SRCS}
        INCLUDE_DIRS ${PUBLIC_HEADERS} ${ESP_PUBLIC_HEADERS}
    )
else()
    project(embedlibs C)

    add_library(embedlibs STATIC ${SRCS})
    target_include_directories(embedlibs PUBLIC ${PUBLIC_HEADERS})
endif()
