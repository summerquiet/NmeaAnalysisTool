CMAKE_MINIMUM_REQUIRED (VERSION 2.8.8)
cmake_policy (VERSION 2.8.8)
if (NOT (CMAKE_MAJOR_VERSION LESS 3))
    cmake_policy (SET CMP0046 OLD)
endif ()

#
# IMPORTANT:
#    MUST be placed before the first 'project' or 'enable_language'
#    statement, or it will NOT work as expected.
#
include(global_vars.guess.cmake)

PROJECT (Project)

include(global_functions.cm)
include(global_vars.cm)
include(global_setting.cm)

#
# For certain libraries, they're provided as part of the OS distribution. But
# if any of them is missing, try build them from source.
#

g_func_clear_private_var()

#------------------------------------------------------------
# Platform-dependent Modules Begin
#------------------------------------------------------------

# For runs on Linux

#------------------------------------------------------------
# Platform-dependent Modules End
#------------------------------------------------------------

#------------------------------------------------------------
# Common (platform-independent) Modules Begin
#------------------------------------------------------------
#----Inner the 3rd party modules Begin

include(../src/3rdParty/tinyxml/tinyxml-ant.cm)
include(../src/3rdParty/zeromq/zeromq-ant.cm)

#----Inner the 3rd party modules End


#----Inner the Major Modules Begin

include(../src/antlib/antlib-ant.cm)

#----Inner the Major Modules End
#------------------------------------------------------------
# Common (platform-independent) Modules End
#------------------------------------------------------------

PROJECT (Project)

g_func_build_external_modules()
