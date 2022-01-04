#pragma once

#include <iostream>
#include <sstream>

#include <Core/PlatformDetection.h>

#define CL_DEBUG
#ifdef CL_DEBUG
	#if defined(CL_PLATFORM_WINDOWS)
		#define CL_DEBUGBREAK(x) if(!(x)) __debugbreak()
	#elif defined(CL_PLATFORM_LINUX)
		#include <signal.h>
		#define CL_DEBUGBREAK(x) if(!(x)) raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#else
	#define CL_DEBUGBREAK()
#endif

#define BIT(x) (1 << x)