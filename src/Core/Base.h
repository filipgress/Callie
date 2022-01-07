#pragma once

#include <Core/PlatformDetection.h>

#define CL_DEBUG
#ifdef CL_DEBUG
	#if defined(CL_PLATFORM_WINDOWS)
		#define CL_DEBUGBREAK() __debugbreak()
	#elif defined(CL_PLATFORM_LINUX)
		#include <signal.h>
		#define CL_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define CL_ASSERT(check) if(!(check)) CL_DEBUGBREAK()
#else
	#define CL_DEBUGBREAK()
	#define CL_ASSERT(check)
#endif

#define BIT(x) (1 << x)