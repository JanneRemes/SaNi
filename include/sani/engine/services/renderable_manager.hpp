#pragma once

#include "sani/engine/services/contracts/renderable_manager_contract.hpp"
#include "sani/core/memory/page_pool_allocator.hpp"
#include "sani/engine/services/engine_service.hpp"

#include <vector>

using namespace sani::engine::services::renderablemanager;

namespace sani {

	namespace engine {
		
		namespace services {

			template<class T>
			class RenderableManager : public EngineService {
			private:
				const ElementType type;

				PagePoolAllocator<T> allocator;
				
				std::vector<T*> elementsToUpdate;
				std::vector<T*> elements;

				void updateContainers();

				void handleDocumentMessage(messages::DocumentMessage* const message);
				void handleCommandMessage(messages::CommandMessage* const message);

				void createElement(messages::DocumentMessage* const message);
				void deleteElement(messages::DocumentMessage* const message);
				void createElement(messages::CommandMessage* const message);

				void getElements(messages::DocumentMessage* const message);
			protected:
				RenderableManager(const String& name, const ElementType type, engine::SaNiEngine* const engine);
			public:
				void receive(messages::Message* const message) final override;
				void update(const SaNiEngine& time) final override;
			};
		}
	}
}

#include "sani/engine/services/impl/renderable_manager.hpp"