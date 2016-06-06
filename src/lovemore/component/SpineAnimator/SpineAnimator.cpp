
#include "lovemore.h"
#include "SpineAnimator.h"
#include "scope_guard.h"

//Love
#include "modules/filesystem/Filesystem.h"
#include "modules/graphics/Texture.h"
#include "modules/graphics/opengl/Image.h"
#include "modules/graphics/opengl/OpenGL.h"
#include "common/runtime.h"

using namespace love;
using namespace love::filesystem;
using namespace love::graphics;
using namespace love::graphics::opengl;

using namespace lovemore;

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path)
{
	lua_State* L = lovemore_getLuaState();
	int top = lua_gettop(L);
	
	lua_getglobal(L, "love");
	lua_getfield(L, -1, "graphics");
	lua_getfield(L, -1, "newImage");
	lua_pushstring(L, path);
	lua_call(L, 1, 1);
	
	Texture* t = luax_checktype<Texture>(L, -1, GRAPHICS_TEXTURE_ID);
	t->retain();
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
	FileData *fd = fs->read(path);
	*length = (int)fd->getSize();
	char* data = MALLOC(char, *length);
	memcpy(data, fd->getData(), *length);
	fd->release();
	return data;
}

template <typename T_Attachment> Texture* getTexture(T_Attachment* attachment)
{
	return (Texture*)((spAtlasRegion*)attachment->rendererObject)->page->rendererObject;
}

template <int n>
void flipVerticesY(float* v)
{
	for (int i = 0; i < n; ++i) {
		v[i * 2 + 1] = - v[i * 2 + 1];
	}
}

SpineAnimator::SpineAnimator(const char* skeletonDataFile, const char* atlasFile)
{
	_atlas = spAtlas_createFromFile(atlasFile, nullptr);
	assert(_atlas && "Error reading atlas file.");
	spSkeletonJson* json = spSkeletonJson_create(_atlas);
	json->scale = 1;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile);
	assert(skeletonData && (json->error ? json->error : "Error reading skeleton data file."));
	spSkeletonJson_dispose(json);
	
	_skeleton = spSkeleton_create(skeletonData);
	_state = spAnimationState_create(spAnimationStateData_create(_skeleton->data));
	_state->rendererObject = this;
}

SpineAnimator::~SpineAnimator()
{
	spAnimationStateData_dispose(_state->data);
	spAnimationState_dispose(_state);
}

void SpineAnimator::update(float dt)
{
	dt *= _speedScale;
	spSkeleton_update(_skeleton, dt);
	spAnimationState_update(_state, dt);
	spAnimationState_apply(_state, _skeleton);
	spSkeleton_updateWorldTransform(_skeleton);
}

void SpineAnimator::draw(GLGraphics* /*g*/)
{
	spSlot* slot = nullptr;
	Vertex vertices[4];
	float worldVertices[8];
	for (int i = 0, n = _skeleton->slotsCount; i < n; ++i)
	{
		slot = _skeleton->drawOrder[i];
		if (!slot->attachment)
		{
			continue;
		}
		switch (slot->attachment->type)
		{
			case SP_ATTACHMENT_REGION:
			{
				spRegionAttachment *attachment = (spRegionAttachment*)(slot->attachment);
				spRegionAttachment_computeWorldVertices(attachment, slot->bone, worldVertices);
				flipVerticesY<4>(worldVertices);
				Texture* t = getTexture(attachment);
				
				GLbyte r = static_cast<GLbyte>(_skeleton->r * slot->r * 255);
				GLbyte g = static_cast<GLbyte>(_skeleton->g * slot->g * 255);
				GLbyte b = static_cast<GLbyte>(_skeleton->b * slot->b * 255);
				GLbyte a = static_cast<GLbyte>(_skeleton->a * slot->a * 255);
				
				vertices[0].r = r;
				vertices[0].g = g;
				vertices[0].b = b;
				vertices[0].a = a;
				vertices[0].x = worldVertices[SP_VERTEX_X2];
				vertices[0].y = worldVertices[SP_VERTEX_Y2];
				vertices[0].s = attachment->uvs[SP_VERTEX_X2];
				vertices[0].t = attachment->uvs[SP_VERTEX_Y2];
				
				vertices[1].r = r;
				vertices[1].g = g;
				vertices[1].b = b;
				vertices[1].a = a;
				vertices[1].x = worldVertices[SP_VERTEX_X1];
				vertices[1].y = worldVertices[SP_VERTEX_Y1];
				vertices[1].s = attachment->uvs[SP_VERTEX_X1];
				vertices[1].t = attachment->uvs[SP_VERTEX_Y1];
				
				vertices[2].r = r;
				vertices[2].g = g;
				vertices[2].b = b;
				vertices[2].a = a;
				vertices[2].x = worldVertices[SP_VERTEX_X3];
				vertices[2].y = worldVertices[SP_VERTEX_Y3];
				vertices[2].s = attachment->uvs[SP_VERTEX_X3];
				vertices[2].t = attachment->uvs[SP_VERTEX_Y3];
				
				vertices[3].r = r;
				vertices[3].g = g;
				vertices[3].b = b;
				vertices[3].a = a;
				vertices[3].x = worldVertices[SP_VERTEX_X4];
				vertices[3].y = worldVertices[SP_VERTEX_Y4];
				vertices[3].s = attachment->uvs[SP_VERTEX_X4];
				vertices[3].t = attachment->uvs[SP_VERTEX_Y4];
				
				gl.bindTexture(*((GLuint*)t->getHandle()));
		
				gl.useVertexAttribArrays(ATTRIBFLAG_POS | ATTRIBFLAG_TEXCOORD);
				
				glVertexAttribPointer(ATTRIB_POS, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vertices[0].x);
				glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &vertices[0].s);
				
				gl.prepareDraw();
				gl.drawArrays(GL_TRIANGLE_STRIP, 0, 4);

				break;
			}
			case SP_ATTACHMENT_MESH:
			{
				
				break;
			}
			case SP_ATTACHMENT_WEIGHTED_MESH:
			{
				
				break;
			}
			default:
				break;
		}
	}
}

void SpineAnimator::setMix (const char* fromAnimation, const char* toAnimation, float duration)
{
	spAnimationStateData_setMixByName(_state->data, fromAnimation, toAnimation, duration);
}

void SpineAnimator::setAnimation(int trackIndex, const char* name, bool loop)
{
	spAnimation* animation = spSkeletonData_findAnimation(_skeleton->data, name);
	if (!animation) {
		printf("Spine: Animation not found: %s", name);
		return;
	}
	spAnimationState_setAnimation(_state, trackIndex, animation, loop);
}

void SpineAnimator::addAnimation (int trackIndex, const char* name, bool loop, float delay)
{
	spAnimation* animation = spSkeletonData_findAnimation(_skeleton->data, name);
	if (!animation) {
		printf("Spine: Animation not found: %s", name);
		return;
	}
	spAnimationState_addAnimation(_state, trackIndex, animation, loop, delay);
}

void SpineAnimator::registerClassToLua(luabridge::Namespace& ns)
{
	ns.deriveClass<SpineAnimator, Component>("SpineAnimator")
	.addData("speedScale", &SpineAnimator::_speedScale)
	.addFunction("setMix", &SpineAnimator::setMix)
	.addFunction("setAnimation", &SpineAnimator::setAnimation)
	.addFunction("addAnimation", &SpineAnimator::addAnimation)
	.endClass();
}
