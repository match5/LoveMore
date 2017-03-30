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
#include "gameobject/GameObject.h"
#include "gameobject/Transform.h"
#include "component/Component.h"
#include "component/LuaComponent.h"
#include "Component/Renderer.h"
#include "component/SpriteRenderer.h"
#include "component/SpineAnimator/SpineAnimator.h"

// C++
#include <string>
#include <vector>
#include <list>

using namespace love;
using namespace lovemore;

static lua_State* _L = nullptr;

static GameObject* stage = nullptr;

int lovemore_newGameObject(lua_State *L)
{
	GameObject* obj = new GameObject();
	luabridge::push(L, obj);
	return 1;
};

int lovemore_newComponent(lua_State* L)
{
	luabridge::LuaRef imp = luabridge::Stack<luabridge::LuaRef>::get(L, 1);
	if (imp.isFunction())
	{
		try {
			imp = imp();
		} catch (std::exception& e) {
			luaL_error(L, e.what());
		}
	}
	if (!imp.isTable()) {
		luaL_error(L, "Invalid arg type %s, need table or function returns a table", lua_typename(L, imp.type()));
	}
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
	SpriteRenderer* sr = new SpriteRenderer;
	sr->setTexture(t);
	sr->setQuad(q);
	
	GameObject* obj = new GameObject;
	obj->addComponent(sr);
	
	luabridge::push(L, obj);
	luabridge::push(L, sr);
	return 2;
}

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
	
	stage = new GameObject;
	
	luabridge::getGlobalNamespace(L)
		.beginNamespace("lovemore")
			.addVariable("stage", &stage, false)
			.addCFunction("newGameObject", lovemore_newGameObject)
			.addCFunction("newComponent", lovemore_newComponent)
			.addCFunction("newSpriteRenderer", lovemore_newSpriteRenderer)
			.addCFunction("newSprite", lovemore_newSprite)
			.addCFunction("newSpineAnimator", lovemore_newSpineAnimator)
		.endNamespace();
	
	GameObject::registerClassToLua(L);
	Transform::registerClassToLua(L);
	Component::registerClassToLua(L);
	LuaComponent::registerClassToLua(L);
	Renderer::registerClassToLua(L);
	SpriteRenderer::registerClassToLua(L);
	SpineAnimator::registerClassToLua(L);
	
	return 0;
}

lua_State* lovemore_getLuaState() {
	return _L;
}


