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
#include "Renderer.h"

namespace lovemore {
	
	using namespace love;
	using namespace love::graphics;
	
	using namespace glad;
	
	class SpineAnimator : public Renderer
	{
		enum {
			K_MAX_TRIANGLES_NUM		= 128,
			K_MAX_VERTICES_NUM		= K_MAX_TRIANGLES_NUM * 3
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
		GLbyte				_colors[K_MAX_VERTICES_NUM * 4];
		GLshort				_verticesCount = 0;
		
		static float*		_worldVertices;
		
		void				addVertices(Texture* texture, float* vts, float* uvs, int first, int n, GLbyte r, GLbyte g, GLbyte b, GLbyte a);
		void				flush();
		
		float				_speedScale = 1;
	};
}

#endif