#include "sani/graphics/renderables/renderable.hpp"

namespace sani {

	namespace graphics {

		Renderable::Renderable(const uint32 vertices, const uint32 indices, const uint32 renderElements, Texture* const texture) : renderData(vertices, indices, renderElements),
																																   texture(texture),
																																   id(0),
																																   visible(true) {
		}
		Renderable::Renderable(const uint32 vertices, const uint32 indices, const uint32 renderElements) : Renderable(vertices, indices, renderElements, nullptr) {
		}
		Renderable::Renderable(const uint32 vertices, const uint32 renderElements) : Renderable(vertices, 0, renderElements, nullptr) {
		}
	}
}