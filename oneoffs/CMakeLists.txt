project(Oneoffs)
cmake_minimum_required(VERSION 2.6)

#add_definitions("-std=c++0x")

###
### Build projects
###
add_simple_oneoff(findnextfile)
add_simple_oneoff(mkcrctable)
add_simple_oneoff(stdequalstest)
add_simple_oneoff(string_to_ints)
add_simple_oneoff(checkdebug)

# Temporarily disable warnings
set(SAVED_CXX_FLAGS "${CXX_FLAGS}")
set(CXX_FLAGS "")
add_simple_oneoff(template_compiler_spec)
set(CXX_FLAGS "${SAVED_CXX_FLAGS}")

# add_simple_oneoff(gameworld)
# add_oneoff(googletest)
# add_oneoff(sdlfonts)
