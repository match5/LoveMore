#ifndef LOVEMORE_LUA_OBJECT_H
#define LOVEMORE_LUA_OBJECT_H

// LUA
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "LuaBridge/LuaBridge.h"

//LOVE
#include "common/runtime.h"
#include "common/Object.h"
#include "modules/graphics/Drawable.h"
#include "modules/graphics/Graphics.h"
#include "modules/graphics/Texture.h"
#include "modules/graphics/Quad.h"
#include "modules/graphics/opengl/Graphics.h"

namespace luabridge
{
	template <class T>
	struct ContainerTraits <love::StrongRef <T> >
	{
		typedef typename T Type;

		static T* get (love::StrongRef <T> const& c)
		{
			return c.get ();
		}
	};
}

namespace lovemore {
	typedef love::Object Object;
	typedef love::graphics::Drawable Drawable;
    typedef love::graphics::opengl::Graphics GlGraphics;
}

#endif
