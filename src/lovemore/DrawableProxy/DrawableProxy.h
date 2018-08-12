#ifndef LOVEMORE_DRAWABLE_PROXY_H
#define LOVEMORE_DRAWABLE_PROXY_H

//LOVE MORE
#include "base/def.h"


namespace lovemore {

	class DrawableProxy : public Drawable
	{
    public:

		virtual ~DrawableProxy() {};

		int lua_get_proxy(lua_State* L);

		static void registerClassToLua(lua_State* L);

	protected:

		virtual const char* name() = 0;
    };

}

#endif