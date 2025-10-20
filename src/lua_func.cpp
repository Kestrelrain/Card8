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
    double result = a;
    if ((b > a) != (b > c)) result = b;
    else if ((c > a) != (c > b)) result = c;
    lua_pushnumber(L, result);
    return 1;
}

// srand(seed)
int lua_srand(lua_State* L) {
    uint32_t seed = luaL_checkinteger(L, 1);
    _srand(seed);
    return 0;
}


// sgn(x)
int lua_sgn(lua_State* L) {
    double x = luaL_checknumber(L, 1);
    int sign = (x > 0) - (x < 0);
    lua_pushnumber(L, sign);
    return 0;
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
        lua_pushnumber(L, _rnd());
        return 1;
    }

    if (lua_istable(L, 1)) {
        int len = lua_objlen(L, 1);
        if (len == 0) {
            lua_pushnil(L);
            return 1;
        }
        int index = (int)(_rnd(len));
        lua_rawgeti(L, 1, index + 1); // Lua is 1-indexed
        return 1;
    }

    float x = luaL_checknumber(L, 1);
    lua_pushnumber(L, _rnd(x));
    return 1;
}

int lua_btn(lua_State* L) {
    int i = luaL_checkinteger(L, 1);
    int p = luaL_optinteger(L, 2, 0);

    bool result = btn(i, p);

    lua_pushboolean(L, result);
    return 1;
}
int lua_btnp(lua_State* L) {
    int i = luaL_checkinteger(L, 1);
    int p = luaL_optinteger(L, 2, 0);

    bool pressed = btnp(i, p);
    lua_pushboolean(L, pressed);
    return 1;
}


int lua_circfill(lua_State* L) {
    int x0 = luaL_checkinteger(L, 1);
    int y0 = luaL_checkinteger(L, 2);
    int r = luaL_checkinteger(L, 3);
    int col = luaL_optinteger(L, 4, 7); // default color white
    circfill(x0, y0, r, col);
    return 0;
}

// rectfill(x1, y1, x2, y2, col)
int lua_rectfill(lua_State* L) {
    int x1 = luaL_checkinteger(L, 1);
    int y1 = luaL_checkinteger(L, 2);
    int x2 = luaL_checkinteger(L, 3);
    int y2 = luaL_checkinteger(L, 4);
    int col = luaL_optinteger(L, 5, 7);
    rectfill(x1, y1, x2, y2, col);
    return 0;
}

// rect(x1, y1, x2, y2, col)
int lua_rect(lua_State* L) {
    int x1 = luaL_checkinteger(L, 1);
    int y1 = luaL_checkinteger(L, 2);
    int x2 = luaL_checkinteger(L, 3);
    int y2 = luaL_checkinteger(L, 4);
    int col = luaL_optinteger(L, 5, 7);
    rect(x1, y1, x2, y2, col);
    return 0;
}

// line(x0, y0, x1, y1, col)
int lua_line(lua_State* L) {
    int x0 = luaL_checkinteger(L, 1);
    int y0 = luaL_checkinteger(L, 2);
    int x1 = luaL_checkinteger(L, 3);
    int y1 = luaL_checkinteger(L, 4);
    int col = luaL_optinteger(L, 5, 7);
    line(x0, y0, x1, y1, col);
    return 0;
}
int lua_pal(lua_State* L) {
    int c0 = luaL_checkinteger(L, 1);
    int c1 = luaL_checkinteger(L, 2);
    pal(c0, c1);
    return 0;
}

int lua_assert_(lua_State* L) {
    int cond = lua_toboolean(L, 1);  // get condition, false if nil or false

    if (!cond) {
        // You can throw a Lua error to stop the game
        // Optionally provide a message
        return luaL_error(L, "Assertion failed!");
    }

    return 0;  // nothing pushed to Lua stack
}
// similarly lua_spr, lua_cls...

void register_lua_functions(lua_State* L) {
  //INPUT
  lua_register(L, "btn", lua_btn);
  lua_register(L, "btnp", lua_btnp);
  //GFX
  lua_register(L, "pset", lua_pset);
  lua_register(L, "cls", lua_cls);
  lua_register(L, "flip", lua_flip);
  lua_register(L, "circfill", lua_circfill);
  lua_register(L, "rectfill", lua_rectfill);
  lua_register(L, "rect", lua_rect);
  lua_register(L, "line", lua_line);
  lua_register(L, "pal", lua_pal);
  // Math
  lua_register(L, "abs", lua_abs);
  lua_register(L, "sin", lua_sin);
  lua_register(L, "cos", lua_cos);
  lua_register(L, "atan2", lua_atan2);//Buggy
  lua_register(L, "flr", lua_flr);
  lua_register(L, "rnd", lua_rnd);
  lua_register(L, "sqrt", lua_sqrt);
  lua_register(L, "sgn", lua_sgn);
  lua_register(L, "max", lua_max);
  lua_register(L, "min", lua_min);
  lua_register(L, "mid", lua_mid);
  lua_register(L, "srand", lua_srand);

  lua_register(L, "assert", lua_assert_);

}