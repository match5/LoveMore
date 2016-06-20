
//LoveMore
#include "lovemore.h"
#include "SpineAnimator.h"

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

using namespace glad;

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path)
{
	lua_State* L = lovemore_getLuaState();
	
	//call love.graphics.newImage(path)
	static lua_CFunction love_graphics_newImage = nullptr;
	if (love_graphics_newImage == 0)
	{
		lua_getglobal(L, "love");
		lua_getfield(L, -1, "graphics");
		lua_getfield(L, -1, "newImage");
		love_graphics_newImage = lua_tocfunction(L, -1);
	}
	lua_settop(L, 0);
	lua_pushstring(L, path);
	love_graphics_newImage(L);
	
	Texture* t = luax_checktype<Texture>(L, -1, GRAPHICS_TEXTURE_ID);
	t->retain();
	self->rendererObject = t;
	self->width = t->getWidth();
	self->height = t->getHeight();
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

void animationCallback (spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) {
	static_cast<SpineAnimator*>(state->rendererObject)->onAnimationStateEvent(trackIndex, type, event, loopCount);
}

Texture* SpineAnimator::_texture = nullptr;
GLshort SpineAnimator::_verticesCount = 0;
float*	SpineAnimator::_vertices = new float[K_MAX_VERTICES_NUM * 2];
float*	SpineAnimator::_uvs = new float[K_MAX_VERTICES_NUM * 2];
GLbyte*	SpineAnimator::_colors =  new GLbyte[K_MAX_VERTICES_NUM * 4];
float*	SpineAnimator::_worldVertices = new float[K_MAX_VERTICES_NUM * 2];

SpineAnimator::SpineAnimator(const char* skeletonDataFile, const char* atlasFile)
:_startListener(lovemore_getLuaState())
,_endListener(lovemore_getLuaState())
,_completeListener(lovemore_getLuaState())
,_eventListener(lovemore_getLuaState())
{
	_atlas = spAtlas_createFromFile(atlasFile, nullptr);
	assert(_atlas && "Error reading atlas file.");
	spSkeletonJson* json = spSkeletonJson_create(_atlas);
	json->scale = 1;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile);
	assert(skeletonData && (json->error ? json->error : "Error reading skeleton data file."));
	spSkeletonJson_dispose(json);
	
	if (skeletonData)
	{
		_skeleton = spSkeleton_create(skeletonData);
		_state = spAnimationState_create(spAnimationStateData_create(_skeleton->data));
		_state->rendererObject = this;
		_state->listener = animationCallback;
	}
}

SpineAnimator::~SpineAnimator()
{
	spAnimationStateData_dispose(_state->data);
	spAnimationState_dispose(_state);
}

void SpineAnimator::setMix(const char* fromAnimation, const char* toAnimation, float duration)
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

void SpineAnimator::clearTrack(int trackIndex)
{
	spAnimationState_clearTrack(_state, trackIndex);
}

void SpineAnimator::clearTracks()
{
	spAnimationState_clearTracks(_state);
}

int SpineAnimator::lua_setAnimationListener(lua_State* L)
{
	const char* name = lua_tostring(L, 2);
	if (name && strlen(name) > 0)
	{
		luabridge::LuaRef func = luabridge::Stack<luabridge::LuaRef>::get(L, 3);
		if (func.isFunction())
		{
			if (strcmp(name, "start") == 0)
			{
				_startListener = func;
			}
			else if (strcmp(name, "end") == 0)
			{
				_endListener = func;
			}
			else if (strcmp(name, "complete") == 0)
			{
				_completeListener = func;
			}
			else if (strcmp(name, "event") == 0)
			{
				_eventListener = func;
			}
		}
	}
	return 0;
}

void SpineAnimator::onAnimationStateEvent (int trackIndex, spEventType type, spEvent* event, int loopCount)
{
	switch (type)
	{
		case SP_ANIMATION_START:
			if (_startListener.isFunction())
			{
				try {
					_startListener(trackIndex);
				} catch (std::exception& e) {
					luaL_error(_startListener.state(), e.what());
				}
			}
			break;
		case SP_ANIMATION_END:
			if (_endListener.isFunction())
			{
				try {
					_endListener(trackIndex);
				} catch (std::exception& e) {
					luaL_error(_endListener.state(), e.what());
				}
			}
			break;
		case SP_ANIMATION_COMPLETE:
			if (_completeListener.isFunction())
			{
				try {
					_completeListener(trackIndex, loopCount);
				} catch (std::exception& e) {
					luaL_error(_completeListener.state(), e.what());
				}
			}
			break;
		case SP_ANIMATION_EVENT:
			if (_eventListener.isFunction())
			{
				try {
					luabridge::LuaRef evt = luabridge::newTable(lovemore_getLuaState());
					evt["intValue"] = event->intValue;
					evt["floatValue"] = event->floatValue;
					if (event->stringValue)
					{
						evt["stringValue"] = event->stringValue;
					}
					evt["time"] = event->time;
					_eventListener(trackIndex, evt);
				} catch (std::exception& e) {
					luaL_error(_eventListener.state(), e.what());
				}
			}
			break;
	}
}

void SpineAnimator::update(float dt)
{
	dt *= _speedScale;
	spSkeleton_update(_skeleton, dt);
	spAnimationState_update(_state, dt);
	spAnimationState_apply(_state, _skeleton);
	spSkeleton_updateWorldTransform(_skeleton);
}

void SpineAnimator::draw(GLGraphics* g)
{
	spSlot* slot = nullptr;
	g->push();
	g->scale(_flipX? -1 : 1,_flipY ? -1 : 1);
	for (int i = 0, n = _skeleton->slotsCount; i < n; ++i)
	{
		slot = _skeleton->drawOrder[i];
		if (!slot->attachment)
		{
			continue;
		}
		
		love::graphics::Graphics::BlendAlpha	alpha;
		love::graphics::Graphics::BlendMode		mode = g->getBlendMode(alpha);
		love::graphics::Graphics::BlendMode		newMode;
		switch (slot->data->blendMode)
		{
			case SP_BLEND_MODE_ADDITIVE:
				newMode = love::graphics::Graphics::BlendMode::BLEND_ADD;
				break;
				
			case SP_BLEND_MODE_MULTIPLY:
				newMode = love::graphics::Graphics::BlendMode::BLEND_MULTIPLY;
				break;
				
			case SP_BLEND_MODE_SCREEN:
				newMode = love::graphics::Graphics::BlendMode::BLEND_SCREEN;
				break;
				
			case SP_BLEND_MODE_NORMAL:
			default:
				newMode = love::graphics::Graphics::BlendMode::BLEND_ALPHA;
				break;
		}
		if (newMode != mode)
		{
			flush();
			g->setBlendMode(newMode, alpha);
		}
		
		GLbyte r = static_cast<GLbyte>(_skeleton->r * slot->r * _color.r);
		GLbyte g = static_cast<GLbyte>(_skeleton->g * slot->g * _color.g);
		GLbyte b = static_cast<GLbyte>(_skeleton->b * slot->b * _color.b);
		GLbyte a = static_cast<GLbyte>(_skeleton->a * slot->a * _color.a);
		
		switch (slot->attachment->type)
		{
			case SP_ATTACHMENT_REGION:
			{
				spRegionAttachment *region = (spRegionAttachment*)slot->attachment;
				Texture* t = (Texture*)((spAtlasRegion*)region->rendererObject)->page->rendererObject;
				spRegionAttachment_computeWorldVertices(region, slot->bone, _worldVertices);
				
				addVertices(t, _worldVertices, region->uvs, 0, 3, r, g, b, a);	//0 1 2
				addVertices(t, _worldVertices, region->uvs, 2, 2, r, g, b, a);	//2 3
				addVertices(t, _worldVertices, region->uvs, 0, 1, r, g, b, a);	//0
				
				break;
			}
			case SP_ATTACHMENT_MESH:
			{
				spMeshAttachment* mesh = (spMeshAttachment*)slot->attachment;
				if (mesh->verticesCount > K_MAX_VERTICES_NUM)
				{
					assert(!"mesh->verticesCount > K_MAX_VERTICES_NUM");
					continue;
				}
				Texture *t = (Texture*)((spAtlasRegion*)mesh->rendererObject)->page->rendererObject;
				spMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
				
				for (int i = 0; i < mesh->trianglesCount; ++i)
				{
					addVertices(t, _worldVertices, mesh->uvs, mesh->triangles[i], 1, r, g, b, a);
				}
				
				break;
			}
			case SP_ATTACHMENT_WEIGHTED_MESH:
			{
				spWeightedMeshAttachment* mesh = (spWeightedMeshAttachment*)slot->attachment;
				if (mesh->uvsCount > K_MAX_VERTICES_NUM)
				{
					assert(!"mesh->uvsCount > K_MAX_VERTICES_NUM");
					continue;
				}
				Texture *t = (Texture*)((spAtlasRegion*)mesh->rendererObject)->page->rendererObject;
				spWeightedMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
				
				for (int i = 0; i < mesh->trianglesCount; ++i)
				{
					addVertices(t, _worldVertices, mesh->uvs, mesh->triangles[i], 1, r, g, b, a);
				}
				
				break;
			}
			case SP_ATTACHMENT_LINKED_MESH:
			{
				break;
			}
			default:
				break;
		}
	}
	flush();
	g->pop();
}

void SpineAnimator::addVertices(Texture* texture, float* vts, float* uvs, int first, int n, GLbyte r, GLbyte g, GLbyte b, GLbyte a)
{
	if (_verticesCount + n > K_MAX_VERTICES_NUM)
	{
		flush();
	}
	if (_texture != texture)
	{
		flush();
		_texture = texture;
	}
	for (int i = first, iend = first + n; i < iend; ++i, ++_verticesCount)
	{
		_vertices[_verticesCount * 2] = vts[i * 2];
		_vertices[_verticesCount * 2 + 1] = - vts[i * 2 + 1];	//filpY
		_uvs[_verticesCount * 2] = uvs[i * 2];
		_uvs[_verticesCount * 2 + 1] = uvs[i * 2 + 1];
		
		_colors[_verticesCount * 4] = r;
		_colors[_verticesCount * 4 + 1] = g;
		_colors[_verticesCount * 4 + 2] = b;
		_colors[_verticesCount * 4 + 3] = a;
	}
}

void SpineAnimator::flush()
{
	if (_verticesCount > 0)
	{
		gl.bindTexture(*((GLuint*)_texture->getHandle()));
		gl.useVertexAttribArrays(ATTRIBFLAG_POS | ATTRIBFLAG_TEXCOORD | ATTRIBFLAG_COLOR);
		
		glVertexAttribPointer(ATTRIB_POS, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, _vertices);
		glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, _uvs);
		glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, _colors);
		
		gl.prepareDraw();
		gl.drawArrays(GL_TRIANGLES, 0, _verticesCount);
		_verticesCount = 0;
	}
}

void SpineAnimator::registerClassToLua(lua_State* L)
{
	luabridge::getGlobalNamespace(L)
	.deriveClass<SpineAnimator, Renderer>("SpineAnimator")
	.addStaticFunction("castFrom", &Component::castFrom<SpineAnimator>)
	.addData("speedScale", &SpineAnimator::_speedScale)
	.addFunction("setMix", &SpineAnimator::setMix)
	.addFunction("setAnimation", &SpineAnimator::setAnimation)
	.addFunction("addAnimation", &SpineAnimator::addAnimation)
	.addFunction("clearTrack", &SpineAnimator::clearTrack)
	.addFunction("clearTracks", &SpineAnimator::clearTracks)
	.addCFunction("setAnimationListener", &SpineAnimator::lua_setAnimationListener)
	.endClass();
}
