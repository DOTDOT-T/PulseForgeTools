#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef BUILDING_DLL
    #define MA_DLL_API __declspec(dllexport)
  #else
    #define MA_DLL_API __declspec(dllimport)
  #endif
#else
  #define MA_DLL_API
#endif