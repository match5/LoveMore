
#ifndef LOVEMORE_TRANSFORM_H
#define LOVEMORE_TRANSFORM_H

// LOVE
#include "common/Object.h"
#include "common/Vector.h"

//LOVEMORE
#include "Component.h"

namespace lovemore {
	
	using namespace love;
	
	class Transform : public RefCountedObject
	{
	public:
		
		Transform();
		
		virtual ~Transform() override;
		
		Vector getPosition() { return Vector(_x, _y); }
		
		void setPosition(float x, float y) { _x = x; _y = y; }
		
		float getRotation() { return _rotation; }
		
		int lua_getPosition(lua_State* L);
		
		void setRotation(float r) { _rotation = r; }
		
		float getScale() {return _scale; }
		
		void setScale(float s) { _scale = s; }
		
		void translate(float x, float y) { _x += x; _y += y; }
		
		void rotate(float r) { _rotation += r; }
		
		void apply();
		
		static void registerClassToLua(luabridge::Namespace& ns);
		
	protected:
		
		float	_x = 0;
		float	_y = 0;
		float	_rotation = 0;
		float	_scale = 1;
	};
}

#endif