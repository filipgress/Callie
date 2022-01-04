#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
   #define CL_PLATFORM_WINDOWS
   #ifdef _WIN64
      //define something for Windows x64 (64-bit only)
   #else
      //define something for Windows x86 (32-bit only)
   #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
         // iOS, tvOS, or watchOS Simulator
         #error "IOS simulator is not supported!"
    #elif TARGET_OS_MACCATALYST
         // Mac's Catalyst (ports iOS API into Mac, like UIKit).
         #error "IOS api is not supported!"
    #elif TARGET_OS_IPHONE
        // iOS, tvOS, or watchOS device
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC
        // Other kinds of Apple platforms
        #define CL_PLATFORM_MACOS
        #error "MacOs not platform supported"
    #else
        #error "Unknown Apple platform!"
    #endif
#elif __linux__
    // linux
    #define CL_PLATFORM_LINUX
#elif __unix__ // all unices not caught above
    // Unix
    #error "This version of unix is not supported!"
#elif defined(_POSIX_VERSION)
    // POSIX
    #error "POSIX not supported!"
#else
    #error "Unknown compiler!"
#endif
