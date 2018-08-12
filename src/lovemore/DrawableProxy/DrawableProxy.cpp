
#include "DrawableProxy.h"

using namespace lovemore;
using love::Proxy;

int DrawableProxy::lua_get_proxy(lua_State* L)
{
	Proxy *proxy = (Proxy *)lua_newuserdata(L, sizeof(Proxy));
	proxy->type = love::Type::GRAPHICS_DRAWABLE_ID;
	proxy->object = this;
	luaL_newmetatable(L, name());
	lua_setmetatable(L, -2);
	return 1;
}

void DrawableProxy::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginClass<DrawableProxy>("DrawableProxy")
	//.addData("drawable", &DrawableProxy::_proxy)
	.addCFunction("getDrawable", &DrawableProxy::lua_get_proxy)
	.endClass();
}