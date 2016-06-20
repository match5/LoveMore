#ifndef LOVEMORE_SPINE_ANIMATOR_H
#define LOVEMORE_SPINE_ANIMATOR_H

//SPINE
#include <spine/spine.h>
#include <spine/extension.h>

//LOVE
#include "modules/graphics/Texture.h"
#include "modules/graphics/Quad.h"
#include "common/Vector.h"

//LOVEMORE
#include "../Renderer.h"

//C++
#include <unordered_map>
#include <string>

namespace lovemore {
	
	using namespace love;
	using namespace love::graphics;
	
	using namespace glad;
	
	class SpineAnimator : public Renderer
	{
		enum {
			K_MAX_TRIANGLES_NUM	= 256,
			K_MAX_VERTICES_NUM	= K_MAX_TRIANGLES_NUM * 3
		};
		
	public:
		
		SpineAnimator(const char* skeletonDataFile, const char* atlasFile);
		
		virtual ~SpineAnimator() override;
		
		virtual const char* getName() const override { return "SpineAnimator"; }
		
		void setMix (const char* fromAnimation, const char* toAnimation, float duration);
		
		void setAnimation(int trackIndex, const char* name, bool loop);
		
		void addAnimation (int trackIndex, const char* name, bool loop, float delay);
		
		void clearTrack(int trackIndex);
		
		void clearTracks();
		
		int lua_setAnimationListener(lua_State* L);
		
		void onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount);
		
		virtual void update(float dt) override;
		
		virtual void draw(GLGraphics* g) override;
		
		static void registerClassToLua(lua_State* L);
		
	protected:
		
		spSkeleton*			_skeleton = nullptr;
		spAtlas*			_atlas = nullptr;
		spAnimationState*	_state = nullptr;
		
		luabridge::LuaRef	_startListener;
		luabridge::LuaRef	_endListener;
		luabridge::LuaRef	_completeListener;
		luabridge::LuaRef	_eventListener;
		
		static Texture*		_texture;
		static GLshort		_verticesCount;
		static float*		_vertices;
		static float*		_uvs;
		static GLbyte*		_colors;
		
		static float*		_worldVertices;
		
		void				addVertices(Texture* texture, float* vts, float* uvs, int first, int n, GLbyte r, GLbyte g, GLbyte b, GLbyte a);
		void				flush();
		
		float				_speedScale = 1;
	};
}

#endif