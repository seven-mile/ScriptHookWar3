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

#endif //PCH_H
