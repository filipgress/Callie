#pragma once

#include <Core/PlatformDetection.h>

#define CL_DEBUG
#ifdef CL_DEBUG
	#if defined(CL_PLATFORM_WINDOWS)
		#define CL_ENABLE_ASSERT
		#define CL_DEBUGBREAK() __debugbreak()
	#elif defined(CL_PLATFORM_LINUX)
		#define CL_ENABLE_ASSERT
		#include <signal.h>
		#define CL_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#endif

#ifdef CL_ENABLE_ASSERT
	#define CL_ASSERT(check) if(!(check)) CL_DEBUGBREAK()
	#define CL_CORE_ASSERT(...) ERROR("Assertion failed: ", __VA_ARGS__);\
		CL_DEBUGBREAK();
#else
	#define CL_DEBUGBREAK()
	#define CL_ASSERT(check)
	#define CL_CORE_ASSERT(message)
#endif

#define BIT(x) (1 << x)
#define CL_BIND_CALLBACK_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace cl{
	template<typename T>
	using Scope = std::unique_ptr<T>;
}