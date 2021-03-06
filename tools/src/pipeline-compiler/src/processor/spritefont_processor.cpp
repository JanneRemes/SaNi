#include "sani/resource/processor/spritefont_processor.hpp"
#include "sani/resource/font_description.hpp"
#include <ft2build.h>
#include <algorithm>
#include <numeric>
#include "sani/resource/spritefont_content.hpp"
#include "sani/core/math/rectangle.hpp"
#include "sani/resource/bitmap_content.hpp"
#include "sani/core/math/vector4.hpp"

#include FT_FREETYPE_H
#include FT_STROKER_H
#include FT_GLYPH_H

namespace sani {

	namespace resource {

		namespace processor {

			static FT_Library library;

			SpriteFontProcessor::SpriteFontProcessor() {
			}

			SpriteFontProcessor::~SpriteFontProcessor() {
			}

			class Glyph {
			public:
				BitmapContent* pixels;
				unsigned long character;
				float32 bearingX;
				float32 bearingY;
				float32 advance;
				sani::math::Rect32i source;
			public:
				Glyph() : pixels(nullptr), character('\0') {}
			};

			struct Span {
				Span() { }
				Span(int _x, int _y, int _width, int _coverage)
					: x(_x), y(_y), width(_width), coverage(_coverage) { }

				int x, y, width, coverage;
			};

			static void rasterSpan(const int y, const int count, const FT_Span * const spans, void * const user) {
				std::vector<Span>* sptr = (std::vector<Span> *)user;
				for (int i = 0; i < count; ++i) {
					sptr->push_back(Span(spans[i].x, y, spans[i].len, spans[i].coverage));
				}
			}

			static void renderSpans(FT_Outline* outline, std::vector<Span>* spans) {
				FT_Raster_Params params;
				memset(&params, 0, sizeof(params));
				params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
				params.gray_spans = rasterSpan;
				params.user = spans;

				FT_Outline_Render(library, outline, &params);
			}

			struct Rect {
				Rect() { }
				Rect(float left, float top, float right, float bottom)
					: xmin(left), xmax(right), ymin(top), ymax(bottom) { }

				void include(const sani::math::Vec2f &r) {
					xmin = std::min(xmin, r.x);
					ymin = std::min(ymin, r.y);
					xmax = std::max(xmax, r.x);
					ymax = std::max(ymax, r.y);
				}

				float width() const { return xmax - xmin + 1; }
				float height() const { return ymax - ymin + 1; }

				float xmin, xmax, ymin, ymax;
			};

			static void renderOutline(FT_Face face, const int32 outlineWidth, std::vector<Span>& outlineSpans) {
				FT_Stroker stroker = nullptr;
				FT_Glyph glyph = nullptr;
				FT_Outline* outline = nullptr;

				// setup stroker
				FT_Stroker_New(library, &stroker);

				FT_Stroker_Set(stroker,
					outlineWidth,
					FT_STROKER_LINECAP_ROUND,
					FT_STROKER_LINEJOIN_ROUND,
					0);

				if (FT_Get_Glyph(face->glyph, &glyph)){
					throw std::runtime_error("Cant get glyph");
				}

				FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
				if (glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
					throw std::runtime_error("Not outline");
				}

				// render the outline spans to the span list
				outline = &reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
				renderSpans(outline, &outlineSpans);

				// Clean up afterwards.
				FT_Stroker_Done(stroker);
				FT_Done_Glyph(glyph);
			}

			static Rect buildImageRect(const std::vector<Span>& spans) {
				Rect rect(
					static_cast<float>(spans.front().x),
					static_cast<float>(spans.front().y),
					static_cast<float>(spans.front().x),
					static_cast<float>(spans.front().y)
					);

				for (auto& s : spans) {
					rect.include(sani::math::Vec2i(s.x, s.y));
					rect.include(sani::math::Vec2i(s.x + s.width - 1, s.y));
				}
				return rect;
			}

			static void renderSpansToBitmap(const std::vector<Span>& spans, const Rect& imgRect, PixelBitmapContent<sani::math::Vector4<unsigned char>>* to) {
				const int32 imgWidth = static_cast<int32>( imgRect.width());
				const int32 imgHeight = static_cast<int32>(imgRect.height());
				for (const Span& s : spans) {
					for (int w = 0; w < s.width; ++w) {
						int32 index = (int)((imgHeight - 1 - (s.y - imgRect.ymin)) * imgWidth + s.x - imgRect.xmin + w);
						to->setPixel(index, sani::math::Vector4<unsigned char>(255, 255, 255, s.coverage)); // white							
					}
				}
				to->flipVertical();
			}

			static void importGlyph(FontDescription* desc, unsigned short character, FT_Face face, Glyph& out) {
				FT_UInt gindex = FT_Get_Char_Index(face, character);
				
				std::vector<Span> spans;

				if (FT_Load_Glyph(face, gindex, FT_LOAD_NO_BITMAP)) {
					throw std::runtime_error("FT_Load_Glyph failed");
				}

				if (face->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
					throw std::runtime_error("No outline");
				}

				out.character = character;
				out.bearingX = static_cast<float32>(face->glyph->metrics.horiBearingX >> 6); // left
				out.bearingY = static_cast<float32>(face->glyph->metrics.horiBearingY >> 6); // top
				out.advance = static_cast<float32>(face->glyph->advance.x >> 6); // offset

				if (desc->getOutlineType() == OutlineType::None) {
					renderSpans(&face->glyph->outline, &spans);

					// Its empty, like space etc.
					if (spans.empty()) {
						// create empty bitmap
						out.pixels = new PixelBitmapContent<sani::math::Vector4<unsigned char>>(0u, 0u);
						return;
					}

					Rect rect = buildImageRect(spans);
					// Get some metrics of our image.
					uint32 imgWidth = static_cast<uint32>(rect.width()),
						imgHeight = static_cast<uint32>(rect.height()),
						imgSize = imgWidth * imgHeight;

					PixelBitmapContent<sani::math::Vector4<unsigned char>>* pixels = new PixelBitmapContent<sani::math::Vector4<unsigned char>>(imgWidth, imgHeight);
					renderSpansToBitmap(spans, rect, pixels);

					out.pixels = pixels;

				}
				else if (desc->getOutlineType() == OutlineType::Outer) {

					int32 outlineWidth = static_cast<int32>(desc->getOutlineWidth() * 64.f);
					
					std::vector<Span> outlineSpans;

					renderSpans(&face->glyph->outline, &spans);

					renderOutline(face, outlineWidth, outlineSpans);

					// Its empty, like space etc.
					if (spans.empty()) {
						// create empty bitmap
						out.pixels = new PixelBitmapContent<sani::math::Vector4<unsigned char>>(0u, 0u);
						return;
					}

					Rect rect = buildImageRect(spans);

					// TODO figure out something for this
					for (auto& s : outlineSpans) {
						rect.include(sani::math::Vec2i(s.x, s.y));
						rect.include(sani::math::Vec2i(s.x + s.width - 1, s.y));
					}

					// Get some metrics of our image.
					uint32 imgWidth = static_cast<uint32>(rect.width()),
						imgHeight = static_cast<uint32>(rect.height()),
						imgSize = imgWidth * imgHeight;

					PixelBitmapContent<sani::math::Vector4<unsigned char>>* pixels = new PixelBitmapContent<sani::math::Vector4<unsigned char>>(imgWidth, imgHeight);
					
					// TODO normal spans too
					renderSpansToBitmap(outlineSpans, rect, pixels); 
					out.pixels = pixels;
				}
				else {
					throw std::logic_error("not impl");
				}
			}

			static FT_Face createFontFace(FontDescription* desc) {
				FT_Face face = nullptr;

				FT_Error error = FT_New_Face(library, desc->getFontPath().c_str(), 0, &face);

				if (error == FT_Err_Unknown_File_Format) {
					throw std::runtime_error("Invalid font file format!");
				}
				else if (error) {
					throw std::runtime_error("Font file could not be read");
				}
				const uint32 size = static_cast<uint32>(desc->getSize());
				const uint32 dpi = 96;
				FT_Set_Char_Size(face, 0, size << 6, dpi, dpi);
				return face;
			}

			static uint32 calculateOutputWidth(const std::vector<Glyph>& glyphs) {
				return std::accumulate(glyphs.begin(), glyphs.end(), 0, [](uint32 r, const Glyph& g) {
					if (g.pixels->getWidth() == 0u) {
						return r;
					}

					return 1 + r + g.pixels->getWidth() + 1;
				});
			}

			static uint32 calculateOutputHeight(const std::vector<Glyph>& glyphs) {
				// TODO max width shaiba
				return std::max_element(glyphs.begin(), glyphs.end(), [](const Glyph& a, const Glyph& b) {
					return a.pixels->getHeight() < b.pixels->getHeight();
				})->pixels->getHeight();
			}

			static BitmapContent* packGlyphs(std::vector<Glyph>& glyphs) {
				uint32 outputWidth = calculateOutputWidth(glyphs);
				uint32 outputHeight = calculateOutputHeight(glyphs);


				BitmapContent* bitmap = new PixelBitmapContent<sani::math::Vector4<unsigned char>>(outputWidth, outputHeight);

				uint32 xOffset = 0u;
				// TODO add max width
				uint32 yOffset = 0u;

				for (auto& glyph : glyphs) {
					BitmapContent* pixels = glyph.pixels;

					// white space and stuff
					if (pixels->getWidth() == 0 || pixels->getHeight() == 0) {
						glyph.source = math::Rect32i{ 0, 0, 0, 0 };
						continue;
					}

					uint32 width = pixels->getWidth();
					uint32 height = pixels->getHeight();
					sani::math::Rect32i source(0, 0, width, height);
					sani::math::Rect32i destination(1 + xOffset, yOffset, width, height);
					
					glyph.source = destination;
					bitmap->copyFrom(pixels, source, destination);
					xOffset += width;

					delete pixels;
				}

				return bitmap;

			}

			ResourceItem* SpriteFontProcessor::process(ResourceItem* input) {
				if (FT_Init_FreeType(&library)) {
					throw std::runtime_error("Failed to initialize freetype");
				}

				FontDescription* desc = static_cast<FontDescription*>(input);

				// import the actual font now
				FT_Face face = createFontFace(desc);
			
				const std::vector<unsigned short>& characters = desc->getCharacters();
				std::vector<Glyph> glyphs;
				glyphs.reserve(characters.size());

				for (auto character : characters) {
					Glyph glyph;
					importGlyph(desc, character, face, glyph);
					glyphs.push_back(glyph);
				}

				BitmapContent* bitmap = packGlyphs(glyphs);
				
				std::vector<GlyphContent> glyphContent;
				glyphContent.reserve(characters.size());
				for (auto& glyph : glyphs) {
					glyphContent.push_back(GlyphContent{
						glyph.character,
						glyph.source,
						glyph.bearingX,
						glyph.bearingY,
						glyph.advance
					});
				}

				// font height
				float lineSpacing = static_cast<float>(face->size->metrics.height >> 6);

				SpriteFontContent* output = new SpriteFontContent(desc, bitmap, glyphContent, lineSpacing);

				FT_Done_Face(face);
				FT_Done_FreeType(library);

				return output;
			}
		}
	}
}