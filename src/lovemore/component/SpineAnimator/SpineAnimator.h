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
#include "Component.h"

namespace lovemore {
	
	using namespace love;
	using namespace love::graphics;
	
	class SpineAnimator : public Component
	{
		enum {
			K_MAX_VERTICES_NUM = 128
		};
		
	public:
		
		SpineAnimator(const char* skeletonDataFile, const char* atlasFile);
		
		virtual ~SpineAnimator() override;
		
		virtual void update(float dt) override;
		
		virtual void draw(GLGraphics* g) override;
		
		virtual const char* getName() const override { return "SpineAnimator"; }
		
		void setMix (const char* fromAnimation, const char* toAnimation, float duration);
		
		void setAnimation(int trackIndex, const char* name, bool loop);
		
		void addAnimation (int trackIndex, const char* name, bool loop, float delay);
		
		static void registerClassToLua(lua_State* L);
		
	protected:
		
		spSkeleton*			_skeleton = nullptr;
		spAtlas*			_atlas = nullptr;
		spAnimationState*	_state = nullptr;
		
		Texture*			_texture = nullptr;
		float				_vertices[K_MAX_VERTICES_NUM * 2];
		float				_uvs[K_MAX_VERTICES_NUM * 2];
		unsigned			_verticesCount = 0;
		
		static float*		_worldVertices;
		
		void				addVertices(Texture* texture, float* vts, float* uvs, int n);
		void				flush();
		
		float				_speedScale = 1;
		
		bool				_flipX = false;
		bool				_flipY = false;
	};
}

#endif