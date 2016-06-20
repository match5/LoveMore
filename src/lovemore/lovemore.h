
#ifndef LOVE_LOVEMORE_H
#define LOVE_LOVEMORE_H

// LOVE
#include "common/config.h"

// Forward declare lua_State.
struct lua_State;

LOVE_EXPORT int luaopen_lovemore(lua_State *L);

lua_State* lovemore_getLuaState();

#endif // LOVE_LOVE_MORE_H
