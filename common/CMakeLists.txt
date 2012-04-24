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
set(COMMON_LIBS_VERSION 3)

cmake_minimum_required(VERSION 2.8)

init_googletest()
include_directories(${PROJECT_SOURCE_DIR}/include)
add_definitions("-DNDEBUG=1 -DDEBUG=1")

#########################################################################
### Core common lib
#########################################################################
set(commonlib_incs
        include/common/delete.h
        include/common/deref.h
        include/common/macros.h
        include/common/noncopyable.h
        include/common/nullptr.h
        include/common/pointer.h
        include/common/singleton.h
        include/common/static_assert.h
        include/common/time.h
        include/common/utils.h
)

set(commonlib_srcs
        src/common/time.cpp
)

set(commonlib_tests
        tests/common/test_delete.cpp
        tests/common/test_deref.cpp
)

#########################################################################
### Application common lib
#########################################################################
set(applib_incs
        include/app/app.h
        include/app/debug.h
        include/app/logging.h
        include/app/logging_debugstreambuf.h
        include/app/logging_debugstreambuf.inl
        include/app/logging_stream.h
        include/app/osplatform.h
        include/app/platform.h
        include/app/windowsinclude.h
)

set(applib_srcs
        src/app/app.cpp
        src/app/debug.cpp
        src/app/globallog.cpp
        src/app/log.cpp
        src/app/logentry.cpp
        src/app/logstream.cpp
        src/app/platform.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(applib_srcs ${applib_srcs} src/app/platform_darwin.cpp)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(applib_srcs ${applib_srcs} src/app/platform_xp.cpp)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(applib_srcs ${applib_srcs} src/app/platform_unix.cpp)
endif()

set(applib_tests
        )

#########################################################################
### Game2d
#########################################################################
set(game2d_incs
    include/game2d/fixedgrid.h
    include/game2d/point.h
)

set(game2d_srcs
    src/game2d/point.cpp
)

set(game2d_tests
    tests/game2d/test_fixedgrid.cpp
    tests/game2d/test_point.cpp
)

#########################################################################
### Game3d (geomlib)
#########################################################################
set(geomlib_incs
        include/geom/boundingbox.h
        include/geom/camera.h
        include/geom/defs.h
        include/geom/fpscamera.h
        include/geom/frustum.h
        include/geom/intersection.h
        include/geom/plane.h
        include/geom/primitives.h
        include/geom/ray.h
        include/geom/sphere.h
        include/geom/utils.h
)

set(geomlib_srcs
        src/geom/boundingbox.cpp
        src/geom/frustum.cpp
        src/geom/geoms.cpp
        src/geom/intersectresult.cpp
        src/geom/plane.cpp
        src/geom/ray.cpp
        src/geom/utils.cpp
)

set(geomlib_tests
        tests/geom/test_ray.cpp
        tests/geom/test_intersectionresult.cpp
        tests/geom/test_color.cpp
)

#########################################################################
### Math library
#########################################################################
set(mathlib_incs
        include/math/angle.h
        include/math/constants.h
        include/math/conversion.h
        include/math/defs.h
        include/math/interpolation.h
        include/math/matrix.h
        include/math/matrixutils.h
        include/math/point.h
        include/math/random.h
        include/math/rational.h
        include/math/rect.h
        include/math/tmatrix.h
        include/math/util.h
        include/math/vector.h
)

set(mathlib_srcs
        src/math/vector.cpp
        src/math/utils.cpp
)

set(mathlib_tests
        tests/math/test_angle.cpp
        tests/math/test_matrix4.cpp
        tests/math/test_point.cpp
        tests/math/test_rect.cpp
        tests/math/test_utils.cpp
        tests/math/test_vector4.cpp
        tests/math/test_vector3.cpp
        tests/math/test_vector2.cpp
)

#########################################################################
### String common lib                                                 ###
#########################################################################
set(stringlib_incs
        include/string/crc.h
        include/string/lcasts.h
        include/string/makestring.h
        include/string/reftag.h
        include/string/sequenceformatter.h
        include/string/stringbuffer.h
        include/string/tokenizer.h
        include/string/util.h
)

set(stringlib_srcs
        src/string/crc.cpp
        src/string/loadfile.cpp
        src/string/replace.cpp
        src/string/streamprint.cpp
        src/string/tokenizer.cpp
        src/string/utils.cpp
)

set(stringlib_tests
        tests/string/test_crc.cpp
        tests/string/test_tokenizer.cpp
        tests/string/test_streamprint.cpp
        tests/string/test_replace.cpp)

###====================================================================###
### Create separate libraries for every common library                 ###
###====================================================================###
add_library(common STATIC 
            ${commonlib_srcs} ${commonlib_incs}
            ${applib_srcs} ${applib_incs}
            ${mathlib_srcs} ${mathlib_incs}
            ${stringlib_srcs} ${stringlib_incs}
            ${geomlib_srcs} ${geomlib_incs}
)

###====================================================================###
### Create an executable that will run all unit tests for every common ###
### library that we build.                                             ###
###====================================================================###
set(testlibs "common")
set(tests    ${commonlib_tests}
             ${applib_tests}
             ${mathlib_tests}
             ${geomlib_tests}
             ${stringlib_tests}
)

add_program_with( testcommonlibs
                  "${testlibs};googletest"
                  "${PROJECT_SOURCE_DIR}/include"
                  "-std=c++0x"
                  ${tests} ${GTEST_RUNNER_PATH} )
