cmake_minimum_required(VERSION 2.6)
PROJECT(ArchiveTool)

find_package(Boost COMPONENTS filesystem program_options date_time REQUIRED)

set(srcs commandline.cpp archive.cpp archivedata.cpp fileentry.cpp
         thirdparty/sha2.c crc.cpp )

add_executable( far_tool ${srcs} )
target_link_libraries( far_tool ${Boost_FILESYSTEM_LIBRARY}
                                ${Boost_PROGRAM_OPTIONS_LIBRARY}
                                ${Boost_DATE_TIME_LIBRARY} )
