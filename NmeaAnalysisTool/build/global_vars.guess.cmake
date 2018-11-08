# Purpose:
#   Make it possible to generate Makefile by cmake directly without various
#   settings in script. Some environment variables may affect the default
#   behavior. The following variables will be regarded in the order as they
#   represent below:
#
#       1. if CMAKE_C_COMPILER/CMAKE_CXX_COMPILER is set in command line,
#          they will be regarded (we'll try to automatically detect if any
#          iAuto toolchain is specified in such case).
#       2. if 'ANDROID_NDK' is set (must be set to path of the Android NDK
#          directory), try to build navi for Android.
#       3. if 'XCODE_PATH' is set (must be set to path returned by
#          'xcode-select -print-path'), try to build navi for iOS (OSX only).
#       4. try detect if any iAuto build environment has been lunched, if
#          yes, try use it.
#       5. try to build navi runs natively on your host OS.
#

#=======================================================================
# Guess path of source tree
if (NOT DEFINED USER_CONF_PROJECT_ROOT_DIR)
    # Root directory of the source tree
    get_filename_component (USER_CONF_PROJECT_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
endif ()
if (NOT DEFINED USER_CONF_LIB_SRC_DIR)
    # Root directory of the navi source tree
    get_filename_component (USER_CONF_LIB_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/../src" ABSOLUTE)
endif ()

if (WIN32)
    # for compatibility issue on windows
    execute_process(
        COMMAND cmd /C "echo %CD%"
        OUTPUT_VARIABLE PRIVATE_PWD
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    set (ENV{PWD} ${PRIVATE_PWD})
endif ()

#=======================================================================
# Guess toolchain to be used.
if ((DEFINED CMAKE_C_COMPILER) OR (DEFINED CMAKE_CXX_COMPILER))
    # Use user supplied compiler
    if ((DEFINED ENV{NUTSHELL_PRODUCT_OUT}) AND (NOT "$ENV{NUTSHELL_PRODUCT_OUT}" STREQUAL "")
        AND (DEFINED ENV{NUTSHELL_TOOLCHAIN}) AND (NOT "$ENV{NUTSHELL_TOOLCHAIN}" STREQUAL ""))
        set (NUTSHELL True)
    endif ()
elseif ((DEFINED ENV{ANDROID_NDK}) AND (NOT "$ENV{ANDROID_NDK}" STREQUAL ""))
    # Build for Android if ANDROID_NDK is supplied
    file (GLOB _PRIVATE_TOOLCHAIN_LIST
        RELATIVE "$ENV{ANDROID_NDK}/toolchains"
        "$ENV{ANDROID_NDK}/toolchains/*")
    list (LENGTH _PRIVATE_TOOLCHAIN_LIST _PRIVATE_LIST_LENGTH)
    if ((_PRIVATE_LIST_LENGTH GREATER 0) AND (NOT DEFINED ANDROID_TOOLCHAIN_NAME))
        LIST(GET _PRIVATE_TOOLCHAIN_LIST 0 ANDROID_TOOLCHAIN_NAME)
    endif ()
    set (CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/android.toolchain.cmake")
elseif (APPLE)
    # Build for iOS on OSX if XCODE_PATH is supplied
    if ((DEFINED ENV{XCODE_PATH}) AND (NOT "$ENV{XCODE_PATH}" STREQUAL ""))
        set (XCODE_PATH "$ENV{XCODE_PATH}")
        set (PRIVATE_IOS_PLATFORM "OS")
        set (CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/iOS.toolchain.cmake")
    endif ()
elseif ((DEFINED ENV{NUTSHELL_PRODUCT_OUT}) AND (NOT "$ENV{NUTSHELL_PRODUCT_OUT}" STREQUAL "")
        AND (DEFINED ENV{NUTSHELL_TOOLCHAIN}) AND (NOT "$ENV{NUTSHELL_TOOLCHAIN}" STREQUAL ""))
    set (CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/nutshell.toolchain.cmake")
endif ()
if ((NOT DEFINED CMAKE_C_COMPILER) AND (NOT DEFINED CMAKE_TOOLCHAIN_FILE))
    if (APPLE)
        # there's no known working iAuto toolchain on OSX
        set (CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/osx.toolchain.cmake")
    else ()
        set (CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/auto.toolchain.cmake")
    endif ()
endif ()

enable_language (C)
enable_language (CXX)

#=======================================================================
# NUTSHELL:
#   - 1: building for iAuto
#   - (other): not building for iAuto
#
# NOTE: give it another chance to detect if we're building for iAuto.
#
if (NOT DEFINED NUTSHELL)
    include (CheckSymbolExists)
    CHECK_SYMBOL_EXISTS("HAVE_NUTSHELL_OS" "" NUTSHELL)
endif (NOT DEFINED NUTSHELL)

#=======================================================================
# USER_CONF_TARGET_PRODUCT:
#   - android: antsupporter runs on Android
#   - iOS: antsupporter runs on iOS
#   - mac: antsupporter runs on mac
#   - (other): (to be added)
#   - (default): antsupporter runs on linux
#
if (NOT DEFINED USER_CONF_TARGET_PRODUCT)
    if (ANDROID)
        set (USER_CONF_TARGET_PRODUCT android)
    elseif (IOS)
        set (USER_CONF_TARGET_PRODUCT iOS)
    elseif (APPLE)
        set (USER_CONF_TARGET_PRODUCT mac)
    else ()
        set (USER_CONF_TARGET_PRODUCT linux)
    endif ()
endif ()

#=======================================================================
# USER_CONF_TARGET_DEVICE:
#    - smartphone: Navigation runs on smartphone (Android, iOS, etc.)
#    - (other): (to be added)
#
if (NOT DEFINED USER_CONF_TARGET_DEVICE)
    if (ANDROID)
        set (USER_CONF_TARGET_DEVICE smartphone)
    elseif (IOS)
        set (USER_CONF_TARGET_DEVICE smartphone)
    else ()
        set (USER_CONF_TARGET_DEVICE simulator)
    endif ()
endif ()

#=======================================================================
# USER_CONF_TARGET_BOARD_TYPE:
#    - PC:
#
if (NOT DEFINED USER_CONF_TARGET_BOARD_TYPE)
    set (USER_CONF_TARGET_BOARD_TYPE "PC")
endif ()

#=======================================================================
# USER_CONF_TARGET_ARCH:
#    - arm: target CPU architecture is arm
#    - x86: target CPU architecture is x86
#    - x86_64: target CPU architecture is x86_64
#    - (other): (to be added)
#
if (NOT DEFINED USER_CONF_TARGET_ARCH)
    if (IOS)
        set (USER_CONF_TARGET_ARCH arm)
    else ()
        include (CheckSymbolExists)
        if (MSVC)
            CHECK_SYMBOL_EXISTS("_M_ARM" "" PRIVATE_TARGET_ARCH_ARM)
            CHECK_SYMBOL_EXISTS("_M_X64" "" PRIVATE_TARGET_ARCH_X86_64)
            CHECK_SYMBOL_EXISTS("_M_IX86" "" PRIVATE_TARGET_ARCH_X86)
        else ()
            CHECK_SYMBOL_EXISTS("__arm__" "" PRIVATE_TARGET_ARCH_ARM)
            CHECK_SYMBOL_EXISTS("__x86_64__" "" PRIVATE_TARGET_ARCH_X86_64)
            CHECK_SYMBOL_EXISTS("__i386__" "" PRIVATE_TARGET_ARCH_X86)
        endif ()
        if (PRIVATE_TARGET_ARCH_ARM)
            set (USER_CONF_TARGET_ARCH arm)
        elseif (PRIVATE_TARGET_ARCH_X86_64)
            set (USER_CONF_TARGET_ARCH x86_64)
        elseif (PRIVATE_TARGET_ARCH_X86)
            set (USER_CONF_TARGET_ARCH x86)
        else ()
            message(FATAL_ERROR "Unknown target processor architecture.")
        endif ()
    endif ()
endif ()

#=======================================================================
# USER_CONF_USE_EGL_PLATFORM:
#    - android: currently the only supported value.
#
if (NOT DEFINED USER_CONF_USE_EGL_PLATFORM)
    # default to use EGL I/F same as linux
    set (USER_CONF_USE_EGL_PLATFORM "linux")
endif ()

if ((NOT DEFINED CMAKE_BUILD_TYPE) OR ("" STREQUAL "${CMAKE_BUILD_TYPE}"))
    set (CMAKE_BUILD_TYPE Debug)
endif ()

#=======================================================================
# USER_CONF_GLOBAL_C_FLAGS/USER_CONF_GLOBAL_CXX_FLAGS:
#    - compiler options to build binary targets
#
if (NOT DEFINED USER_CONF_GLOBAL_C_FLAGS)
    if (NUTSHELL AND "${NUTSHELL_TOOLCHAIN}" MATCHES "arm-poky-linux-gnueabi")

    elseif (NUTSHELL AND "${NUTSHELL_TOOLCHAIN}" MATCHES "x86-linux-gnueabi")

    elseif (ANDROID)

        set (USER_CONF_GLOBAL_C_FLAGS "${USER_CONF_GLOBAL_C_FLAGS} -D_FOR_ANDROID_ -g")

    elseif (APPLE)

        set (USER_CONF_GLOBAL_C_FLAGS "${USER_CONF_GLOBAL_C_FLAGS} -D_FOR_APPLE_ -g")
        if (IOS)
            set (USER_CONF_GLOBAL_C_FLAGS "${USER_CONF_GLOBAL_C_FLAGS} -D_FOR_IPHONE_")
        endif ()

    elseif (UNIX)

        set (USER_CONF_GLOBAL_C_FLAGS "${USER_CONF_GLOBAL_C_FLAGS} -D_LINUX -g")

    endif()
endif ()

#=======================================================================
# USER_CONF_INCLUDES_FOR_OPENGL:
#    - folder for OpenGLES header files.
#
if (NUTSHELL AND (NOT DEFINED USER_CONF_INCLUDES_FOR_OPENGL))

endif ()

#=======================================================================
# USER_CONF_PLATFORM_INCLUDES_FILE:
#    - file to import platform dependent include directories
#
# NOTE: MUST be places after 'enable_language', since some platform dependent
#       variables will be set in toolchain file.
#
if (NOT DEFINED USER_CONF_PLATFORM_INCLUDES_FILE)
    # Directory of the external dependencies' build tree
    if (NUTSHELL)
        get_filename_component (USER_CONF_PLATFORM_INCLUDES_FILE "${CMAKE_SOURCE_DIR}/platform_includes_for_nutshell.cm" ABSOLUTE)
    endif ()
endif ()

#=======================================================================
# USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR:
#     - full path to root of external dependencies' build tree, static libs/
#       dynamic libs/intermediate files will be put in directory
#       STATIC_LIBRARIES/lib/<name>_intermediates respectively.
# USER_CONF_TARGET_OUT_INTERMEDIATES:
#    - relative path to root of external dependencies' build tree.
#
if (NOT DEFINED USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR)
    # Build result of external projects
    if (NUTSHELL)
        if ((DEFINED ENV{NUTSHELL_PRODUCT_OUT}) AND (NOT "$ENV{NUTSHELL_PRODUCT_OUT}" STREQUAL ""))
            get_filename_component (USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR "$ENV{NUTSHELL_PRODUCT_OUT}/obj" ABSOLUTE)
            file (RELATIVE_PATH USER_CONF_TARGET_OUT_INTERMEDIATES "$ENV{PWD}" "${USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR}")
        else ()
            message (FATAL_ERROR "Please run \"lunch ...\" first.")
        endif ()
    else ()
        get_filename_component (USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR "${USER_CONF_PROJECT_ROOT_DIR}/../out/target/${USER_CONF_TARGET_PRODUCT}" ABSOLUTE)
        file (RELATIVE_PATH USER_CONF_TARGET_OUT_INTERMEDIATES "$ENV{PWD}" "${USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR}")
    endif (NUTSHELL)
else()
endif ()
if (NOT DEFINED USER_CONF_TARGET_OUT_INTERMEDIATES)
    file (RELATIVE_PATH USER_CONF_TARGET_OUT_INTERMEDIATES "$ENV{PWD}" "${USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR}")
endif ()
if (NOT NUTSHELL)
    include_directories("${USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR}/include")
endif ()

#=======================================================================
# USER_CONF_LIBRARY_OUTPUT_PATH:
#     - full path to root of install tree, static libs/dynamic libs/executables
#       will be install in directory static/shared/bin respectively.
#
if (NOT DEFINED USER_CONF_LIBRARY_OUTPUT_PATH)
    # Root directory of the build tree
    get_filename_component (USER_CONF_LIBRARY_OUTPUT_PATH "${USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR}" ABSOLUTE)
endif ()

#=======================================================================
# USER_CONF_DEP_EXTERNAL_MODULES_FILE:
#    - full path of the file to save external dependencies.
#
if (NOT DEFINED USER_CONF_DEP_EXTERNAL_MODULES_FILE)
    # Path of the generated external dependencies
    get_filename_component (USER_CONF_DEP_EXTERNAL_MODULES_FILE "${USER_CONF_LIBRARY_OUTPUT_PATH}/cmake/navi_external_modules.txt" ABSOLUTE)
endif ()

#=======================================================================
# USER_CONF_PROTOBUF_OUT_DIR:
#    - full path to root directory of protoc generated header files.
# USER_CONF_TARGET_OUT_HEADERS:
#    - relative path to root directory of protoc generated header files.
#
if (NOT DEFINED USER_CONF_PROTOBUF_OUT_DIR)
    # Directory of generated protobuffer header files (absolute/relative path).
    set (USER_CONF_PROTOBUF_OUT_DIR "${USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR}/include")
    file(RELATIVE_PATH USER_CONF_TARGET_OUT_HEADERS "$ENV{PWD}" "${USER_CONF_PROTOBUF_OUT_DIR}/build")
endif ()
if (NOT DEFINED USER_CONF_TARGET_OUT_HEADERS)
    file (RELATIVE_PATH USER_CONF_TARGET_OUT_HEADERS "$ENV{PWD}" "${USER_CONF_PROTOBUF_OUT_DIR}")
endif ()

message ("================================================================")
message ("CMAKE_C_COMPILER                      = ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_ARG1}")
message ("CMAKE_CXX_COMPILER                    = ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1}")
message ("================================================================")
message ("USER_CONF_PROJECT_ROOT_DIR            = ${USER_CONF_PROJECT_ROOT_DIR}")
message ("USER_CONF_LIB_SRC_DIR                 = ${USER_CONF_LIB_SRC_DIR}")
message ("================================================================")
message ("USER_CONF_TARGET_OUT_INTERMEDIATES    = ${USER_CONF_TARGET_OUT_INTERMEDIATES}")
message ("USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR = ${USER_CONF_EXTERNAL_MODULES_OUTPUT_DIR}")
message ("USER_CONF_TARGET_OUT_HEADERS          = ${USER_CONF_TARGET_OUT_HEADERS}")
message ("USER_CONF_PROTOBUF_OUT_DIR            = ${USER_CONF_PROTOBUF_OUT_DIR}")
message ("================================================================")
message ("USER_CONF_LIBRARY_OUTPUT_PATH         = ${USER_CONF_LIBRARY_OUTPUT_PATH}")
message ("USER_CONF_DEP_EXTERNAL_MODULES_FILE   = ${USER_CONF_DEP_EXTERNAL_MODULES_FILE}")
message ("================================================================")
message ("USER_CONF_PLATFORM_INCLUDES_FILE      = ${USER_CONF_PLATFORM_INCLUDES_FILE}")
message ("================================================================")
message ("USER_CONF_TARGET_PRODUCT              = ${USER_CONF_TARGET_PRODUCT}")
message ("USER_CONF_TARGET_DEVICE               = ${USER_CONF_TARGET_DEVICE}")
message ("USER_CONF_TARGET_BOARD_TYPE           = ${USER_CONF_TARGET_BOARD_TYPE}")
message ("USER_CONF_TARGET_ARCH                 = ${USER_CONF_TARGET_ARCH}")
message ("USER_CONF_USE_EGL_PLATFORM            = ${USER_CONF_USE_EGL_PLATFORM}")
message ("================================================================")
