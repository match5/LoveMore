#ifndef LOVEMORE_LUA_OBJECT_H
#define LOVEMORE_LUA_OBJECT_H

// LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/RefCountedObject.h"
#include "LuaBridge/RefCountedPtr.h"

namespace lovemore {
	typedef RefCountedObject LuaObject;
}

#endif
