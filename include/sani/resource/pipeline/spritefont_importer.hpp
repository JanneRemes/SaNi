#pragma once
#include "sani/resource/pipeline/content_importer.hpp"
#include "sani/forward_declare.hpp"
#include <vector>

SANI_FORWARD_DECLARE_2(sani, resource, ResourceItem);
SANI_FORWARD_DECLARE_2(sani, io, FileSystem);

namespace sani {
	namespace resource {
		namespace pipeline {
			class SpriteFontImporter : public ContentImporter {
			private:
			public:
				SpriteFontImporter();
				~SpriteFontImporter();
				ResourceItem* import(const String& filename, io::FileSystem* fileSystem) const override;
				virtual String getImporterName() const {
					return "sani::resource::pipeline::SpriteFontImporter";
				}
			};
		}
	}
}