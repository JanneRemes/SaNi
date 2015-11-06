#pragma once
#include "sani/forward_declare.hpp"
#include "sani/types.hpp"
SANI_FORWARD_DECLARE_3(sani, resource, compiler, ResourceWriter);
namespace sani {
	namespace resource {
		namespace compiler {
			class ResourceTypeWriter {
			public:
				/// Default constructor
				ResourceTypeWriter() {}
				/// Default destructor
				virtual ~ResourceTypeWriter() {}

				/// Writes the data type to stream
				/// @param writer The content writer which has stream open
				virtual void write(ResourceWriter* writer, const void* value) = 0;

				virtual String getRuntimeReader() const = 0;
			};
		}
	}
}