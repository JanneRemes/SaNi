#include "sani/engine/services/contracts/entity_manager_contract.hpp"
#include "sani/engine/messaging/messages/document_message.hpp"
#include "sani/engine/messaging/messages/query_message.hpp"

namespace sani {

	namespace engine {

		namespace services {

			namespace entitymanager {

				void createEntity(messages::DocumentMessage* const message) {
					message->getRecipients().addRecipient("entity manager");

					message->setCommand(static_cast<uint32>(EntityManagerCommands::CreateEntity));
				}

				void destroyEntity(messages::QueryMessage* const message, const uint32 id) {
					message->getRecipients().addRecipient("entity manager");

					message->setCommand(static_cast<uint32>(EntityManagerCommands::DestroyEntity));
					message->setContents(std::to_string(id));
				}

				void listEntities(messages::DocumentMessage* const message) {
					message->getRecipients().addRecipient("entity manager");

					message->setCommand(static_cast<uint32>(EntityManagerCommands::ListEntities));
				}
			}
		}
	}
}