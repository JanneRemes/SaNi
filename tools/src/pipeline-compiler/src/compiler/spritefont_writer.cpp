#include "sani/resource/compiler/resource_writer.hpp"
#include "sani/resource/spritefont_content.hpp"
#include "sani/resource/compiler/spritefont_writer.hpp"
#include "sani/resource/serialization.hpp"
#include "sani/resource/texture2d_content.hpp"
#include "sani/resource/bitmap_content.hpp"
#include "sani/resource/texture2d.hpp"

namespace sani {

	namespace resource {

		namespace compiler {

			SpriteFontWriter::SpriteFontWriter() : ResourceTypeWriter() {
			}

			SpriteFontWriter::~SpriteFontWriter() {
			}

			String SpriteFontWriter::getRuntimeReader() const {
				return DeserializableWith<SpriteFontContent>::value();
			}

			void SpriteFontWriter::write(ResourceWriter* writer, const void* value) {
				const SpriteFontContent* content = static_cast<const SpriteFontContent*>(value);

				// TODO BitmapContent and Texture2DContent should be the same?
				BitmapContent* texture = content->getTexture();
				std::vector<unsigned char> pixels;
				pixels.reserve(texture->getWidth() * texture->getHeight() * sizeof(math::Vector4<unsigned char>));
				texture->getPixelData(pixels);
				Texture2DContent texcontent(texture->getWidth(), texture->getHeight(), pixels);
				writer->writeObject(std::type_index(typeid(Texture2DContent)), &texcontent);

				using Glyphs = std::vector<GlyphContent>;
								
				const Glyphs& glyphs = content->getGlyphs();
				// write sources
				writer->write7BitEncodedInt(glyphs.size());
				
				for (const auto& glyph : glyphs) {
					writer->write(glyph.character);
					writer->writeRectangle(glyph.source);
					writer->write(glyph.xOffset);
					writer->write(glyph.yOffset);
					writer->write(glyph.xAdvance);
				}

				writer->write(content->getLineSpacing());
			}
		}
	}
}