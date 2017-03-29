
#ifndef LOVEMORE_LUA_COMPONENT_H
#define LOVEMORE_LUA_COMPONENT_H

//LOVEMORE
#include "Component.h"

namespace lovemore {
	
	using namespace love;
	
	class GameObject;
	
	class Transform;
	
	class LuaComponent : public Component
	{
	public:
		
		LuaComponent(luabridge::LuaRef& imp);
		
		virtual ~LuaComponent() override;
		
		void preLoadImp();
		
		luabridge::LuaRef getScript() const { return _luaImp; }
		
		virtual void start() override;
		
		virtual void update(float dt) override;
		
		virtual void end() override;
		
		virtual void draw(GLGraphics* g) override;
		virtual void preDraw(GLGraphics* g) override;
		virtual void postDraw(GLGraphics* g) override;
		
		virtual const char* getName() const override { return _name.c_str(); }
		
		static void registerClassToLua(lua_State* L);
		
	protected:
		
	private:
		
		luabridge::LuaRef	_luaImp;
		
		luabridge::LuaRef	_fnStart;
		luabridge::LuaRef	_fnUpdate;
		luabridge::LuaRef	_fnFinish;
		luabridge::LuaRef	_fnDraw;
		luabridge::LuaRef	_fnPreDraw;
		luabridge::LuaRef	_fnPostDraw;
		
		std::string			_name;
	};
}

#endif