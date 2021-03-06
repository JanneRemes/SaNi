#include "sani/resource/reader/resource_reader.hpp"
#include "sani/platform/file/file_stream.hpp"
#include "sani/resource/reader/resource_type_reader.hpp"
#include "sani/core/logging/log.hpp"
#include <sstream>

namespace sani {

	namespace resource {

		namespace reader {

			ResourceReader::ResourceReader(FileStream* stream, ResourceManager& manager, GraphicsDevice* device) : BinaryReader(stream), 
																												   manager(manager), 
																												   graphicsDevice(device) {
			}

			ResourceReader::~ResourceReader() {
			}

			ResourceManager& ResourceReader::getResourceManager() const {
				return manager;
			}

			GraphicsDevice* ResourceReader::getGraphicsDevice() const {
				return graphicsDevice;
			}

			void* ResourceReader::readAsset(const ResoureTypeReaderManager& typeReaders) {
				// Ensure its SNB file
				char magic[4] = { 0 };
				magic[0] = readByte();
				magic[1] = readByte();
				magic[2] = readByte();
				
				if (strcmp(magic, "SNB") != 0) {
					FNCLOG_ERR(log::OutFlags::All, "not SNB file");

					std::abort();
				}

				// read platform
				readByte();
				// read version
				readByte();

				// type readers are initialized, get the list of used type readers
				// read the count of type readers
				uint32 typeReaderCount = static_cast<uint32>(read7BitEncodedInt());

				readers.reserve(typeReaderCount);

				// read the strings
				for (size_t i = 0; i < typeReaderCount; ++i) {
					String typeReaderName = readString();
					// get the type reader from manager and append it to our list
					ResourceTypeReader* reader = typeReaders.getReaderByName(typeReaderName);

					if (reader == nullptr) {
						std::stringstream ss;
						ss << "no reader for \"";
						ss << typeReaderName;
						ss << "\"";

						FNCLOG_ERR(log::OutFlags::All, ss.str());

						std::abort();
					}

					readers.push_back(reader);
				}

				uint32 index = static_cast<uint32>(read7BitEncodedInt());
				
				return (*readers[index]).read((this));
			}

			sani::math::Rectangle<int32> ResourceReader::readRectangle()  {
				int32 x = readInt32();
				int32 y = readInt32();
				int32 w = readInt32();
				int32 h = readInt32();
				return sani::math::Rectangle<int32>(x, y, w, h);
			}

			sani::math::Matrix4<float32> ResourceReader::readMatrix() {
				return sani::math::Matrix4<float32>(
					readSingle(), readSingle(), readSingle(), readSingle(),
					readSingle(), readSingle(), readSingle(), readSingle(),
					readSingle(), readSingle(), readSingle(), readSingle(),
					readSingle(), readSingle(), readSingle(), readSingle()
					);
			}
		}
	}
}