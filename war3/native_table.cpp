#include "../pch.h"
#include "native_table.h"
#include "hash_table.h"
#include "jass_vm.h"

JassFnRegNode* GetNativeFuncNode(const char* name) {
  using func_t = JassFnRegNode * (__fastcall*)(const char* name);
  if (g_gameVersion == GameVersion::V120E) {
    static MemPtr fnGetNative = (size_t)::GetModuleHandle(L"Game.dll") + 0x69E010;
    return reinterpret_cast<func_t>(fnGetNative.pointer)(name);
  }
  if (g_gameVersion == GameVersion::V124E) {
    static MemPtr fnGetNative = (size_t)::GetModuleHandle(L"Game.dll") + 0x44F510;
    return reinterpret_cast<func_t>(fnGetNative.pointer)(name);
  }
  assert(false && "Invalid game version");
  return nullptr;
  // this is not usable, maybe because hash table is not inited defaultly.
  // return reinterpret_cast<JassFnRegNode*>(GetNativeFuncTable()->find(name));
}
