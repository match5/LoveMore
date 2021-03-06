#ifndef LOVEMORE_DRAWABLE_PROXY_H
#define LOVEMORE_DRAWABLE_PROXY_H

//LOVE MORE
#include "base/def.h"


namespace lovemore {

	class DrawableProxy : public Drawable
	{
    public:

        DrawableProxy() {
            _proxy.type = love::Type::GRAPHICS_DRAWABLE_ID;
	        _proxy.object = this;
        }

		virtual ~DrawableProxy() {};

		int lua_get_proxy(lua_State* L);

		static void registerClassToLua(lua_State* L);

	protected:
        love::Proxy _proxy;
    };

}

#endif