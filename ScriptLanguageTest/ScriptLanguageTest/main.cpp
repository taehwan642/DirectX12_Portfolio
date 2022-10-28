#include <iostream>
#include <string>
#include <memory>

extern "C"
{
#include "lua/include/lua.h"
#include "lua/include/lauxlib.h"
#include "lua/include/lualib.h"
}

#pragma comment(lib, "lua/liblua54.a")

int main()
{
	lua_State* L = luaL_newstate();

	int r = luaL_dofile(L, "testScript.lua");

	if (r == LUA_OK)
	{
		lua_getglobal(L, "a");
		if (lua_isnumber(L, -1))
		{
			float a = (float)lua_tonumber(L, -1);
			std::cout << a << std::endl;
		}

	}
	else
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
	}

	return 0;
}