#include "lua_func.h"
extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}
#include "pico8_mem.h"
#include "gfx.h"
int lua_pset(lua_State* L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  int col = luaL_optinteger(L, 3, currentColor);
  pset(x, y, col);
  return 0;
}
int lua_cls(lua_State* L) {
  int col = luaL_optinteger(L, 1, currentColor);
  cls(col);
  return 0;
}
int lua_flip(lua_State* L) {
  flip();
  return 0;
}

int lua_rnd(lua_State* L) {
    int nargs = lua_gettop(L);

    if (nargs == 0) {
        lua_pushnumber(L, rnd());  // rnd() => float in [0,1)
        return 1;
    }

    if (lua_istable(L, 1)) {
        int len = lua_objlen(L, 1);  
        if (len == 0) {
            lua_pushnil(L);
            return 1;
        }

        int index = (int)(rnd((float)len)) + 1;  
        return 1;
    }

    // Assume number
    float x = luaL_checknumber(L, 1);
    lua_pushnumber(L, rnd(x));  // float in [0, x)
    return 1;
}


// similarly lua_spr, lua_cls...

void register_lua_functions(lua_State* L) {
  lua_register(L, "pset", lua_pset);
  lua_register(L, "cls", lua_cls);
  lua_register(L, "flip", lua_flip);
  lua_register(L, "rnd", lua_rnd);

}