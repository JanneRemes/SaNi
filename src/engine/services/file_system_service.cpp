#include "sani/engine/messaging/messages/document_message.hpp"
#include "sani/engine/services/file_system_service.hpp"
#include "sani/engine/sani_engine.hpp"

namespace sani {

	namespace engine {
		
		namespace services {

			FileSystemService::FileSystemService(SaNiEngine* const engine) : EngineService("file system service", engine) {
			}

			void FileSystemService::handleDocumentMessage(messages::DocumentMessage* const message) {
			}

			void FileSystemService::fileExists(messages::DocumentMessage* const message) {
			}
			void FileSystemService::openFile(messages::DocumentMessage* const message) {
			}

			void FileSystemService::isAbsolutePath(messages::DocumentMessage* const message) {
			}

			void FileSystemService::getFileData(messages::DocumentMessage* const message) {
			}
			void FileSystemService::getFileDataString(messages::DocumentMessage* const message) {
			}

			void FileSystemService::listFiles(messages::DocumentMessage* const message) {
			}

			void FileSystemService::handleStateMessage(StateMessage* const message) {
			}

			void FileSystemService::receive(messages::Message* const message) {
			}
			void FileSystemService::update(const EngineTime& time) {
			}
		}
	}
}