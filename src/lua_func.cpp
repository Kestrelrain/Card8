#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "lua_func.h"
extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}
#include "mem.h"
#include "gfx.h"
#include "math.h"


// abs(x)
int lua_abs(lua_State* L) {
    lua_pushnumber(L, fabs(luaL_checknumber(L, 1)));
    return 1;
}

// atan2(x, y)
int lua_atan2(lua_State* L) {
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 2);
    double angle = atan2(x, y) / (2.0 * M_PI); // Normalize to [0..1]
    if (angle < 0) angle += 1.0;
    lua_pushnumber(L, angle);
    return 1;
}

// cos(x)
int lua_cos(lua_State* L) {
    double x = luaL_checknumber(L, 1);
    double val = cos(x * 2.0 * M_PI);
    lua_pushnumber(L, val);
    return 1;
}

// sin(x)
int lua_sin(lua_State* L) {
    double x = luaL_checknumber(L, 1);
    double val = -sin(x * 2.0 * M_PI); // Pico-8 inverts sin
    lua_pushnumber(L, val);
    return 1;
}

// flr(x)
int lua_flr(lua_State* L) {
    lua_pushnumber(L, floor(luaL_checknumber(L, 1)));
    return 1;
}

// ceil(x) (not a Pico-8 function, but useful)
int lua_ceil(lua_State* L) {
    lua_pushnumber(L, ceil(luaL_checknumber(L, 1)));
    return 1;
}

// max(x, y)
int lua_max(lua_State* L) {
    double a = luaL_checknumber(L, 1);
    double b = luaL_checknumber(L, 2);
    lua_pushnumber(L, a > b ? a : b);
    return 1;
}

// min(x, y)
int lua_min(lua_State* L) {
    double a = luaL_checknumber(L, 1);
    double b = luaL_checknumber(L, 2);
    lua_pushnumber(L, a < b ? a : b);
    return 1;
}

// mid(x, y, z)
int lua_mid(lua_State* L) {
    double a = luaL_checknumber(L, 1);
    double b = luaL_checknumber(L, 2);
    double c = luaL_checknumber(L, 3);
    double result = std::max(std::min(a, b), std::min(std::max(a, b), c));
    lua_pushnumber(L, result);
    return 1;
}

// srand(seed)
int lua_srand(lua_State* L) {
    uint32_t seed = luaL_checkinteger(L, 1);
    pico8_srand(seed);
    return 0;
}


// sgn(x)
int lua_sgn(lua_State* L) {
    double x = luaL_checknumber(L, 1);
    lua_pushnumber(L, x < 0 ? -1 : 1);
    return 1;
}

// sqrt(x)
int lua_sqrt(lua_State* L) {
    lua_pushnumber(L, sqrt(luaL_checknumber(L, 1)));
    return 1;
}





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
    if (lua_gettop(L) == 0) {
        lua_pushnumber(L, pico8_rnd());
        return 1;
    }

    if (lua_istable(L, 1)) {
        int len = lua_objlen(L, 1);
        if (len == 0) {
            lua_pushnil(L);
            return 1;
        }
        int index = (int)(pico8_rnd(len));
        lua_rawgeti(L, 1, index + 1); // Lua is 1-indexed
        return 1;
    }

    float x = luaL_checknumber(L, 1);
    lua_pushnumber(L, pico8_rnd(x));
    return 1;
}


// similarly lua_spr, lua_cls...

void register_lua_functions(lua_State* L) {
  lua_register(L, "pset", lua_pset);
  lua_register(L, "cls", lua_cls);
  lua_register(L, "flip", lua_flip);
    // Math
  lua_register(L, "abs", lua_abs);
  lua_register(L, "sin", lua_sin);
  lua_register(L, "cos", lua_cos);
  lua_register(L, "atan2", lua_atan2);
  lua_register(L, "flr", lua_flr);
  lua_register(L, "rnd", lua_rnd);
  lua_register(L, "sqrt", lua_sqrt);
  lua_register(L, "sgn", lua_sgn);
  lua_register(L, "max", lua_max);
  lua_register(L, "min", lua_min);
  lua_register(L, "mid", lua_mid);
  lua_register(L, "srand", lua_srand);

}