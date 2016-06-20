
#include "common/runtime.h"

#include "SpriteRenderer.h"
#include "gameobjct/GameObject.h"
#include "gameobjct/Transform.h"

using namespace love;
using namespace lovemore;

SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::~SpriteRenderer()
{
}

int SpriteRenderer::lua_setTexture(lua_State *L)
{
	Texture* t = luax_checktype<Texture>(L, 2, GRAPHICS_TEXTURE_ID);
	_texture = t;
	_quad = nullptr;
	return 0;
}

int SpriteRenderer::lua_setTextureAndQuad(lua_State *L)
{
	Texture* t = luax_checktype<Texture>(L, 2, GRAPHICS_TEXTURE_ID);
	Quad* q = luax_checktype<Quad>(L, 3, GRAPHICS_QUAD_ID);
	_texture = t;
	_quad = q;
	return 0;
}

int SpriteRenderer::lua_getAnchor(lua_State* L)
{
	lua_pushnumber(L, _anchorX);
	lua_pushnumber(L, _anchorY);
	return 2;
}

void SpriteRenderer::draw(GLGraphics* g)
{
	if (_texture)
	{
		g->push();
		g->scale(_flipX? -1 : 1,_flipY ? -1 : 1);
		g->setColor(_color);
		if (_quad)
		{
			Quad::Viewport vp = _quad->getViewport();
			_texture->drawq(_quad, 0, 0, 0, 1, 1, _anchorX * vp.w, _anchorY * vp.h, 0, 0);
		}
		else
		{
			_texture->draw(0, 0, 0, 1, 1, _anchorX * _texture->getWidth(), _anchorY * _texture->getHeight(), 0, 0);
		}
		g->pop();
	}
}

void SpriteRenderer::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.deriveClass<SpriteRenderer, Renderer>("SpriteRenderer")
	.addStaticFunction("castFrom", &Component::castFrom<SpriteRenderer>)
	.addCFunction("setTexture", &SpriteRenderer::lua_setTexture)
	.addCFunction("setTextureAndQuad", &SpriteRenderer::lua_setTextureAndQuad)
	.addFunction("setAnchor", &SpriteRenderer::setAnchor)
	.addCFunction("getAnchor", &SpriteRenderer::lua_getAnchor)
	.endClass();
}
