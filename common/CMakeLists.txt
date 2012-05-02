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
add_definitions("-DNDEBUG=1 " "-D_DEBUG=1")

###=====================================================================###
### Extra warnings and options when building the common library         ###
###=====================================================================###
set(XTRAOPTS "${XTRAOPTS} -std=c++0x")
set(XTRAOPTS "${XTRAOPTS} -mtune=native -msse2 -mfpmath=sse -msse3")


# Extra warnings
set(XTRAWARNS "-Wextra -Wdouble-promotion -Winit-self -Wswitch-enum" )
set(XTRAWARNS "${XTRAWARNS} -Wuninitialized -Wfloat-equal -Wsign-conversion" )
set(XTRAWARNS "${XTRAWARNS} -Wpointer-arith -Wconversion -Wlogical-op" )
set(XTRAWARNS "${XTRAWARNS} -Wctor-dtor-privacy -Wnon-virtual-dtor" )
set(XTRAWARNS "${XTRAWARNS} -Woverloaded-virtual" )
#set(XTRAWARNS "${XTRAWARNS} -Wnarrowing" )
#set(XTRAWARNS "${XTRAWARNS} -Wzero-as-null-pointer-constant" )

#########################################################################
### Core common lib
#########################################################################
set(common_incs
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

set(common_srcs
        src/common/time.cpp
)

set(common_tests
        tests/common/test_delete.cpp
        tests/common/test_deref.cpp
)

#########################################################################
### Application common lib
#########################################################################
set(app_incs
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

set(app_srcs
        src/app/app.cpp
        src/app/debug.cpp
        src/app/globallog.cpp
        src/app/log.cpp
        src/app/logentry.cpp
        src/app/logstream.cpp
        src/app/platform.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(app_srcs ${app_srcs} src/app/platform_darwin.mm)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    SET(app_srcs ${app_srcs} src/app/platform_xp.cpp)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET(app_srcs ${app_srcs} src/app/platform_unix.cpp)
endif()

set(app_tests
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
set(geom_incs
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

set(geom_srcs
        src/geom/boundingbox.cpp
        src/geom/color.cpp
        src/geom/frustum.cpp
        src/geom/geoms.cpp
        src/geom/intersectresult.cpp
        src/geom/plane.cpp
        src/geom/ray.cpp
        src/geom/utils.cpp
)

set(geom_tests
        tests/geom/test_ray.cpp
        tests/geom/test_intersectionresult.cpp
        tests/geom/test_color.cpp
)

#########################################################################
### Math library
#########################################################################
set(math_incs
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

set(math_srcs
        src/math/vector.cpp
        src/math/utils.cpp
)

set(math_tests
        tests/math/test_angle.cpp
        tests/math/test_conversions.cpp
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
set(string_incs
        include/string/crc.h
        include/string/lcasts.h
        include/string/makestring.h
        include/string/reftag.h
        include/string/sequenceformatter.h
        include/string/stringbuffer.h
        include/string/tokenizer.h
        include/string/util.h
)

set(string_srcs
        src/string/crc.cpp
        src/string/loadfile.cpp
        src/string/replace.cpp
        src/string/streamprint.cpp
        src/string/tokenizer.cpp
)

set(string_tests
        tests/string/test_crc.cpp
        tests/string/test_tokenizer.cpp
        tests/string/test_streamprint.cpp
)

###====================================================================###
### Generate a static library to hold all of the common code for use   ###
### in our other projects                                              ###
###====================================================================###
set( allcommonlibsrcs 
            ${common_srcs} ${common_incs}
            ${app_srcs} ${app_incs}
            ${math_srcs} ${math_incs}
            ${string_srcs} ${string_incs}
            ${geom_srcs} ${geom_incs}
)

set_source_files_properties(
    ${allcommonlibsrcs}
    PROPERTIES COMPILE_FLAGS "${CXX_FLAGS} -ggdb ${XTRAOPTS} ${XTRAWARNS}"
)

add_library(common STATIC ${allcommonlibsrcs} )

###=====================================================================###
### Unit test runner that executes all the common lib unit tests        ###
###=====================================================================###
set(tests
    ${common_tests}
    ${app_tests}
    ${math_tests}
    ${geom_tests}
    ${string_tests}
)

add_program_with( testcommonlibs
                  "common;googletest"
                  "${PROJECT_SOURCE_DIR}/include"
                  "-std=c++0x"
                  ${tests} ${GTEST_RUNNER_PATH} )
