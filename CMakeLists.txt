cmake_minimum_required(VERSION 3.20)

project(libconfig_test
    VERSION 1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

function(enable_warnings target)
    if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Werror
        )
    elseif (MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /WX
        )
    endif()
endfunction()

add_subdirectory(libconfig)

# Manuelle Tests / Experimente
add_executable(libconfig_manual
    tests/manual/main.c
)

target_link_libraries(libconfig_manual
    PRIVATE
        libconfig
        libconfig_internal
)

# Automatische Tests
add_executable(libconfig_tests
    tests/automatic/main.c
	tests/automatic/test_lexer.c
)

target_link_libraries(libconfig_tests
    PRIVATE
        libconfig
        libconfig_internal
)

enable_warnings(libconfig)
enable_warnings(libconfig_manual)
enable_warnings(libconfig_tests)