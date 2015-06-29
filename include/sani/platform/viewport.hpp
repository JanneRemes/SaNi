#pragma once

#include "sani/precompiled.hpp"

namespace sani {
	namespace graphics {
		
		/// @class Viewport viewport.hpp "sani/platform/graphics/viewport.hpp"
		/// @author voidbab
		/// 
		/// Basic viewport implementation.
		class Viewport {
		private:
			uint32 x;
			uint32 y;
			
			uint32 width;
			uint32 height;
		public:
			Viewport(const uint32 x, const uint32 y, const uint32 width, const uint32 height);
			Viewport(const Viewport& viewport);

			uint32 getX() const;
			uint32 getY() const;

			uint32 getWidth() const;
			uint32 getHeight() const;

			~Viewport();

			Viewport& operator =(const Viewport& other);
		};
	}
}
