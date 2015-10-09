#pragma once

#include "sani/core/memory/heap_page.hpp"
#include "sani/types.hpp"
#include "sani/debug.hpp"
#include <list>

namespace sani {

	/// @class HeapAllocator heap_allocator.hpp "sani/core/memory/heap_allocator.hpp"
	/// @author voidbab
	///
	/// Basic heap allocator with defragmentation support.
	class HeapAllocator {
	private:
		const uint32 pageSize;

		std::list<HeapPage*> pages;

		void initialize(const uint32 initialPages);
	public:
		/// Creates new heap allocator with given page size and 
		/// with given count of initial pages.
		HeapAllocator(const uint32 pageSize, const uint32 initialPages);

		/// Creates new heap allocator with page size of 2mb (2097152-bytes)
		/// and with given count of initial pages.
		HeapAllocator(const uint32 initialPages);

		/// Creates new heap allocator with page-size of 2mb (2097152-bytes)
		/// and with initial page count of one.
		HeapAllocator();

		template<class T>
		inline T* allocate() {
			const size_t size = sizeof(T);

			// TODO: runtime assert this?
			SANI_ASSERT(size < pageSize);

			for (HeapPage* page : pages) {
				if (page->canAllocate(size)) return page->allocate<T>();
			}
			
			HeapPage* page = new HeapPage(pageSize);
			pages.push_back(page);

			return page->allocate<T>();
		}
		template<class T>
		inline bool deallocate(T* element) {
			for (HeapPage* page : pages) if (page->deallocate<T>(element)) return true;
			
			return false;
		}

		inline size_t pagesCount() const {
			return pages.size();
		}

		// TODO: implement defragmentation logic.

		~HeapAllocator();

		HeapAllocator& operator =(HeapAllocator& other) = delete;
		HeapAllocator& operator =(HeapAllocator&& other) = delete;

		HeapAllocator(HeapAllocator& other) = delete;
		HeapAllocator(HeapAllocator&& other) = delete;
	};
}