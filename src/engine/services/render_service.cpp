#include "sani/engine/services/contracts/render_service_contract.hpp"
#include "sani/engine/messaging/messages/document_message.hpp"
#include "sani/engine/messaging/messages/command_message.hpp"
#include "sani/platform/graphics/graphics_device.hpp"
#include "sani/engine/messaging/state_message.hpp"
#include "sani/engine/services/render_service.hpp"
#include "sani/core/utils/string_utils.hpp"
#include "sani/core/utils/convert.hpp"
#include "sani/platform/graphics/window.hpp"
#include "sani/engine/sani_engine.hpp"
#include "sani/graphics/layer.hpp"
#include "sani/core/profiling/profiler.hpp"
#include "sani/engine/services/service_logging.hpp"
#include "sani/engine/services/contracts/cvar_service_contract.hpp"
#include "sani/engine/messaging/messages/query_message.hpp"
#include "sani/core/cvar/cvar.hpp"

namespace sani {

	namespace engine {
		
		namespace services {

			using namespace renderservice;
			using namespace graphics;

			RenderService::RenderService(engine::SaNiEngine* const engine, graphics::GraphicsDevice* const graphicsDevice, graphics::Window* const window) 
				: EngineService("render service", engine),
				  graphicsDevice(graphicsDevice),
				  window(window),
				  renderer(graphicsDevice),
				  clearColor(color::Black) {
			}

			void RenderService::windowClosed(SaNiEngine* const engine) {
				engine->exit();
			}
			void RenderService::windowSizeChanged(GraphicsDevice* const device, Window* const window, Camera2D* const camera) {
				device->setBackbufferSize(window->getClientWidth(), window->getClientHeight());
				device->applyBackbufferChanges();

				const Viewport viewport(0, 0, window->getClientWidth(), window->getClientHeight());
				device->setViewport(viewport);
				camera->setViewport(viewport);
			}
			void RenderService::layerOrderChanged(std::vector<Layer>& layers) {
				std::sort(layers.begin(), layers.end(), [](const Layer& lhs, const Layer& rhs) {
					return lhs.getOrder() < rhs.getOrder();
				});
			}

			void RenderService::handleDocumentMessage(messages::DocumentMessage* const message) {
				const RenderServiceCommands command = static_cast<RenderServiceCommands>(message->getCommand());

				switch (command) {
				case RenderServiceCommands::GetLayers:
					getLayers(message);
					return;
				case RenderServiceCommands::GetCameras:
					getCameras(message);
					return;
				case RenderServiceCommands::GetClearColor:
					getClearColor(message);
					return;
				case RenderServiceCommands::GetGraphicsDevice:
					getGraphicsDevice(message);
					return;
				case RenderServiceCommands::GetWindow:
					getWindow(message);
					return;
				default:
					LOG_DEAD_LETTER(message);
					return;
				}
			}
			void RenderService::handleCommandMessage(messages::CommandMessage* const message) {
				const RenderServiceCommands command = static_cast<RenderServiceCommands>(message->getCommand());

				switch (command) {
				case RenderServiceCommands::CreateLayer:
					createLayer(message);
					return;
				case RenderServiceCommands::DeleteLayer:
					deleteLayer(message);
					return;
				case RenderServiceCommands::CreateCamera:
					createCamera(message);
					return;
				case RenderServiceCommands::DeleteCamera:
					deleteCamera(message);
					return;
				case RenderServiceCommands::SetClearColor:
					setClearColor(message);
					return;
				default:
					LOG_DEAD_LETTER(message);
					return;
				}
			}

			bool RenderService::onStart() {
				// Initialize default viewport.
				Viewport viewport = Viewport(0, 0, window->getClientWidth(), window->getClientHeight());
				graphicsDevice->setViewport(viewport);

				// Initialize default camera and layer.
				// TODO: remove in the future.
				cameras.push_back(Camera2D(viewport));

				// Listen for window exit events so we can close the engine after
				// the window has been closed.
				window->closed += SANI_EVENT_HANDLER(void(void), std::bind(RenderService::windowClosed, getEngine()));
				
				// Check if we need to fit backbuffer.
				SaNiEngine* const engine = getEngine();

				auto* message = engine->createEmptyMessage<messages::QueryMessage>();
				cvarservice::getCVar(message, "stretch_backbuffer");
				engine->routeMessage(message);

				CVar* const cvar = static_cast<CVar* const>(message->getResults());

				int32 stretchBackBuffer = 0;
				VAR_OR_DEFAULT(cvar, stretchBackBuffer, 0);

				if (!stretchBackBuffer) {
					window->sizeChanged += SANI_EVENT_HANDLER(void(void), std::bind(RenderService::windowSizeChanged, graphicsDevice, window, &cameras.back()));
				}

				engine->releaseMessage(message);

				if (!renderer.initialize()) return false;

				// Should never fail.
				return true;
			}
			void RenderService::onTerminate() {
				graphicsDevice->dispose();
			}

			void RenderService::createLayer(messages::CommandMessage* const message) {
				std::vector<String> tokens;

				utils::split(message->getData(), "||", tokens, true);

				SANI_ASSERT(tokens.size() == 3);

				const String name			= tokens[0];
				const LayerType type		= static_cast<LayerType>(utils::toUInt8(tokens[1]));
				const float32 order			= utils::toFloat32(tokens[2]);
				
				layers.push_back(Layer(name, type, order));

				layers.back().orderChanged += SANI_EVENT_HANDLER(void(), std::bind(RenderService::layerOrderChanged, layers));;

				message->markHandled();
			}
			void RenderService::deleteLayer(messages::CommandMessage* const message) {
				const String& name = message->getData();

				auto it = std::find_if(layers.begin(), layers.end(), [&name](const Layer& layer) {
					return layer.getName() == name;
				});

				if (it != layers.end()) {
					layers.erase(it);

					message->markHandled();
				}
			}
			void RenderService::getLayers(messages::DocumentMessage* const message) {
				std::vector<Layer* const>* results = getEngine()->allocateShared<std::vector<Layer* const>>();
				NEW_DYNAMIC_DEFAULT(std::vector<Layer* const>, results);
				
				for (Layer& layer : layers) results->push_back(&layer);

				message->setData(results);
				message->markHandled();
			}
			
			void RenderService::deleteCamera(messages::CommandMessage* const message) {
				const String& name = message->getData();

				auto it = std::find_if(cameras.begin(), cameras.end(), ([&name](const Camera2D& camera) {
					return camera.getName() == name;
				}));

				if (it != cameras.end()) {
					cameras.erase(it);
					
					message->markHandled();
				}
			}
			void RenderService::createCamera(messages::CommandMessage* const message) {
				const String name = message->getData();
				
				auto viewport = graphicsDevice->getViewport();

				cameras.push_back(Camera2D(viewport));
				
				if (name.size() != 0) cameras.back().setName(name);
			}
			void RenderService::getCameras(messages::DocumentMessage* const message) {
				std::vector<Camera2D* const>* results = getEngine()->allocateShared<std::vector<Camera2D* const>>();
				NEW_DYNAMIC_DEFAULT(std::vector<Camera2D* const>, results);

				for (Camera2D& camera : cameras) results->push_back(&camera);

				message->setData(results);
				message->markHandled();
			}

			void RenderService::getClearColor(messages::DocumentMessage* const message) {
				graphics::Color* results = getEngine()->allocateShared<graphics::Color>();
				NEW_DYNAMIC(graphics::Color, results, clearColor);

				message->setData(static_cast<void*>(&results));
				message->markHandled();
			}
			void RenderService::setClearColor(messages::CommandMessage* const message) {
				std::vector<String> tokens;

				utils::split(message->getData(), "||", tokens, true);

				SANI_ASSERT(tokens.size() == 4);

				clearColor.r = utils::toFloat32(tokens[0]);
				clearColor.g = utils::toFloat32(tokens[1]);
				clearColor.b = utils::toFloat32(tokens[2]);
				clearColor.a = utils::toFloat32(tokens[3]);

				message->markHandled();
			}

			void RenderService::getGraphicsDevice(messages::DocumentMessage* const message) {
				message->setData(graphicsDevice);
				message->markHandled();
			}
			void RenderService::getWindow(messages::DocumentMessage* const message) {
				message->setData(window);
				message->markHandled();
			}

			void RenderService::renderToCamera(const graphics::Camera2D& camera) {
				// Swap viewports.
				const Viewport real = graphicsDevice->getViewport();
				const math::Mat4f& transform = camera.transformation();

				graphicsDevice->setViewport(camera.getViewport());

				renderer.beginRendering(transform);

				for (Layer& layer : layers) layer.render(&renderer);

				renderer.endRendering();
				
				graphicsDevice->setViewport(real);
			}

			void RenderService::receive(messages::Message* const message) {
				const MessageType messageType = message->getType();

				switch (messageType) {
				case MessageType::Command:
					handleCommandMessage(static_cast<messages::CommandMessage*>(message));
					break;
				case MessageType::Document:
					handleDocumentMessage(static_cast<messages::DocumentMessage*>(message));
					break;
				default:
					LOG_DEAD_LETTER(message);
					break;
				}
			}
			void RenderService::update(const EngineTime& time) {
				BEGIN_PROFILING;

				window->listen();

				// No need to render if there are no cameras.
				graphicsDevice->clear(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

				if (cameras.size() == 0) return;

				for (Camera2D& camera : cameras) {
					camera.computeTransformation();

					renderToCamera(camera);
				}

				graphicsDevice->present(NULL);
				
				END_PROFILING;
			}

			RenderService::~RenderService() {
				delete graphicsDevice;
				delete window;
			}
		}
	}
}