
#include "lovemore.h"
#include "SpineAnimator.h"

//Love
#include "modules/filesystem/Filesystem.h"
#include "modules/graphics/Texture.h"
#include "modules/graphics/opengl/Image.h"
#include "common/runtime.h"

using namespace love;
using namespace love::filesystem;
using namespace love::graphics;

using namespace lovemore;

static StrongRef<FileData> SharedFileData = nullptr;

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path)
{
	lua_State* L = lovemore_getLuaState();
	int top = lua_gettop(L);
	
	lua_getglobal(L, "love");
	lua_getfield(L, -1, "graphics");
	lua_getfield(L, -1, "newImage");
	lua_pushstring(L, path);
	lua_call(L, -2, 1);
	
	Texture* t = luax_checktype<Texture>(L, -1, GRAPHICS_TEXTURE_ID);
	self->rendererObject = t;
	self->width = t->getWidth();
	self->height = t->getHeight();
	
	lua_settop(L, top);
}

void _spAtlasPage_disposeTexture (spAtlasPage* self)
{
	static_cast<Texture*>(self->rendererObject)->release();
}

char* _spUtil_readFile (const char* path, int* length)
{
	Filesystem *fs = Module::getInstance<Filesystem>(Module::M_FILESYSTEM);
	SharedFileData = fs->read(path);
	SharedFileData->release();
	*length = (int)SharedFileData->getSize();
	return (char*)SharedFileData->getData();
}

SpineAnimator* SpineAnimator::create(const char* skeletonDataFile, const char* atlasFile)
{
	return nullptr;
}

SpineAnimator::SpineAnimator(spSkeletonData* skeletonData)
{
	
}

SpineAnimator::~SpineAnimator()
{
}

void SpineAnimator::update(float dt)
{
	spSkeleton_update(_skeleton, dt);
	spAnimationState_update(_state, dt * _speedScale);
	spAnimationState_apply(_state, _skeleton);
	spSkeleton_updateWorldTransform(_skeleton);
}

void SpineAnimator::draw(GLGraphics* /*g*/)
{
	
}

void SpineAnimator::registerClassToLua(luabridge::Namespace& ns)
{
	ns.deriveClass<SpineAnimator, Component>("SpineAnimator")
	.endClass();
}
