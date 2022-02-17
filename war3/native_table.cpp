#include "../pch.h"
#include "native_table.h"
#include "hash_table.h"

//std::unordered_map<std::string, JassFnRegNode*> *pMap = nullptr;

void RegenerateFuncMap() {
  if (g_gameVersion == GameVersion::V120E) {
    // do nothing
  } else if (g_gameVersion == GameVersion::V124E) {
    //delete pMap;
    //using map_t = decltype(*pMap);
    //pMap = new map_t{};
    
  } else {
    assert(false && "Invalid game version");
  }
}

JassFnRegNode* GetNativeFuncNode(const char* name) {
  //auto res = g_pSymTable->find(name);
  //if (!res) return nullptr;
  //return (JassFnRegNode*)res->value;
  // Game.dll + 69E010
  // Game.dll + 45DA10
  if (g_gameVersion == GameVersion::V120E) {
    static MemPtr fnGetNative = (size_t)::GetModuleHandle(L"Game.dll") + 0x69E010;
    return reinterpret_cast<JassFnRegNode* (__fastcall*)(const char* name)>
      (fnGetNative.pointer)(name);
  } else if (g_gameVersion == GameVersion::V124E) {
    static MemPtr fnGetNative = (size_t)::GetModuleHandle(L"Game.dll") + 0x44F510;
    auto k = reinterpret_cast<JassFnRegNode * (__fastcall*)(const char* name)>
      (fnGetNative.pointer)(name);
    return k;
  } else {
    assert(false && "Invalid game version");
  }
  return nullptr;
}
