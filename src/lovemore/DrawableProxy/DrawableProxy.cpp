
#include "DrawableProxy.h"

using namespace lovemore;
using love::Proxy;

int DrawableProxy::lua_get_proxy(lua_State* L)
{
	lua_pushlightuserdata(L, &_proxy);
	return 1;
}

void DrawableProxy::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginClass<love::Object>("LoveObject")
	.endClass()
	.deriveClass<DrawableProxy, love::Object>("DrawableProxy")
	.addCFunction("getDrawable", &DrawableProxy::lua_get_proxy)
	.endClass();
}