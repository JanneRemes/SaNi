#pragma once 

#include "sani/engine/services/renderable_manager.hpp"
#include "sani/graphics/renderables/triangle.hpp"

namespace sani {

	namespace engine {

		namespace services {

			class TriangleManager : public RenderableManager<graphics::Triangle> {
			public:
				TriangleManager(SaNiEngine* const engine);
			};
		}
	}
}