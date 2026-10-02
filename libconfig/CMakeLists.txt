add_library(libconfig STATIC
    src/lexer.c
)

target_include_directories(libconfig
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(libconfig_internal INTERFACE)

target_include_directories(libconfig_internal
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)