
#include "gameobject/GameObject.h"
#include "LuaComponent.h"

using namespace love;
using namespace lovemore;


LuaComponent::LuaComponent(luabridge::LuaRef& imp)
:_luaImp(imp)
,_fnStart(imp.state())
,_fnUpdate(imp.state())
,_fnFinish(imp.state())
,_fnDraw(imp.state())
,_fnPreDraw(imp.state())
,_fnPostDraw(imp.state())
{
	assert(_luaImp.isTable() && "lua imp must be a table");
	
	auto name = _luaImp["name"];
	if (name.isString()) {
		_name = name.cast<std::string>();
	}
}

LuaComponent::~LuaComponent()
{
}

void LuaComponent::preLoadImp()
{
	_fnStart = _luaImp["start"];
	_fnUpdate = _luaImp["update"];
	_fnFinish = _luaImp["finish"];
	_fnDraw = _luaImp["draw"];
	_fnPreDraw = _luaImp["preDraw"];
	_fnPostDraw = _luaImp["postDraw"];
	
	_luaImp["owner"] = _owner;
	_luaImp["transform"] = _owner->getTransform();
	_luaImp["component"] = this;
}

void LuaComponent::start()
{
	preLoadImp();
	if (_fnStart.isFunction())
	{
		try {
			_fnStart(_luaImp);
		} catch (std::exception& e) {
			luaL_error(_luaImp.state(), e.what());
		}
	}
}

void LuaComponent::update(float dt)
{
	if (_fnUpdate.isFunction())
	{
		try {
			_fnUpdate(_luaImp, dt);
		} catch (std::exception& e) {
			luaL_error(_luaImp.state(), e.what());
		}
	}
}

void LuaComponent::end()
{
	if (_started && _fnFinish.isFunction())
	{
		try {
			_fnFinish(_luaImp);
		} catch (std::exception& e) {
			luaL_error(_luaImp.state(), e.what());
		}
	}
}

void LuaComponent::draw(GLGraphics* /*g*/)
{
	if (_fnDraw.isFunction())
	{
		try {
			_fnDraw(_luaImp);
		} catch (std::exception& e) {
			luaL_error(_luaImp.state(), e.what());
		}
	}
}

void LuaComponent::preDraw(GLGraphics* /*g*/)
{
	if (_fnPreDraw.isFunction())
	{
		try {
			_fnPreDraw(_luaImp);
		} catch (std::exception& e) {
			luaL_error(_luaImp.state(), e.what());
		}
	}
}

void LuaComponent::postDraw(GLGraphics* /*g*/)
{
	if (_fnPostDraw.isFunction())
	{
		try {
			_fnPostDraw(_luaImp);
		} catch (std::exception& e) {
			luaL_error(_luaImp.state(), e.what());
		}
	}
}

void LuaComponent::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.deriveClass<LuaComponent, Component>("LuaComponent")
	.addStaticFunction("castFrom", &Component::castFrom<LuaComponent>)
	.addProperty("script", &LuaComponent::getScript)
	.endClass();
}
