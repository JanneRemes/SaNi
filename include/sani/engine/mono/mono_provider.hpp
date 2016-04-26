#pragma once

#include <mono\jit\jit.h>
#include <mono\metadata\assembly.h>

#include "sani/forward_declare.hpp"

#include "sani/types.hpp"

#include <vector>

SANI_FORWARD_DECLARE_STRUCT(MonoClassDefinition);
SANI_FORWARD_DECLARE_STRUCT(MonoFunctionDefinition);
SANI_FORWARD_DECLARE_STRUCT(MonoFieldDefinition);

namespace sani {

	namespace engine {

		class MonoProvider final {
		private:
			std::vector<MonoAssembly*> assemblies;
		public:
			MonoProvider(std::vector<MonoAssembly*>& assemblies);
			
			MonoClass* classFromDefinition(const MonoClassDefinition* const classDef) const;
			MonoType* typeFromDefinition(const MonoClassDefinition* const classDef) const;

			MonoObject* readField(MonoObject* instance, const char* const name);
			void writeField(MonoObject* instance, const char* const name, void* value);

			bool fieldExists(const MonoClassDefinition* const classDef, const char* const name);
			bool classExists(const MonoClassDefinition* const classDef) const;
			bool functionExists(const MonoClassDefinition* const classDef, const MonoFunctionDefinition* const funcDef) const;

			void addInternalCall(const MonoClassDefinition* const classDef, const MonoFunctionDefinition* const funcDef) const;

			MonoObject* createObject(const MonoClassDefinition* const classDef) const;
			MonoObject* createObject(const MonoClassDefinition* const classDef, void** args, const uint32 argc) const;

			MonoString* createString(const char* const str) const;

			MonoObject* invoke(MonoObject* instance, const char* const name, void** args, const int32 argc) const;
			MonoObject* invoke(MonoObject* instance, const char* const name) const;

			~MonoProvider() = default;

			MonoProvider& operator =(MonoProvider& other) = delete;
			MonoProvider& operator =(MonoProvider&& other) = delete;

			MonoProvider(MonoProvider& other) = delete;
			MonoProvider(MonoProvider&& other) = delete;
		};
	}
}