project(LuaScript)
file(GLOB lua_src "${PROJECT_SOURCE_DIR}/lua/*.c")

include_directories(${PROJECT_SOURCE_DIR}/lua)
add_library(lua STATIC ${lua_src})
add_program_with(luascript "lua" "" "${PROJECT_SOURCE_DIR}/lua" src/luascript.cpp)
