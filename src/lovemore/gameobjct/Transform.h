
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
		
		void setPosition(float x, float y) { _x = x; _y = y; _dirty = _inverseDirty = true; }
		
		int lua_getPosition(lua_State* L);
		
		float getRotation() { return _rotation; }
		
		void setRotation(float r) { _rotation = r; _dirty = _inverseDirty = true; }
		
		float getScaleX() {return _scaleX; }
		
		float getScaleY() {return _scaleY; }
		
		void setScaleX(float s) { _scaleX = s; _dirty = _inverseDirty = true; }
		
		void setScaleY(float s) { _scaleY = s; _dirty = _inverseDirty = true; }
		
		float getScale() const { assert(_scaleX == _scaleY); return _scaleX; }
		
		void setScale(float s) { _scaleX = s; _scaleY = s; _dirty = _inverseDirty = true; }
		
		void translate(float x, float y) { _x += x; _y += y; _dirty = _inverseDirty = true; }
		
		void rotate(float r) { _rotation += r; _dirty = _inverseDirty = true; }
		
		void apply();
		
		const Matrix4& getMatrix();
		
		const Matrix4& getInverse();
		
		void setParent(Transform* t) { _parent = t; }
		
		Vector& convertToWorldPosition(Vector& localPos);
		
		Vector& convertToLocalPosition(Vector& worldPos);
		
		int lua_convertToWorldPosition(lua_State* L);
		
		int lua_convertToLocalPosition(lua_State* L);
		
		static void registerClassToLua(lua_State* L);
		
	protected:
		
		float	_x = 0;
		float	_y = 0;
		float	_rotation = 0;
		float	_scaleX = 1;
		float	_scaleY = 1;
		
		Matrix4	_matrix;
		Matrix4 _inverse;
		bool	_dirty = false;
		bool	_inverseDirty = false;
		void	updateMatrix();
		void	updateInverse();
		
		Transform* _parent = nullptr;
	};
}

#endif