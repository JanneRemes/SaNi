#include "../bitmap_content.hpp"
#include "sani/debug.hpp"

namespace sani {

	namespace resource {

		inline const uint32 BitmapContent::getWidth() const {
			return width;
		}

		inline const uint32 BitmapContent::getHeight() const {
			return height;
		}

		template <class PixelType>
		PixelBitmapContent<PixelType>::PixelBitmapContent(uint32 width, uint32 height) 
			: BitmapContent(width, height), pixels(new PixelType[width * height]), format(graphics::SurfaceFormat::ColorRGBA) {
			// transparent
			memset(pixels, 0, sizeof(PixelType) * width * height);
			tryGetFormat(&format);
		}

		template <class PixelType>
		PixelBitmapContent<PixelType>::~PixelBitmapContent() {
			delete[] pixels;
		}

		template <class PixelType>
		void PixelBitmapContent<PixelType>::tryGetFormat(graphics::SurfaceFormat* out) const {
			using namespace sani::math;
			using namespace sani::graphics;

			if (typeid(PixelType) == typeid(Vector4<unsigned char>)) {
				*out = SurfaceFormat::ColorRGBA;
			}
		}

		template <class PixelType>
		void PixelBitmapContent<PixelType>::setPixel(uint32 index, const PixelType& value) {
			SANI_ASSERT(index < (width * height));

			pixels[index] = value;
		}

		template <class PixelType>
		void PixelBitmapContent<PixelType>::setPixel(uint32 y, uint32 x, const PixelType& pxl) {
			SANI_ASSERT(y < height && x < width);

			pixels[y * width + x] = pxl;
		}

		template <class PixelType>
		PixelType& PixelBitmapContent<PixelType>::getPixel(uint32 y, uint32 x) const {
			SANI_ASSERT(y < height && x < width);

			return pixels[y * width + x];
		}

		template <class PixelType>
		void sani::resource::PixelBitmapContent<PixelType>::flipVertical() {
			uint32 w = getWidth();
			uint32 h = getHeight();
			for (uint32 r = 0; r < (h / 2); r++) {
				for (uint32 c = 0; c != w; ++c) {
					std::swap(pixels[r * w + c], pixels[(h - 1 - r) * w + c]);
				}
			}
		}


		template <class PixelType>
		void PixelBitmapContent<PixelType>::copyFrom(BitmapContent* fromThis,
			const sani::math::Rect32i& sourceArea, const sani::math::Rect32i& destinationArea) {

			graphics::SurfaceFormat fmt;
			fromThis->tryGetFormat(&fmt);
			// TODOF It should be possible to copy stuff where the source and destination are different
			SANI_ASSERT(format == fmt && 
						sourceArea.w == destinationArea.w && 
						sourceArea.h == destinationArea.h);
		
			PixelBitmapContent<PixelType>* from = static_cast<PixelBitmapContent<PixelType>*>(fromThis);

			uint32 destY = destinationArea.y;
			uint32 destX = destinationArea.x;

			for (int y = sourceArea.y; y < sourceArea.y + sourceArea.h; ++y) {
				for (int x = sourceArea.x; x < sourceArea.x + sourceArea.w; ++x) {
					setPixel(destY, destX, from->getPixel(y, x));
					++destX;
				}

				++destY;
				destX = destinationArea.x;
			}
		}

		template <class PixelType>
		void PixelBitmapContent<PixelType>::getPixelData(std::vector<unsigned char>& outPixels) const {

			uint32 size = getWidth() * getHeight();
			
			// resize the vector to fit all pixels in bytes
			//outPixels.resize(size * sizeof(PixelType));
			// find the correct starting address and correct type
			unsigned char* ptr = (unsigned char*)pixels;
			outPixels.insert(
				// where to insert
				outPixels.begin(),
				// from pixels 
				ptr,
				// calculate the ending address in bytes of pixels
				ptr + size * sizeof(PixelType)
				);
		}

		template <class PixelType>
		void PixelBitmapContent<PixelType>::setPixelData(std::vector<unsigned char>& pixels) {
			uint32 size = getWidth() * getHeight();
			SANI_ASSERT(size * sizeof(PixelType) >= pixels.size());
			std::memcpy(this->pixels, pixels.data(), size * sizeof(PixelType));
		}
	}
}