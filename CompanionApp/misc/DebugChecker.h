#pragma once


#ifdef NDEBUG
inline constexpr bool IS_DEBUG = false;
#else
inline constexpr bool IS_DEBUG = true;
#endif
