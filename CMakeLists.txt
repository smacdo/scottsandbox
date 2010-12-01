###
### Common build system
###
project(ScottsSharedCodeRepository)
cmake_minimum_required(VERSION 2.6)

# Pull in shared functions
include(extra/cmake/sharedfuncs.cmake)

# Bring in google tests
add_subdirectory(extra/googletest)

###
### Global build flags
###
set(CXX_FLAGS "-Wall -Wextra -Werror")

###
### Build workbench code
###
add_subdirectory(oneoffs)
add_subdirectory(workbench)
add_subdirectory(miniapps)
