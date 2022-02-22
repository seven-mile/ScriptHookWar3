#include "../pch.h"
#include "jass_vm.h"

MemPtr GetGInsFunc() {
  // GetInstance is useful for accessing internal resources
  // war3.exe#497 for 1.20e
  // 1.20e : Game.dll + AE5C
  // 1.24e : Game.dll + 4C3FD0
  if (g_gameVersion == GameVersion::V120E) {
    return GetProcAddress(::GetModuleHandle(nullptr), MAKEINTRESOURCEA(497));
  } else if (g_gameVersion == GameVersion::V124E) {
    return MemPtr(::GetModuleHandle(L"Game.dll")).address + 0x4C3FD0;
  } else {
    assert(false && "Invalid Game Version!");
    return nullptr;
  }
}

inline size_t GetInstance(int index)
{
  static auto func = GetGInsFunc();
  return reinterpret_cast<size_t(__fastcall*)(int)>(func.address)(index);
}

size_t GetCurrentVM(int index)
{
  return MemRead(MemRead(GetInstance(5) + 0x90) + 0x4 * index);
}

size_t GetCurrentCodeTable()
{
  const int offset = g_gameVersion > GameVersion::V120E ? 4 : 0;
  // 0x2884 for 1.20e, 0x2888 for higher
  return MemRead(GetCurrentVM() + 0x2884 + offset);;
}

size_t GetCurrentCodeRelativeAddr()
{
  return MemRead(GetCurrentCodeTable() + 0x8);
}

reverse_table* GetSymbolTable()
{
  const int offset = g_gameVersion > GameVersion::V120E ? 4 : 0;
  return reinterpret_cast<reverse_table*>
    (MemRead(MemRead(GetCurrentVM() + 0x2854 + offset) + 0x8));
}

table<>* GetNativeFuncTable()
{
  return reinterpret_cast<table<>*>(GetInstance(5) + 0x18);
}
