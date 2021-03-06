#pragma once

#include "sani/engine/mono/mono_define.hpp"
#include "sani/engine/services/engine_service.hpp"
#include "sani/types.hpp"

#include <mono\metadata\object.h>


namespace sani {

	namespace engine {
		
		namespace services {

			enum class HookFlags : uint32 {
				None			= 0,
				OnStart			= 1,
				OnResume		= 2,
				OnSuspended		= 4,
				OnTerminated	= 8,
				OnUpdate		= 16
			};

			HookFlags operator &(HookFlags lhs, const HookFlags rhs);
			HookFlags& operator &=(HookFlags& lhs, const HookFlags rhs);

			HookFlags operator |(HookFlags lhs, const HookFlags rhs);
			HookFlags& operator |=(HookFlags& lhs, const HookFlags rhs);

			class UserService final : public EngineService {
			private:
				MonoObject* const instance;					// Mono instance.
				
				HookFlags flags;
			protected:
				// If some one calls these inside the C++ side of the engine, 
				// we need to invoke C# versions of the
				// functions if the user has defined hooks for them.
				virtual bool onStart() final override;
				virtual bool onResume() final override;
				virtual void onSuspend() final override;
				virtual void onTerminate() final override;
			public:
				UserService(MonoObject* const instance, MonoString* const name, SaNiEngine* const engine);
			
				MonoObject* const getMonoInstance() const;
				void setMonoHooks(const HookFlags flags);
	
				virtual void update(const EngineTime& time) final override;

				~UserService() = default;
			};
		}
	}
}