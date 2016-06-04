
#ifndef LOVEMORE_SPRITE_RENDERER_H
#define LOVEMORE_SPRITE_RENDERER_H

//LOVE
#include "modules/graphics/Texture.h"
#include "modules/graphics/Quad.h"
#include "common/Vector.h"

//LOVEMORE
#include "Component.h"

namespace lovemore {
	
	using namespace love;
	using namespace love::graphics;
	
	class SpriteRenderer : public Component
	{
	public:
		
		SpriteRenderer();
		
		virtual ~SpriteRenderer() override;
		
		virtual void draw(GLGraphics* g) override;
		
		virtual const char* getName() const override { return "SpriteRenderer"; }
		
		void setTexture(Texture* t) { _texture = t; }
		
		void setQuad(Quad* q) { _quad = q; }
		
		int lua_setTexture(lua_State *L);
		
		int lua_setTextureAndQuad(lua_State *L);
		
		void setAnchor(float x, float y) { _anchorX = x; _anchorY = y; }
		
		Vector getAnchor() const { return Vector(_anchorX, _anchorY); }
		
		int lua_getAnchor(lua_State* L);
		
		static void registerClassToLua(luabridge::Namespace& ns);
		
	protected:
		
		StrongRef<graphics::Texture>	_texture;
		
		StrongRef<graphics::Quad>		_quad;
		
		float _anchorX = 0.5;
		
		float _anchorY = 0.5;
	};
}

#endif
