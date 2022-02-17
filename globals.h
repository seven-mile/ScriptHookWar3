#pragma once

inline MemPtr g_pVM = nullptr, g_pCodeRel = nullptr;
inline struct reverse_table* g_pSymTable;
inline MemPtr OriginalHookNativeFunc = nullptr;
inline enum class GameVersion {
  UNKNOWN,
  V120E,
  V124E,
} g_gameVersion;
