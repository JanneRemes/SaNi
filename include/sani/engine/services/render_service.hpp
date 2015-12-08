#pragma once

#include "sani/engine/services/engine_service.hpp"
#include "sani/graphics/renderer.hpp"
#include "sani/graphics/camera2d.hpp"
#include "sani/graphics/layer.hpp"

#include <vector>
#include <list>

SANI_FORWARD_DECLARE_3(sani, engine, message, DocumentMessage);
SANI_FORWARD_DECLARE_3(sani, engine, message, CommandMessage);

SANI_FORWARD_DECLARE_2(sani, graphics, GraphicsDevice);
SANI_FORWARD_DECLARE_2(sani, engine, SaNiEngine);

namespace sani {

	namespace engine {
	
		namespace services {

			class RenderService final : public EngineService {
			private:
				graphics::GraphicsDevice* const graphicsDevice;
				graphics::Renderer renderer;

				std::list<graphics::Camera2D> cameras;
				std::vector<graphics::Layer> layers;

				void handleStateMessage(StateMessage* const message) final override;

				/*
					Message handling.
				*/

				void handleDocumentMessage(messages::DocumentMessage* const message);
				void handleCommandMessage(messages::CommandMessage* const message);
				
				/*
					Command functions.				
				*/

				void createLayer(messages::CommandMessage* const message);
				void deleteLayer(messages::CommandMessage* const message);
				void getLayers(messages::DocumentMessage* const message);

				void removeCamera(message::CommandMessage* const message);
				void addCamera(message::CommandMessage* const message);

				void renderToCamera(const graphics::Camera2D& camera);
			public:
				RenderService(engine::SaNiEngine* const engine, graphics::GraphicsDevice* const graphicsDevice);

				void receive(messages::Message* const message) final override;
				void update(const EngineTime& time) final override;
			};
		}
	}
}