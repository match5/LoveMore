
// LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "LuaBridge.h"

//LOVE
#include "modules/graphics/opengl/Graphics.h"

// LOVEMORE
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

using namespace love;
using namespace lovemore;

GameObject::GameObject()
{
	_components.reserve(4);
	_transform = new Transform;
}

GameObject::~GameObject()
{
	clearComponents();
	removeAllChildren();
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
		obj->removeFormParent();
	}
	obj->_parent = this;
}

void GameObject::setParent(GameObject* obj)
{
	if (obj)
	{
		obj->addChild(this);
	}
	else if (_parent)
	{
		removeFormParent();
	}
}

GameObject* GameObject::getParent() const
{
	return _parent;
}

void GameObject::removeChild(GameObject* child)
{
	auto newEnd = std::remove_if(_children.begin(), _children.end(), [&child](ChildrenVec::value_type& c){
		return child == c.get();
	});
	if (newEnd != _children.end()) {
		child->_parent = nullptr;
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

void GameObject::removeFormParent()
{
	_parent->removeChild(this);
}

void GameObject::update(float dt)
{
	if (_isActive)
	{
		for (auto com : _components)
		{
			if (com.get() && com->isEnabled())
			{
				com->doUpdate(dt);
			}
		}
		for (auto child : _children)
		{
			if (child->isActive())
			{
				child->update(dt);
			}
		}
	}
}

void GameObject::draw()
{
	if (_isVisible)
	{
		//sort children;
		if (_comDrawOrderDirty) {
			std::stable_sort(_children.begin(), _children.end(), [](const ChildrenVec::value_type& a, const ChildrenVec::value_type& b) {
				return a->getZOreder() < b->getZOreder();
			});
			setChildrenZOrderDirty(false);
		}
		
		//apply self transform
		graphics::opengl::Graphics *g = Module::getInstance<graphics::opengl::Graphics>(Module::M_GRAPHICS);
		g->push();
		_transform->apply();
		
		//draw children zOrder < 0
		auto it = _children.begin();
		for (; it != _children.end() && it->get()->getZOreder() < 0; ++it)
		{
			it->get()->draw();
		}
		
		//sort components
		if (_comDrawOrderDirty)
		{
			std::stable_sort(_components.begin(), _components.end(), [](const ComponentVec::value_type& a, const ComponentVec::value_type& b) {
				return a->getDrawOrder() < b->getDrawOrder();
			});
			setComDrawOrderDirty(false);
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
		
		g->pop();
	}
}

void GameObject::setZOreder(int zOrder)
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

void GameObject::registerClassToLua(luabridge::Namespace& ns)
{
	ns.beginClass<GameObject>("GameObject")
	.addFunction("addComponent", &GameObject::addComponent)
	.addFunction("getComponent", &GameObject::getComponent)
	.addFunction("addChild", &GameObject::addChild)
	.addFunction("removeChild", &GameObject::removeChild)
	.addFunction("removeAllChildren", &GameObject::removeAllChildren)
	.addFunction("removeFormParent", &GameObject::removeFormParent)
	.addProperty("parent", &GameObject::getParent, &GameObject::setParent)
	.addProperty("transform", &GameObject::getTransform)
	.addProperty("zOrder", &GameObject::getZOreder, &GameObject::setZOreder)
	.addData("isActive", &GameObject::_isActive)
	.addData("isVisible", &GameObject::_isVisible)
	.endClass();
}
