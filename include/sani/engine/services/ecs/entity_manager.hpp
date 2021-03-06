#pragma once

#include "sani/core/memory/register_allocator.hpp"
#include "sani/engine/services/engine_service.hpp"
#include "sani/forward_declare.hpp"

SANI_FORWARD_DECLARE_1(sani, Entity);
SANI_FORWARD_DECLARE_3(sani, engine, messages, DocumentMessage);

namespace sani {

	namespace engine {

		namespace services {

			/// @class EntityManager entity_manager.hpp "sani/engine/services/cvar_service.hpp"
			/// @author voidbab
			///
			/// Service responsible of managing the entities.
			class EntityManager final : public EngineService {
			private:
				RegisterAllocator<Entity> allocator;

				void handleDocumentMessage(messages::DocumentMessage* const message);

				void createEntity(messages::DocumentMessage* const message);
				void destroyEntity(messages::DocumentMessage* const message);

				void listEntities(messages::DocumentMessage* const message);
			public:
				EntityManager(SaNiEngine* const engine);

				virtual void receive(messages::Message* const message) final override;

				~EntityManager() = default;
			};
		}
	}
}