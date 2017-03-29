
// LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

//LOVE
#include "modules/graphics/opengl/Graphics.h"

// LOVEMORE
#include "GameObject.h"
#include "component/Component.h"
#include "Transform.h"
#include "LuaBridge/LuaBridge.h"

// C++
#include <algorithm>

using namespace love;
using namespace lovemore;

unsigned int GameObject::_nextId = 1;

GameObject::GameObject()
:_id(_nextId++)
{
	_components.reserve(4);
	_transform = new Transform;
}

GameObject::~GameObject()
{
	clearComponents();
}

void GameObject::addComponent(Component* com)
{
	_components.push_back(com);
	com->setOwner(this);
	setComDrawOrderDirty(true);
}

Component* GameObject::getComponent(const std::string& name)
{
	auto it = std::find_if(_components.begin(), _components.end(), [&name](ComponentVec::value_type &com){
		return strcmp(name.c_str(), com->getName()) == 0;
	});
	if (it != _components.end())
	{
		return it->get();
	}
	return nullptr;
}

void GameObject::clearComponents()
{
	for (auto com : _components)
	{
		com->end();
	}
	_components.clear();
}

void GameObject::addChild(GameObject* obj)
{
	_children.push_back(obj);
	if (obj->_parent != nullptr)
	{
		obj->removeFromParent();
	}
	obj->_parent = this;
	obj->_transform->setParent(_transform.get());
}

void GameObject::setParent(GameObject* obj)
{
	if (obj)
	{
		obj->addChild(this);
	}
	else if (_parent)
	{
		removeFromParent();
	}
}

void GameObject::removeChild(GameObject* child)
{
	auto newEnd = std::remove_if(_children.begin(), _children.end(), [&child](ChildrenVec::value_type& c)
	{
		return child == c.get();
	});
	if (newEnd != _children.end())
	{
		child->_parent = nullptr;
		child->_transform->setParent(nullptr);
	}
	_children.erase(newEnd, _children.end());
}

void GameObject::removeAllChildren()
{
	for (auto child : _children)
	{
		child->_parent = nullptr;
	}
	_children.clear();
}

void GameObject::removeFromParent()
{
	if (_parent)
	{
		_parent->removeChild(this);
	}
}


int GameObject::lua_getChild(lua_State* L)
{
	int index = lua_tointeger(L, 2) - 1;
	if (0 <= index && index < (int)_children.size())
	{
		GameObject* child = getChild(index);
		luabridge::Stack<GameObject*>::push(L, child);
		return 1;
	}
	return 0;
}

void GameObject::update(float dt)
{
	if (_isActive)
	{
		for (auto com : _components)
		{
			if (com->isEnabled())
			{
				com->doUpdate(dt);
			}
		}
		for (auto child : _children)
		{
			child->update(dt);
		}
	}
}

void GameObject::draw()
{
	if (_isVisible)
	{
		//sort children;
		if (_comDrawOrderDirty)
		{
			std::stable_sort(_children.begin(), _children.end(), [](const ChildrenVec::value_type& a, const ChildrenVec::value_type& b) {
				return a->getZOrder() < b->getZOrder();
			});
			setChildrenZOrderDirty(false);
		}
		
		//sort components
		if (_comDrawOrderDirty)
		{
			std::stable_sort(_components.begin(), _components.end(), [](const ComponentVec::value_type& a, const ComponentVec::value_type& b) {
				return a->getDrawOrder() < b->getDrawOrder();
			});
			setComDrawOrderDirty(false);
		}
		
		//apply self transform
		graphics::opengl::Graphics *g = Module::getInstance<graphics::opengl::Graphics>(Module::M_GRAPHICS);
		g->push();
		_transform->apply();
		
		//preDraw
		for(auto com : _components)
		{
			if (com.get() && com->isEnabled())
			{
				com->preDraw(g);
			}
		}
		
		//draw children zOrder < 0
		auto it = _children.begin();
		for (; it != _children.end() && it->get()->getZOrder() < 0; ++it)
		{
			it->get()->draw();
		}
		
		//draw self
		for(auto com : _components)
		{
			if (com.get() && com->isEnabled())
			{
				com->draw(g);
			}
		}
		
		//draw children zOrder >= 0
		for (; it != _children.end(); ++it)
		{
			it->get()->draw();
		}
		
		//postDraw
		for(auto com : _components)
		{
			if (com.get() && com->isEnabled())
			{
				com->postDraw(g);
			}
		}
		
		g->pop();
	}
}

void GameObject::setZOrder(int zOrder)
{
	if (_zOrder != zOrder)
	{
		_zOrder = zOrder;
		if (_parent)
		{
			_parent->setChildrenZOrderDirty(true);
		}
	}
}

void GameObject::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginClass<GameObject>("GameObject")
	.addFunction("addComponent", &GameObject::addComponent)
	.addFunction("getComponent", &GameObject::getComponent)
	.addFunction("addChild", &GameObject::addChild)
	.addFunction("removeChild", &GameObject::removeChild)
	.addFunction("removeAllChildren", &GameObject::removeAllChildren)
	.addFunction("removeFromParent", &GameObject::removeFromParent)
	.addFunction("getChildrenNum", &GameObject::getChildrenNum)
	.addCFunction("getChild", &GameObject::lua_getChild)
	.addProperty("parent", &GameObject::getParent, &GameObject::setParent)
	.addProperty("transform", &GameObject::getTransform)
	.addProperty("zOrder", &GameObject::getZOrder, &GameObject::setZOrder)
	.addData("active", &GameObject::_isActive)
	.addData("visible", &GameObject::_isVisible)
	.addData("id", &GameObject::_id)
	.endClass();
}
