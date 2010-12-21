project(CommonLibs)
cmake_minimum_required(VERSION 2.6)

init_googletest()
include_directories(${PROJECT_SOURCE_DIR}/include)

###
### Common subdirectory. This directory contains code marked as "common",
### which basically means its a library of highly reusable code that can
### be copied to new projects.
###
set(commonlib_incs 
        include/common/app.h
        include/common/noncopyable.h
        include/common/singleton.h
        include/common/static_assert.h
        include/common/utils.h)

set(commonlib_srcs
        src/common/app.cpp)

set(commonlib_srcs_nontestable
    )

set(commonlib_tests
    )

add_library(commonlib  ${commonlib_srcs} ${commonlib_srcs_nontestable})
add_library(commonlibX ${commonlib_srcs} )

###
### String common lib
###
set(stringlib_incs
        include/string/tokenizer.h)

set(stringlib_srcs
        src/string/tokenizer.cpp)

set(stringlib_tests
        tests/string/test_tokenizer.cpp)

add_library(stringlib STATIC ${stringlib_srcs})

###
### Test runner
###
set(testlibs "commonlibX;stringlib")
set(tests    ${commonlib_tests} ${stringlib_tests})

add_program_with( commonlibstest
                  "${testlibs};googletest"
                  "${PROJECT_SOURCE_DIR}/include"
                  ""
                  ${tests} ${GTEST_RUNNER_PATH} )
