#pragma once

#include "sani/graphics/renderables/renderable.hpp"
#include "sani/graphics/sprite_texturing_mode.hpp"
#include "sani/forward_declare.hpp"

namespace sani {

	namespace graphics {

		SANI_FORWARD_DECLARE_2(sani, resource, Texture2D);

		/// @class Sprite sprite.hpp "sani/graphics/renderables/sprite.hpp"
		/// @author voidbab
		///
		/// A sprite defined by transform, source and texturing mode.
		class Sprite : public Renderable {
		public:
			SpriteTexturingMode texturingMode;
			Color color;

			sani::resource::Texture2D* texture;

			Sprite(sani::resource::Texture2D* const texture, const math::Vec2f& position, const math::Vec2f& size);
			Sprite(sani::resource::Texture2D* const texture, const math::Rectf& bounds);
			Sprite(sani::resource::Texture2D* const texture, const math::Vec2f& size);
			Sprite(sani::resource::Texture2D* const texture);
		};

		inline void recomputeGeometryData(Sprite& sprite);
		inline void recomputeVertices(Sprite& sprite);
		inline void recomputeBounds(Sprite& sprite);

		inline void updateRenderData(Sprite& sprite);
	}
}