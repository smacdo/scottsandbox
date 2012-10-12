###########################################################################
# Common string code
###########################################################################
set(includes
        ${CMAKE_CURRENT_SOURCE_DIR}/crc.h
        ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/stringutil.h
)

set(sources
        ${CMAKE_CURRENT_SOURCE_DIR}/crc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/stringutil.cpp
)

set(tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_crc.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_lcasts.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_tokenizer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_sequencelister.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_streamprint.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_stringutils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_transferstring.cpp
)

set( libcommon_incs  ${libcommon_incs}  ${includes} PARENT_SCOPE )
set( libcommon_srcs  ${libcommon_srcs}  ${sources} PARENT_SCOPE )
set( libcommon_tests ${libcommon_tests} ${tests} PARENT_SCOPE )
