#!/bin/bash
echo "Building..." \
&& g++ archiver.cpp crc.cpp sha2.c \
    -lboost_filesystem-mt -lboost_date_time-mt -lboost_program_options-mt \
    -g \
&& echo "Removing ohnoes.far..." \
&& rm -f ohnoes.far \
&& echo "Creating ohnoes.far..." \
&& echo "======================" \
&& ./a.out --create ohnoes.far CMakeLists.txt archiver.cpp features.txt \
&& echo "Showing info on ohnoes.far..." \
&& echo "=============================" \
&& ./a.out --info ohnoes.far \
&& echo "Listing files in ohnoes.far..." \
&& echo "==============================" \
&& ./a.out --list ohnoes.far
