
#ifndef LOVEMORE_GAME_OBJECT_H
#define LOVEMORE_GAME_OBJECT_H

// LOVE
#include "common/Object.h"
#include "common/Vector.h"
#include "modules/graphics/Texture.h"

//LOVEMORE
#include "component/Component.h"
#include "LuaBridge/RefCountedObject.h"
#include "LuaBridge/RefCountedPtr.h"

// C++
#include <vector>
#include <unordered_map>


namespace lovemore {
	
	using namespace love;
	
	class Transform;
	
	class GameObject : public RefCountedObject
	{
	public:
		
		typedef std::vector<RefCountedPtr<Component>> ComponentVec;
		
		typedef std::vector<RefCountedPtr<GameObject>> ChildrenVec;
		
		GameObject();
		
		virtual ~GameObject();
		
		void addComponent(Component* com);
		
		Component* getComponent(const std::string& name);
		
		void clearComponents();
		
		void addChild(GameObject* obj);
		
		void setParent(GameObject* obj);
		
		GameObject* getParent() const { return _parent; }
		
		void removeChild(GameObject* child);
		
		void removeAllChildren();
		
		void removeFromParent();
		
		int	getChildrenNum() const { return _children.size(); }
		
		GameObject* getChild(int index) { return _children.at(index).get(); }
		
		int lua_getChild(lua_State* L);
		
		bool isActive() const { return _isActive; }
		
		void setActive(bool b) { _isActive = b; }
		
		Transform* getTransform() const { return _transform.get(); }
		
		virtual void update(float dt);
		
		virtual void draw();
		
		void setZOrder(int zOrder);
		
		int getZOrder() const { return _zOrder; }
		
		void setComDrawOrderDirty(bool b) { _comDrawOrderDirty = b; }
		
		void setChildrenZOrderDirty(bool b) { _zOrderDirty = b; }
		
		bool getChildrenZOrderDirty() { return _zOrderDirty; }
		
		static void registerClassToLua(lua_State* L);
		
	protected:
		
		bool	_isActive = true;
		
		bool	_isVisible = true;
		
		int		_zOrder = 0;
		
		bool	_zOrderDirty = false;
		
		bool	_comDrawOrderDirty = false;
		
		RefCountedPtr<Transform> _transform;
		
		ComponentVec _components;
		
		ChildrenVec _children;
		
		GameObject* _parent = nullptr;
		
		unsigned int		_id = 0;
		static unsigned int _nextId;
	};
}

#endif