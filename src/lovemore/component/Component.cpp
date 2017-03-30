
#include "gameobject/GameObject.h"
#include "Component.h"

using namespace love;
using namespace lovemore;

Component::Component()
{
}

Component::~Component()
{
}

void Component::setDrawOrder(int order)
{
	if (_drawOrder != order)
	{
		_drawOrder = order;
		if (_owner) {
			_owner->setComDrawOrderDirty(true);
		}
	}
}

void Component::doUpdate(float dt)
{
	if (!_started)
	{
		start();
		_started = true;
	}
	else
	{
		update(dt);
	}
}

void Component::start()
{
}

void Component::update(float /*dt*/)
{
}

void Component::end()
{
}

void Component::draw(GLGraphics* /*g*/)
{
}

void Component::preDraw(GLGraphics* /*g*/)
{
}

void Component::postDraw(GLGraphics* /*g*/)
{
}

void Component::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.beginClass<Component>("Component")
	.addProperty("owner", &Component::getOwner)
	.addProperty("drawOrder", &Component::getDrawOrder, &Component::setDrawOrder)
	.addData("enabled", &Component::_enabled)
	.endClass();
}
