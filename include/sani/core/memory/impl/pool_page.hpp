#include "sani/core/memory/pool_page.hpp"

namespace sani {

	template <class T>
	PoolPage<T>::PoolPage(const uint32 size) : size(size * sizeof(T)), 
											   poolpointer(0),
											   bytesUsed(0) {
		memory = new char[this->size];

		lowAddress = reinterpret_cast<IntPtr>(&memory[0]);
		highAddress = reinterpret_cast<IntPtr>(&memory[size - 1]);
	}

	template <class T>
	uint64 PoolPage<T>::getBytesUsed() const {
		return bytesUsed;
	}

	template <class T>
	PoolPage<T>::~PoolPage() {
		delete[] memory;
	}
}