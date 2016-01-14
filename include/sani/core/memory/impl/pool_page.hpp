#include "sani/core/memory/pool_page.hpp"

namespace sani {

	template <class T>
	PoolPage<T>::PoolPage(const int32 size) : size(size * sizeof(T)), 
											  poolpointer(0) {
		memory = new char[size * sizeof(T)];

		lowAddress = reinterpret_cast<IntPtr>(&memory[0]);
		highAddress = reinterpret_cast<IntPtr>(&memory[size - 1]);
	}

	template <class T>
	PoolPage<T>::~PoolPage() {
		delete[] memory;
	}
}