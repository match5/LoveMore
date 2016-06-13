
#include "Renderer.h"

using namespace love;
using namespace lovemore;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

int Renderer::lua_setColor(lua_State* L)
{
	Colorf c;
	if (lua_istable(L, 2))
	{
		for (int i = 1; i <= 4; i++)
			lua_rawgeti(L, 2, i);
		
		c.r = (float) luaL_checknumber(L, -4);
		c.g = (float) luaL_checknumber(L, -3);
		c.b = (float) luaL_checknumber(L, -2);
		c.a = (float) luaL_optnumber(L, -1, 255);
		
		lua_pop(L, 4);
	}
	else
	{
		c.r = (float) luaL_checknumber(L, 2);
		c.g = (float) luaL_checknumber(L, 3);
		c.b = (float) luaL_checknumber(L, 4);
		c.a = (float) luaL_optnumber(L, 5, 255);
	}
	setColor(c);
	return 0;
}

int Renderer::lua_getColor(lua_State* L)
{
	lua_pushnumber(L, _color.r);
	lua_pushnumber(L, _color.g);
	lua_pushnumber(L, _color.b);
	lua_pushnumber(L, _color.a);
	return 4;
}

void Renderer::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.deriveClass<Renderer, Component>("Renderer")
	.addCFunction("setColor", &Renderer::lua_setColor)
	.addCFunction("getColor", &Renderer::lua_getColor)
	.addProperty("alpha", &Renderer::getAlpha, &Renderer::setAlpha)
	.addData("flipX", &Renderer::_flipX)
	.addData("flipY", &Renderer::_flipY)
	.endClass();
}