#pragma once

inline MemPtr OriginalHookNativeFunc = nullptr;
inline enum class GameVersion {
  UNKNOWN,
  V120E,
  V124E,
} g_gameVersion;
