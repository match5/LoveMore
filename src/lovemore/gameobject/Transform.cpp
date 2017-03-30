
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
	if (_rotation * _rotation > 0.001 )
	{
		g->rotate(LOVE_TORAD(_rotation));
	}
	if (_scaleX * _scaleX + _scaleY * _scaleY > 0.001)
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
	if (_matrixDirty)
	{
		_matrix.setIdentity();
		_matrix.translate(_x, _y);
		_matrix.rotate(LOVE_TORAD(_rotation));
		_matrix.scale(_scaleX, _scaleY);
		_matrixDirty = false;
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

Vector* Transform::convertToWorldPosition(Vector* localPos, int n)
{
	getMatrix().transform(localPos, localPos, n);
	if (_parent)
	{
		_parent->convertToWorldPosition(localPos, n);
	}
	return localPos;
}

Vector* Transform::convertToLocalPosition(Vector* worldPos, int n)
{
	if (_parent)
	{
		_parent->convertToLocalPosition(worldPos, n);
	}
	getInverse().transform(worldPos, worldPos, n);
	return worldPos;
}


int Transform::lua_convertToWorldPosition(lua_State* L)
{
	int n = (lua_gettop(L) - 1) / 2;
	n = n > 4 ? 4 : n;
	Vector pos[4];
	for (int i = 0; i < n; ++i) {
		pos[i].x = lua_tonumber(L, i * 2 + 2);
		pos[i].y = lua_tonumber(L, i * 2 + 3);
	}
	
	convertToWorldPosition(pos, n);
	
	for (int i = 0; i < n; ++i) {
		lua_pushnumber(L, pos[i].x);
		lua_pushnumber(L, pos[i].y);
	}
	
	return n * 2;
}

int Transform::lua_convertToLocalPosition(lua_State* L)
{
	int n = (lua_gettop(L) - 1) / 2;
	n = n > 4 ? 4 : n;
	Vector pos[4];
	for (int i = 0; i < n; ++i) {
		pos[i].x = lua_tonumber(L, i * 2 + 2);
		pos[i].y = lua_tonumber(L, i * 2 + 3);
	}
	
	convertToLocalPosition(pos, n);
	
	for (int i = 0; i < n; ++i) {
		lua_pushnumber(L, pos[i].x);
		lua_pushnumber(L, pos[i].y);
	}
	
	return n * 2;
}

void Transform::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginClass<Transform>("Transform")
	.addCFunction("convertToWorldPosition", &Transform::lua_convertToWorldPosition)
	.addCFunction("convertToLocalPosition", &Transform::lua_convertToLocalPosition)
	.addCFunction("getPosition", &Transform::lua_getPosition)
	.addFunction("setPosition", &Transform::setPosition)
	.addFunction("translate", &Transform::translate)
	.addFunction("rotate", &Transform::rotate)
	.addProperty("x", &Transform::getPositionX, &Transform::setPositionX)
	.addProperty("y", &Transform::getPositionY, &Transform::setPositionY)
	.addProperty("rotation",  &Transform::getRotation, &Transform::setRotation)
	.addProperty("scale", &Transform::getScale, &Transform::setScale)
	.addProperty("scaleX", &Transform::getScaleX, &Transform::setScaleX)
	.addProperty("scaleY", &Transform::getScaleY, &Transform::setScaleY)
	.endClass();
}
