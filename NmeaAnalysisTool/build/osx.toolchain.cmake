message(STATUS "#### Using OSX toolchain ####")

# Standard settings
set (UNIX True)
set (APPLE True)

find_program (GLOBAL_APP_CCACHE ccache)
find_program (GLOBAL_APP_CC clang)
find_program (GLOBAL_APP_CXX clang++)

# Force the compilers to clang for iOS
include (CMakeForceCompiler)
if ("GLOBAL_APP_CCACHE-NOTFOUND" STREQUAL "${GLOBAL_APP_CCACHE}")
    CMAKE_FORCE_C_COMPILER ("${GLOBAL_APP_CC}" Apple)
    CMAKE_FORCE_CXX_COMPILER ("${GLOBAL_APP_CXX}" Apple)
else ()
    set (CMAKE_C_COMPILER_ARG1 "${GLOBAL_APP_CC}")
    CMAKE_FORCE_C_COMPILER ("${GLOBAL_APP_CCACHE}" Apple)
    set (CMAKE_C_FLAGS_INIT "-Qunused-arguments -fcolor-diagnostics")

    set (CMAKE_CXX_COMPILER_ARG1 "${GLOBAL_APP_CXX}")
    CMAKE_FORCE_CXX_COMPILER ("${GLOBAL_APP_CCACHE}" Apple)
    set (CMAKE_CXX_FLAGS_INIT "-Qunused-arguments -fcolor-diagnostics")
endif ()

# Skip the platform compiler checks for cross compiling
set (CMAKE_CXX_COMPILER_WORKS TRUE)
set (CMAKE_C_COMPILER_WORKS TRUE)

# All iOS/Darwin specific settings - some may be redundant
set (CMAKE_SHARED_LIBRARY_PREFIX "lib")
set (CMAKE_SHARED_LIBRARY_SUFFIX ".dylib")
set (CMAKE_SHARED_MODULE_PREFIX "lib")
set (CMAKE_SHARED_MODULE_SUFFIX ".so")
set (CMAKE_MODULE_EXISTS 1)
set (CMAKE_DL_LIBS "")

SET(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} -D_FOR_APPLE_")
SET(CMAKE_CXX_FLAGS_INIT "${CMAKE_C_FLAGS_INIT}")
