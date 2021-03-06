#include "sani/engine/services/contracts/file_system_service_contract.hpp"
#include "sani/engine/messaging/messages/query_message.hpp"
#include "sani/engine/messaging/messages/command_message.hpp"

#include <sstream>

namespace sani {

	namespace engine {
		
		namespace services {
			
			namespace filesystemservice {

				template <class T>
				static void decorateMessage(T* const message, const FilesystemServiceCommands filesystemServiceCommands) {
					const uint32 command = static_cast<uint32>(filesystemServiceCommands);

					message->getRecipients().addRecipient("file system service");
					message->setCommand(command);
				}

				void fileExists(messages::QueryMessage* const message, const String& path) {
					decorateMessage<messages::QueryMessage>(message, FilesystemServiceCommands::FileExists);
					message->setContents(path);
				}

				void openFile(messages::QueryMessage* const message, const String& path, const io::Filemode filemode) {
					decorateMessage<messages::QueryMessage>(message, FilesystemServiceCommands::OpenFile);
					
					String contents = path;
					contents += "||";
					contents += std::to_string(static_cast<uint32>(filemode));
					
					message->setContents(contents);
				}

				void closeFile(messages::CommandMessage* const message, const String& path) {
					decorateMessage<messages::CommandMessage>(message, FilesystemServiceCommands::CloseFile);
					message->setData(path);
				}

				void isAbsolutePath(messages::QueryMessage* const message, const String& path) {
					decorateMessage<messages::QueryMessage>(message, FilesystemServiceCommands::IsAbsolutePath);
					message->setContents(path);
				}

				void getFileDataString(messages::QueryMessage* const message, const String& path) {
					decorateMessage<messages::QueryMessage>(message, FilesystemServiceCommands::GetFileDataString);
					message->setContents(path);
				}

				void listFiles(messages::QueryMessage* const message, const String& path) {
					decorateMessage<messages::QueryMessage>(message, FilesystemServiceCommands::ListFiles);
					message->setContents(path);
				}

				void getFileData(messages::QueryMessage* const message, const String& path) {
					decorateMessage<messages::QueryMessage>(message, FilesystemServiceCommands::GetFileData);
					message->setContents(path);
				}
			}
		}
	}
}