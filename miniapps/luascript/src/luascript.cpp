#include <iostream> 

extern "C"
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

int main( int , char*[] )
{
    lua_State *lua = luaL_newstate();

    std::cout << "loading" << std::endl;
    luaL_dofile( lua, "script.lua" );
    std::cout << "all done" << std::endl;

    lua_close( lua );
    return 0;
}
