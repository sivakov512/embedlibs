cmake_minimum_required(VERSION 3.16)

set(PUBLIC_HEADERS include)
set(SRCS
    src/ringbuf.c
    src/sugar.c
)

if(COMMAND idf_component_register)
    idf_component_register(
        SRCS ${SRCS}
        INCLUDE_DIRS ${PUBLIC_HEADERS}
        REQUIRES
            log
    )
else()
    project(embedlibs C)

    add_library(embedlibs STATIC ${SRCS})
    target_include_directories(embedlibs PUBLIC ${PUBLIC_HEADERS})
endif()
