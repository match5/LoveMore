// LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/RefCountedPtr.h"

// LOVE
#include "common/config.h"
#include "common/version.h"
#include "common/runtime.h"
#include "common/wrap_Data.h"

// LOVE MORE
#include "lovemore.h"
#include "SpineAnimator/SpineAnimator.h"

// C++
#include <string>
#include <vector>
#include <list>

using namespace love;
using namespace lovemore;

static lua_State* _L = nullptr;

int lovemore_newSpineAnimator(lua_State* L)
{
	const char* skeletonDataFile =  lua_tostring(L, 1);
	const char* atlasFile =  lua_tostring(L, 2);
	SpineAnimator* spine = new SpineAnimator(skeletonDataFile, atlasFile);
	luabridge::push(L, spine);
	return 1;
}

int luaopen_lovemore(lua_State *L)
{
	_L = L;
	
	luabridge::getGlobalNamespace(L)
		.beginNamespace("lovemore")
			.addCFunction("newSpineAnimator", lovemore_newSpineAnimator)
		.endNamespace();
	
	SpineAnimator::registerClassToLua(L);
	
	return 0;
}

lua_State* lovemore_getLuaState() {
	return _L;
}


