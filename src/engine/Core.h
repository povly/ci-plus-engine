#pragma once

#if defined(_WIN32)
    #if defined(POVLY_BUILD_DLL)
        #define POVLY_API __declspec(dllexport)
    #else
        #define POVLY_API __declspec(dllimport)
    #endif
#else
    #define POVLY_API
#endif
