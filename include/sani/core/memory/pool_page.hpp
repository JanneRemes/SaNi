#pragma once

#include "sani/core/memory/memory.hpp"
#include "sani/types.hpp"
#include <stack>

namespace sani {

	template<class T>
	/// @class PoolPage pool_page.hpp "sani/core/memory/pool_page.hpp"
	/// @author voidbab
	///
	/// Represents a generic memory page used by the pool allocator.
	class PoolPage {
	private:
		IntPtr lowAddress;
		IntPtr highAddress;

		const uint32 size;
		
		uint64 bytesUsed;
		uint32 poolpointer;

		std::stack<char*> releasedHandles;
		char* memory;
	public:
		PoolPage(const uint32 size);

		/// Returns true if the given address is located
		/// in the pages address space.
		inline bool isInAddressSpace(const IntPtr address);
		/// Returns true if the page can be used for allocations.
		inline bool canAllocate() const;

		/// Attempts to allocate. If this page can't be used
		/// for allocations, a null pointer will be returned.
		inline T* allocate();
		inline T* allocate(const uint32 length);
		/// Deallocates the given element and calls it's 
		/// destructor.
		inline void deallocate(T* element);
		inline void deallocate(T* elements, const uint32 length);

		uint64 getBytesUsed() const;

		~PoolPage();

		PoolPage(PoolPage& other) = delete;
		PoolPage(PoolPage&& other) = delete;

		PoolPage& operator =(PoolPage& other) = delete;
		PoolPage& operator =(PoolPage&& other) = delete;
	};
}

#include "sani/core/memory/impl/pool_page.hpp"
#include "sani/core/memory/inl/pool_page.inl"