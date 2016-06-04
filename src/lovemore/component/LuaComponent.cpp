
#include "GameObject.h"
#include "LuaComponent.h"

using namespace love;
using namespace lovemore;

LuaComponent::LuaComponent(luabridge::LuaRef& imp)
:_luaImp(imp)
,_fnStart(imp.state())
,_fnUpdate(imp.state())
,_fnEnd(imp.state())
,_fnDraw(imp.state())
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
	_fnEnd = _luaImp["end"];
	_fnDraw = _luaImp["draw"];
	
	_luaImp["owner"] = _owner;
	_luaImp["transform"] = _owner->getTransform();
	_luaImp["component"] = this;
}

void LuaComponent::start()
{
	preLoadImp();
	if (_fnStart)
	{
		_fnStart(_luaImp);
	}
}

void LuaComponent::update(float dt)
{
	if (_fnUpdate)
	{
		_fnUpdate(_luaImp, dt);
	}
}

void LuaComponent::end()
{
	if (_fnEnd)
	{
		_fnEnd(_luaImp);
	}
}

void LuaComponent::draw(GLGraphics* /*g*/)
{
	if (_fnDraw)
	{
		_fnDraw(_luaImp);
	}
}

void LuaComponent::registerClassToLua(luabridge::Namespace& ns)
{
	ns.deriveClass<LuaComponent, Component>("LuaComponent")
	.addProperty("script", &LuaComponent::getScript)
	.endClass();
}
