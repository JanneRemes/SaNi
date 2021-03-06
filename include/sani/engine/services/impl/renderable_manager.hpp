#include "sani/engine/services/contracts/renderable_manager_contract.hpp"
#include "sani/engine/messaging/messages/document_message.hpp"
#include "sani/engine/services/renderable_manager.hpp"
#include "sani/graphics/renderables/renderables.hpp"
#include "sani/engine/sani_engine.hpp"
#include "sani/engine/services/service_logging.hpp"
#include "sani/core/logging/log.hpp"

#include <vector>

namespace sani {

	namespace engine {
		
		namespace services {

			using namespace renderablemanager;

			template <class T>
			RenderableManager<T>::RenderableManager(const String& name, const ElementType type, engine::SaNiEngine* const engine) : EngineService(name, engine),
																																	type(type),
																																	allocator(256, 1) {
			}

			template <class T>
			const RegisterAllocator<T>& RenderableManager<T>::getAllocator() const {
				return allocator;
			}

			template <class T>
			void RenderableManager<T>::handleDocumentMessage(messages::DocumentMessage* const message) {
				const RenderableManagerCommands command = static_cast<RenderableManagerCommands>(message->getCommand());

				switch (command) {
				case RenderableManagerCommands::CreateElement:
					createElement(message);
					return;
				case RenderableManagerCommands::DeleteElement:
					deleteElement(message);
					return;
				case RenderableManagerCommands::GetElements:
					getElements(message);
					return;
				default:
					return;
				}
			}
			
			template <class T>
			void RenderableManager<T>::createElement(messages::DocumentMessage* const message) {
				T* element	= nullptr;
				uint32 id	= 0;

				allocator.allocate(element, id);

				element->id = id;

				message->setData(element);
				message->markHandled();

				FNCLOG_INF(log::OutFlags::All, "created renderable element");
			}
			template <class T>
			void RenderableManager<T>::deleteElement(messages::DocumentMessage* const message) {
				T* element = static_cast<T*>(message->getData());

				allocator.deallocate(element->id);

				message->markHandled();

				FNCLOG_INF(log::OutFlags::All, "deleted renderable element");
			}

			template <class T>
			void RenderableManager<T>::getElements(messages::DocumentMessage* const message) {
				const std::vector<T*>* results = allocator.allocatedElements();

				message->setData((void*)results);
				message->markHandled();
			}

			template <class T>
			void RenderableManager<T>::receive(messages::Message* const message) {
				if (message->getType() == MessageType::Document) {
					handleDocumentMessage(static_cast<messages::DocumentMessage* const>(message));
				} else {
					LOG_DEAD_LETTER(message);
				}
			}
		}
	}
}