###########################################################################
# Scott's Sandbox - an ecletic collection of code                         #
###########################################################################
project(ScottsCodeSandbox)
cmake_minimum_required(VERSION 2.8)

#=========================================================================#
# Load common cmake modules to incorporate functionality not found in     #
# standard CMake distribution                                             #
#=========================================================================#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include( config )
include( GetGitRevisionDescription )
include( GetCompilerInfoString )
include( EnableExtraCompilerWarnings )
include( ./BuildRules.cmake )

#=========================================================================#
# Build options - Enable or disable these options to configure the build  #
#=========================================================================#
option(BUILD_THIRDPARTY "Build thirdparty" true)
option(BUILD_COMMON "Build libcommon" true)
option(BUILD_ONEOFFS "Build one-off applications" false)
option(BUILD_WORKBENCH "Build work in progress applications" false)
option(BUILD_MINIAPPS "Build miniature applications" false)

#=========================================================================#
#  Apple OSX Platform support                                             #
#   - Changes CMake defaults to use clang as compiler and set up XCode    #
#     project settings                                                    #
#=========================================================================#
# Special  XCode (v4.0) Support
if(DARWIN)
    set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++0x")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x -stdlib=libc++ -g -Wall")

    include_directories(/opt/local/include/)
endif()

#=========================================================================#
# Sub projects                                                            #
#=========================================================================#
if(BUILD_THIRDPARTY)
    add_subdirectory(thirdparty)
endif()

if(BUILD_COMMON)
    add_subdirectory(libcommon)
endif()

if(BUILD_ONEOFFS)
    add_subdirectory(oneoffs)
endif()

if(BUILD_WORKBENCH)
    add_subdirectory(workbench)
endif()

if(BUILD_MINIAPPS)
    add_subdirectory(miniapps)
endif()
