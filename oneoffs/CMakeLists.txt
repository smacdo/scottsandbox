project(Oneoffs)
cmake_minimum_required(VERSION 2.6)

###
### Build projects
###
add_simple_oneoff(findnextfile)
add_simple_oneoff(mkcrctable)
add_simple_oneoff(stdequalstest)
add_simple_oneoff(string_to_ints)
add_simple_oneoff(checkdebug)
add_simple_oneoff(ppm)
add_simple_oneoff(assignopviapointer)
add_simple_oneoff(nastynull)
add_simple_oneoff(singleton)

# Temporarily disable warnings
set(SAVED_CXX_FLAGS "${CXX_FLAGS}")
set(CXX_FLAGS "")
add_simple_oneoff(template_compiler_spec)
set(CXX_FLAGS "${SAVED_CXX_FLAGS}")

add_oneoff( filesystem  "boost_filesystem-mt" " " " " )
add_oneoff( pathfinding " " " " "-std=c++0x" )
