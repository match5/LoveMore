
#ifndef LOVEMORE_RENDERER_H
#define LOVEMORE_RENDERER_H

//LOVEMORE
#include "Component.h"

namespace lovemore {
	
	using namespace love;
	using namespace love::graphics;
	
	class Renderer : public Component
	{
	public:
		
		Renderer();
		
		virtual ~Renderer() override;
		
		void setColor(const Colorf& c) { _color = c; }
		const Colorf& getColor() const { return _color; }
		
		void setAlpha(float a) { _color.a = 255 * a; }
		float getAlpha() const { return _color.a / 255; }
		
		int lua_setColor(lua_State* L);
		int lua_getColor(lua_State* L);
		
		static void registerClassToLua(lua_State* L);
		
	protected:
		
		bool _flipX = false;
		bool _flipY = false;
		
		Colorf _color = {255, 255, 255, 255};
	};
}


#endif
