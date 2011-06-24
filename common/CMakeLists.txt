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
project(CommonLibs)
set(COMMON_LIBS_VERSION 2)

cmake_minimum_required(VERSION 2.6)

init_googletest()
include_directories(${PROJECT_SOURCE_DIR}/include)

#########################################################################
### Core common lib
#########################################################################
set(commonlib_incs
        include/common/delete.h
        include/common/color.h
        include/common/noncopyable.h
        include/common/singleton.h
        include/common/static_assert.h
        include/common/nullptr.h
        include/common/time.h
        include/common/utils.h)

set(commonlib_srcs
        src/common/time.cpp)

set(commonlib_tests
        tests/common/test_delete.cpp
        tests/common/test_color.cpp)

#########################################################################
### Application common lib
#########################################################################
set(applib_incs
        include/app/app.h
        include/app/debug.h
        include/app/platform.h
        include/app/windowsinclude.h)

set(applib_srcs
        src/app/app.cpp
        src/app/debug.cpp)

set(applib_tests
        )

#########################################################################
### Geometry library
#########################################################################
set(geomlib_incs
        include/geom/boundingbox.h
        include/geom/camera.h
        include/geom/fpscamera.h
        include/geom/frustum.h
        include/geom/intersection.h
        include/geom/plane.h
        include/geom/ray.h
        include/geom/sphere.h)

set(geomlib_srcs
        src/geom/geoms.cpp
        src/geom/frustum.cpp)

set(geomlib_tests
        tests/geom/test_ray.cpp
        tests/geom/test_intersectionresult.cpp)

#########################################################################
### Math library
#########################################################################
set(mathlib_incs
        include/math/angle.h
        include/math/constants.h
        include/math/conversion.h
        include/math/defs.h
        include/math/matrix.h
        include/math/interpolation.h
        include/math/random.h
        include/math/point.h
        include/math/util.h
        include/math/vector.h)

set(mathlib_srcs
        src/math/utils.cpp)

set(mathlib_tests
        tests/math/test_angle.cpp
        tests/math/test_matrix2.cpp
        tests/math/test_matrix4.cpp
        tests/math/test_point.cpp
        tests/math/test_utils.cpp
        tests/math/test_vector3.cpp)

#########################################################################
### String common lib                                                 ###
#########################################################################
set(stringlib_incs
        include/string/tokenizer.h
        include/string/stringbuffer.h
        include/string/makestring.h
        include/string/lcasts.h
        include/string/util.h)

set(stringlib_srcs
        src/string/tokenizer.cpp
        src/string/replace.cpp
        src/string/loadfile.cpp
        src/string/streamprint.cpp)

set(stringlib_tests
        tests/string/test_tokenizer.cpp
        tests/string/test_streamprint.cpp
        tests/string/test_replace.cpp)

##########################################################################
### Platform common lib                                                ###
##########################################################################
set(platform_incs
        include/platform/stacktrace.h)

set(platform_srcs
        src/platform/stacktrace.cpp)

set(platform_tests
        )


###====================================================================###
### Create separate libraries for every common library                 ###
###====================================================================###
add_library(commonlib   STATIC ${commonlib_srcs} ${commonlib_incs})
add_library(applib      STATIC ${applib_srcs}    ${applib_incs})
add_library(geomlib     STATIC ${geomlib_srcs}   ${geomlib_incs})
add_library(mathlib     STATIC ${mathlib_srcs}   ${mathlib_incs})
add_library(stringlib   STATIC ${stringlib_srcs} ${stringlib_incs})
add_library(platformlib STATIC ${platform_srcs}  ${platform_incs})

###====================================================================###
### Create an executable that will run all unit tests for every common ###
### library that we build.                                             ###
###====================================================================###
set(testlibs "commonlib;applib;geomlib;mathlib;stringlib")
set(tests    ${commonlib_tests}
             ${applib_tests}
             ${geomlib_tests}
             ${mathlib_tests}
             ${stringlib_tests}
             ${platform_tests})

add_program_with( testcommonlibs
                  "${testlibs};googletest"
                  "${PROJECT_SOURCE_DIR}/include"
                  ""
                  ${tests} ${GTEST_RUNNER_PATH} )
