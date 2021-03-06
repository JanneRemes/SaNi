#include "sani/core/logging/log.hpp"
#include "sani/core/io/memory_stream.hpp"
#include "sani/debug.hpp"

namespace sani {
	namespace io{

		MemoryStream::MemoryStream() 
			: dataSize(32u), buffer(nullptr), position(0u), elements(0u) {
			buffer = static_cast<unsigned char*>(std::realloc(buffer, dataSize));
		}

		MemoryStream::~MemoryStream() {
			free( buffer);
		}

		uint32 MemoryStream::read(unsigned char* outBuffer, const uint32 size)  {
			std::memcpy(outBuffer, buffer + position, size);
			position += size;
			return size;
		}

		uint32 MemoryStream::read(unsigned char* buffer, const int32 offset, const uint32 size)  {
			FNCLOG_ERR(log::OutFlags::All, "not implemented");

			std::abort();
		}

		uint32 MemoryStream::write(const unsigned char* data, const uint32 size)  {
			if ((position + size) >= dataSize) {
				dataSize = (dataSize) * 2u + size;
				buffer = static_cast<unsigned char*>(std::realloc(buffer, dataSize));
			}
			std::memcpy(buffer + position, data, size);
			elements += size;
			position += size;
			return size;
		}

		void MemoryStream::seek(const SeekPosition seekpos, const long offset)  {
			if (seekpos == SeekPosition::Begin) {
				position = offset;
			}
			else if (seekpos == SeekPosition::End) {
				position = size() + offset;
			}
			else {
				position += offset;
			}
		}

		uint32 MemoryStream::size()  {
			return elements;
		}

		void MemoryStream::flush() {
			FNCLOG_ERR(log::OutFlags::All, "not implemented");

			std::abort();
		}
	}
}