// LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "LuaBridge.h"
#include "RefCountedPtr.h"

// LOVE
#include "common/config.h"
#include "common/version.h"
#include "common/runtime.h"
#include "common/wrap_Data.h"

// LOVE MORE
#include "lovemore.h"
#include "gameobjct/GameObject.h"
#include "gameobjct/Transform.h"
#include "component/Component.h"
#include "component/LuaComponent.h"
#include "component/SpriteRenderer.h"

// C++
#include <string>
#include <vector>
#include <list>

using namespace love;
using namespace lovemore;

static lua_State* _L = nullptr;

static GameObject* root = nullptr;

int lovemore_newGameObject(lua_State *L)
{
	GameObject* obj = new GameObject();
	root->addChild(obj);
	luabridge::push(L, obj);
	return 1;
};

int lovemore_newComponent(lua_State* L)
{
	luabridge::LuaRef imp = luabridge::Stack<luabridge::LuaRef>::get(L, 1);
	LuaComponent* com = new LuaComponent(imp);
	luabridge::push(L, com);
	return 1;
}

int lovemore_newSpriteRenderer(lua_State* L)
{
	SpriteRenderer* r = new SpriteRenderer;
	luabridge::push(L, r);
	return 1;
}

int lovemore_newSprite(lua_State* L)
{
	graphics::Texture* t = luax_checktype<graphics::Texture>(L, 1, GRAPHICS_TEXTURE_ID);
	graphics::Quad* q = nullptr;
	if (luax_istype(L, 2, GRAPHICS_QUAD_ID))
	{
		q = luax_checktype<graphics::Quad>(L, 2, GRAPHICS_QUAD_ID);
	}
	SpriteRenderer* r = new SpriteRenderer;
	r->setTexture(t);
	r->setQuad(q);
	
	GameObject* obj = new GameObject;
	obj->addComponent(r);
	root->addChild(obj);
	
	luabridge::push(L, obj);
	return 1;
}

int lovemore_clearGameObjects(lua_State *)
{
	root->removeAllChildren();
	return 0;
}

int lovemore_update(lua_State *L)
{
	float dt = lua_tonumber(L, 1);
	root->update(dt);
	return 0;
}

int lovemore_draw(lua_State *)
{
	root->draw();
	return 0;
}

int luaopen_lovemore(lua_State *L)
{
	_L = L;
	
	auto ns = luabridge::getGlobalNamespace(L)
		.beginNamespace("lovemore")
			.addCFunction("open", luaopen_lovemore)
			.addCFunction("newGameObject", lovemore_newGameObject)
			.addCFunction("clearGameObjects", lovemore_clearGameObjects)
			.addCFunction("update", lovemore_update)
			.addCFunction("draw", lovemore_draw)
			.addCFunction("newComponent", lovemore_newComponent)
			.addCFunction("newSpriteRenderer", lovemore_newSpriteRenderer)
			.addCFunction("newSprite", lovemore_newSprite)
		.endNamespace();
	
	GameObject::registerClassToLua(ns);
	Transform::registerClassToLua(ns);
	Component::registerClassToLua(ns);
	LuaComponent::registerClassToLua(ns);
	SpriteRenderer::registerClassToLua(ns);
	
	root = new GameObject;
	
	return 0;
}

lua_State* lovemore_getLuaState() {
	return _L;
}


