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

bool CheckLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}
	return true;
}

int main()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//if (r == LUA_OK)
	//{
	//	lua_getglobal(L, "a");
	//	if (lua_isnumber(L, -1))
	//	{
	//		float a = (float)lua_tonumber(L, -1);
	//		std::cout << a << std::endl;
	//	}
	//
	//}
	//else
	//{
	//	std::string errormsg = lua_tostring(L, -1);
	//	std::cout << errormsg << std::endl;
	//}

	if (CheckLua(L, luaL_dofile(L, "testScript.lua")))
	{
		for (int i = 0; i < 30; ++i)
		{
			lua_getglobal(L, "Update");
			if (lua_isfunction(L, -1))
			{
				lua_pushnumber(L, 0.1);
				if (CheckLua(L, lua_pcall(L, 1, 1, 0)))
				{
					if (lua_istable(L, -1))
					{
						lua_pushstring(L, "x");
						lua_gettable(L, -2);
						std::cout << lua_tostring(L, -1) << std::endl;
						lua_pop(L, 1);
						lua_pushstring(L, "y");
						lua_gettable(L, -2);
						std::cout << lua_tostring(L, -1) << std::endl;
						lua_pop(L, 1);
					}

				}
			}
		}
	}

	return 0;
}