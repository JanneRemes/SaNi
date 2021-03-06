#pragma once

#include "sani/resource/processor/resource_processor.hpp"

namespace sani {

	namespace resource {

		namespace processor {

			class SpriteFontProcessor : public ResourceProcessor {
			public:
				SpriteFontProcessor();
				~SpriteFontProcessor();

				ResourceItem* process(ResourceItem* input) override;
			};
		}
	}
}