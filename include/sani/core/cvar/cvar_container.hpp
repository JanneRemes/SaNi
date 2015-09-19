#pragma once

#include "sani/core/cvar/cvar_record.hpp"
#include "sani/core/cvar/cvar.hpp"
#include <list>

namespace sani {

	class CVarContainer {
	private:
		std::list<CVar> cvars;
	public:
		CVarContainer(const std::list<CVar>& cvars);

		bool contains(const String& name);
		CVar& find(const String& name);

		~CVarContainer();
	};
}