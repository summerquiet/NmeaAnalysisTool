message(STATUS "#### Automatically choosing toolchain ####")

#
# Try use clang & ccache if found.
#
find_program (GLOBAL_APP_CLANG clang)
find_program (GLOBAL_APP_CLANGXX clang++)
if (NOT "GLOBAL_APP_CLANG-NOTFOUND" STREQUAL "${GLOBAL_APP_CLANG}"
    AND (NOT "GLOBAL_APP_CLANGXX-NOTFOUND" STREQUAL "${GLOBAL_APP_CLANGXX}"))
    find_program (GLOBAL_APP_CCACHE ccache)
    if ("GLOBAL_APP_CCACHE-NOTFOUND" STREQUAL "${GLOBAL_APP_CCACHE}")
        set (CMAKE_C_COMPILER "${GLOBAL_APP_CLANG}")
        set (CMAKE_CXX_COMPILER "${GLOBAL_APP_CLANGXX}")
    else ()
        set (CMAKE_C_COMPILER_ARG1 "${GLOBAL_APP_CLANG}")
        set (CMAKE_C_FLAGS_INIT "-Qunused-arguments -fcolor-diagnostics" CACHE INTERNAL "Workaround for ccache & clang combination")
        set (CMAKE_C_COMPILER "${GLOBAL_APP_CCACHE}")

        set (CMAKE_CXX_COMPILER_ARG1 "${GLOBAL_APP_CLANGXX}")
        set (CMAKE_CXX_FLAGS_INIT "-Qunused-arguments -fcolor-diagnostics" CACHE INTERNAL "Workaround for ccache & clang combination")
        set (CMAKE_CXX_COMPILER "${GLOBAL_APP_CCACHE}")
    endif ()
endif ()
