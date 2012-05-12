#########################################################################
# Common libs
# -----------
#
# This subdirectory provides re-usable unit tested classes and functions
# to accomplish useful functionality including core common code, string
# manipulation, math calculations and geometry processing
#
# You do not need to use everything in here, just remove the libraries
# that you are not interested in :)
########################################################################
project(commonlibs)
set(COMMON_LIBS_VERSION 3)

cmake_minimum_required(VERSION 2.8)

#=========================================================================#
# libcommon components                                                    #
#=========================================================================#
set( libcommon_incs  "" )
set( libcommon_srcs  "" )
set( libcommon_tests "" )

add_subdirectory(common)
add_subdirectory(math)
add_subdirectory(app)
add_subdirectory(string)
#add_subdirectory(game2d)
add_subdirectory(game3d)

#=========================================================================#
# Options for building libcommon                                          #
#=========================================================================#
option(build_libcommon_tests    "Build libcommon unit tests" on)
option(libcommon_cpp0x          "Build libcommon with C++0x support" on)
option(libcommon_assert         "Build libcommon with custom asserts" on)
option(libcommon_debug          "Build libcommon with debugging support" on)
option(libcommon_warnings       "Build libcommon with warnings" on)
option(libcommon_more_warnings  "Build libcommon with lots of warnings" off)
option(libcommon_extra_warnings "Build libcommon with all the warnings" off)
option(libcommon_performance    "Build libcommon with performance flags" on)

#=========================================================================#
# Compiler settings and configuration                                     #
#=========================================================================#
set( CXX_FLAGS "" )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

if(libcommon_cpp0x)
    set(CXX_FLAGS "${CXX_FLAGS} -std=c++0x")
endif()

if(libcommon_assert)
    # Turns off the built-in assertion macro
    #   XXX: Is there a better way of doing this?
    add_definitions("-DNDEBUG=1 " "-D_DEBUG=1")
endif()

if(libcommon_debug)
    set(CXX_FLAGS "${CXX_FLAGS} -ggdb")
endif()

if(libcommon_performance)
    # clang does not support -mfpmath=sse
    set(CXX_FLAGS "${CXX_FLAGS} -mtune=native -msse2 -msse3")
endif()

#=========================================================================#
# Warning configuration                                                   #
#=========================================================================#
if(libcommon_warnings)
    set(CXX_FLAGS "${CXX_FLAGS} -Wall -Werror" )
endif()

if(libcommon_more_warnings)
    set(CXX_FLAGS "${CXX_FLAGS} -Wextra -Winit-self -Wswitch-enum" )
    set(CXX_FLAGS "${CXX_FLAGS} -Wuninitialized -Wfloat-equal -Wsign-conversion" )
    set(CXX_FLAGS "${CXX_FLAGS} -Wpointer-arith -Wconversion" )
    set(CXX_FLAGS "${CXX_FLAGS} -Wctor-dtor-privacy -Wnon-virtual-dtor" )
    set(CXX_FLAGS "${CXX_FLAGS} -Woverloaded-virtual" )
endif()

if(libcommon_extra_warnings)
    set(CXX_FLAGS "${CXX_FLAGS} -Wlogical-op -Wdouble-promotion")
    set(CXX_FLAGS "${CXX_FLAGS} -Wnarrowing" )
    set(CXX_FLAGS "${CXX_FLAGS} -Wzero-as-null-pointer-constant" )
endif()

#=========================================================================#
# Apply all compile flags to the source code and header files             #
#=========================================================================#
set_source_files_properties(
    ${libcommon_incs}
    ${libcommon_srcs}
    PROPERTIES COMPILE_FLAGS "${CXX_FLAGS} ${LIBCOMMON_FLAGS}"
)

#=========================================================================#
# Generate a static library to hold all of the common code for use        #
# in our other projects                                                   #
#=========================================================================#
add_library(common STATIC ${libcommon_srcs} ${libcommon_incs} )

#=========================================================================#
# Create a program that executes all of libcommon's unit tests            #
#=========================================================================#
if(build_libcommon_tests)
    init_googletest()
    add_program_with( testcommonlibs
                      "common;googletest"
                      "${PROJECT_SOURCE_DIR}/include"
                       ""
                       ${libcommon_tests} testrunner.cpp )
endif()
