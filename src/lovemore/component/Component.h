
#ifndef LOVEMORE_COMPONENT_H
#define LOVEMORE_COMPONENT_H

// LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "LuaBridge.h"
#include "RefCountedObject.h"
#include "RefCountedPtr.h"

// LOVE
#include "common/Object.h"
#include "modules/graphics/opengl/Graphics.h"

namespace lovemore {
	
	using namespace love;
	
	typedef love::graphics::opengl::Graphics GLGraphics;
	
	class GameObject;
	
	class Component : public RefCountedObject
	{
	public:
		
		Component();
		
		virtual ~Component();
		
		const GameObject* getOwner() const { return _owner; }
		
		void setOwner(GameObject* obj) { _owner = obj; }
		
		void setEnabled(bool b) { _enabled = b; }
		
		bool isEnabled() const { return _enabled; }
		
		int getDrawOrder() const { return _drawOrder; }
		
		void setDrawOrder(int order);
		
		void doUpdate(float dt);
		
		virtual void start();
		
		virtual void update(float dt);
		
		virtual void end();
		
		virtual void draw(GLGraphics *g);
		
		virtual const char* getName() const { return ""; };
		
		static void registerClassToLua(luabridge::Namespace& ns);
		
	protected:
		
		GameObject* _owner = nullptr;
		bool		_enabled = true;
		bool		_started = false;
		
		int			_drawOrder = 0;
	};
}

#endif