
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

const Matrix4 &Transform::getMatrix()
{
	updateMatrix();
	return _matrix;
}

const Matrix4 &Transform::getInverse()
{
	updateInverse();
	return _inverse;
}

void Transform::updateMatrix()
{
	if (_dirty)
	{
		_matrix.setIdentity();
		_matrix.translate(_x, _y);
		_matrix.rotate(LOVE_TORAD(_rotation));
		_matrix.scale(_scaleX, _scaleY);
		_dirty = false;
	}
}


void Transform::updateInverse()
{
	if (_inverseDirty)
	{
		_inverse.setIdentity();
		_inverse.scale(1 / _scaleX, 1 / _scaleY);
		_inverse.rotate(LOVE_TORAD(-_rotation));
		_inverse.translate(-_x, -_y);
		_inverseDirty = false;
	}
}

Vector& Transform::convertToWorldPosition(Vector& localPos)
{
	getMatrix().transform(&localPos, &localPos, 1);
	if (_parent)
	{
		_parent->convertToWorldPosition(localPos);
	}
	return localPos;
}

Vector& Transform::convertToLocalPosition(Vector& worldPos)
{
	if (_parent) {
		_parent->convertToLocalPosition(worldPos);
	}
	getInverse().transform(&worldPos, &worldPos, 1);
	
	return worldPos;
}


int Transform::lua_convertToWorldPosition(lua_State* L)
{
	Vector pos;
	pos.x = lua_tonumber(L, 2);
	pos.y = lua_tonumber(L, 3);
	
	convertToWorldPosition(pos);
	
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);
	
	return 2;
}

int Transform::lua_convertToLocalPosition(lua_State* L)
{
	Vector pos;
	pos.x = lua_tonumber(L, 2);
	pos.y = lua_tonumber(L, 3);
	
	convertToLocalPosition(pos);
	
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);
	
	return 2;
}

void Transform::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginClass<Transform>("Transform")
	.addCFunction("getPosition", &Transform::lua_getPosition)
	.addCFunction("convertToWorldPosition", &Transform::lua_convertToWorldPosition)
	.addCFunction("convertToLocalPosition", &Transform::lua_convertToLocalPosition)
	.addFunction("setPosition", &Transform::setPosition)
	.addFunction("translate", &Transform::translate)
	.addFunction("rotate", &Transform::rotate)
	.addProperty("scale", &Transform::getScale, &Transform::setScale)
	.addData("x", &Transform::_x)
	.addData("y", &Transform::_y)
	.addData("rotation", &Transform::_rotation)
	.addData("scaleX", &Transform::_scaleX)
	.addData("scaleY", &Transform::_scaleY)
	.endClass();
}
