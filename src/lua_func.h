// lua_func.h
#ifndef LUA_FUNC_H
#define LUA_FUNC_H

extern "C" {
  #include "lua.h"
}

int lua_pset(lua_State* L);
void register_lua_functions(lua_State* L);

#endif