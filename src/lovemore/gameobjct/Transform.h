
#ifndef LOVEMORE_TRANSFORM_H
#define LOVEMORE_TRANSFORM_H

// LOVE
#include "common/Object.h"
#include "common/Vector.h"

//LOVEMORE
#include "component/Component.h"
#include "LuaBridge/RefCountedObject.h"

namespace lovemore {
	
	using namespace love;
	
	class Transform : public RefCountedObject
	{
	public:
		
		Transform();
		
		virtual ~Transform() override;
		
		Vector getPosition() { return Vector(_x, _y); }
		
		void setPosition(float x, float y) { _x = x; _y = y; }
		
		int lua_getPosition(lua_State* L);
		
		float getRotation() { return _rotation; }
		
		void setRotation(float r) { _rotation = r; }
		
		float getScaleX() {return _scaleX; }
		
		float getScaleY() {return _scaleY; }
		
		void setScaleX(float s) { _scaleX = s; }
		
		void setScaleY(float s) { _scaleY = s; }
		
		float getScale() const { assert(_scaleX == _scaleY); return _scaleX; }
		
		void setScale(float s) { _scaleX = s; _scaleY = s; }
		
		void translate(float x, float y) { _x += x; _y += y; }
		
		void rotate(float r) { _rotation += r; }
		
		void apply();
		
		static void registerClassToLua(lua_State* L);
		
	protected:
		
		float	_x = 0;
		float	_y = 0;
		float	_rotation = 0;
		float	_scaleX = 1;
		float	_scaleY = 1;
	};
}

#endif