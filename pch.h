#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files
#include <windows.h>

#include <cassert>
#include <concepts>
#include <vector>
#include <string>
#include <functional>
#include <type_traits>
#include <format>

#include "memory_helper.h"
#include "globals.h"
#include "war3/enum.h"

// disable static_assert for my MSVC, it seems
// the new version has a bug on it.
// if you're not using VC toolset 14.31.31103(VS 17.1 preview5), COMMENT IT!
#define static_assert(...) 

#endif //PCH_H
