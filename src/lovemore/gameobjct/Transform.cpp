
#include "common/math.h"

#include "Transform.h"

using namespace love;
using namespace lovemore;

Transform::Transform()
{
}

Transform::~Transform()
{
}

int Transform::lua_getPosition(lua_State* L)
{
	lua_pushnumber(L, _x);
	lua_pushnumber(L, _y);
	return 2;
}

void Transform::apply()
{
	GLGraphics *g = Module::getInstance<GLGraphics>(Module::M_GRAPHICS);
	if (_x * _x + _y * _y > 0.001)
	{
		g->translate(_x, _y);
	}
	if (-0.001 > _rotation || _rotation > 0.001 )
	{
		g->rotate(LOVE_TORAD(_rotation));
	}
	if (-0.001 > _scaleX || _scaleX > 0.001 || -0.001 > _scaleY || _scaleY > 0.001 )
	{
		g->scale(_scaleX, _scaleY);
	}
}

void Transform::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginClass<Transform>("Transform")
	.addCFunction("getPosition", &Transform::lua_getPosition)
	.addFunction("setPosition", &Transform::setPosition)
	.addFunction("translate", &Transform::translate)
	.addFunction("rotate", &Transform::rotate)
	.addData("x", &Transform::_x)
	.addData("y", &Transform::_y)
	.addData("rotation", &Transform::_rotation)
	.addData("scaleX", &Transform::_scaleX)
	.addData("scaleY", &Transform::_scaleY)
	.endClass();
}
