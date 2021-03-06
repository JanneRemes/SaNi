#pragma once

#include "sani/types.hpp"

namespace sani {

	namespace graphics {

		/// @class Color color.hpp "sani/platform/graphics/viewport.hpp"
		/// @author voidbab
		/// 
		/// Mutable 32-bit color.
		struct Color {
			float32 r;
			float32 g;
			float32 b;
			float32 a;

			/// Creates new instance of the color object with given values. The values are divided by 255.
			Color(const uint8 r, const uint8 g, const uint8 b, const uint8 a);
			/// Creates new instance of the color object with already created color and custom alpha value
			Color(const Color& color, const uint8 a);

			/// Creates new instance of the color object with given values from 0 to 1.0f.
			Color(const float32 r, const float32 g, const float32 b, const float32 a);
			Color(const Color& color, const float32 a);

			Color();
			
			~Color();

			Color& operator =(const Color& rhs);
			bool operator ==(const Color& rhs);
			bool operator !=(const Color& rhs);
		};

		namespace color {

			const Color Transparent(0.0f, 0.0f, 0.0f, 0.0f);
			const Color Black(0.0f, 0.0f, 0.0f, 1.0f);
			const Color White(1.0f, 1.0f, 1.0f, 1.0f);
			const Color Red(1.0f, 0.0f, 0.0f, 1.0f);
			const Color Green(0.0f, 1.0f, 0.0f, 1.0f);
			const Color Blue(0.0f, 0.0f, 1.0f, 1.0f);
		}
	}
}

