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
	public:
		
		static SpineAnimator* create(const char* skeletonDataFile, const char* atlasFile);
		
		SpineAnimator(spSkeletonData* skeletonData);
		
		virtual ~SpineAnimator() override;
		
		virtual void update(float dt) override;
		
		virtual void draw(GLGraphics* g) override;
		
		virtual const char* getName() const override { return "SpineAnimator"; }
		
		static void registerClassToLua(luabridge::Namespace& ns);
		
	protected:
		
		spSkeleton*			_skeleton;
		
		spAnimationState*	_state;
		
		float				_speedScale = 1;
	};
}

#endif