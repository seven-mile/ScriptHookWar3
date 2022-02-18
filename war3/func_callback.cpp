#include "../pch.h"
#include "func_callback.h"
#include "hash_table.h"
#include "native_table.h"

// for passing c++ functions
size_t callbackTopCount = 0;
std::unordered_map<size_t, std::function<void()>> callbackMap;

// my hook native
constexpr int HOOK_NATIVE_CALLBACK_MAGIC = 0x3f3f3f3f;
size_t __cdecl HookNativeIsUnitType(size_t arg1, size_t arg2) {
  if (arg2 == HOOK_NATIVE_CALLBACK_MAGIC) {
    callbackMap[arg1]();
    return 0;
  } else {
    // return CallFn<bool>("IsUnitType", arg1, arg2);
    assert(OriginalHookNativeFunc);
    return reinterpret_cast<size_t(__cdecl*)(size_t, size_t)>
      (OriginalHookNativeFunc.address)(arg1, arg2);
  }
}

void InstallNativeCallbackHook() {

  static bool initedRegCheat = false;
  if (!initedRegCheat) {
    initedRegCheat = true;
    if (g_gameVersion == GameVersion::V124E) {
      // hack reg availability
      // 1. set vm + 0x48 to 0
      // 2. ignore unavailablity [selected]
      // Game.dll+45F288 - 75 05 - jne Game.dll+45F28F
      // Game.dll+45F288 - EB 05 - jmp Game.dll+45F28F

      MemPtr jneAsmcode{ (size_t)GetModuleHandle(L"Game.dll") + 0x45F288 };
      MemWrite<char>(jneAsmcode, 0xEB);
    }
  }

  auto pNode = GetNativeFuncNode("IsUnitType");
  assert(pNode && "Failed to install native call back hook!");

  if (!OriginalHookNativeFunc.address) {
    OriginalHookNativeFunc = pNode->fnAddr;
  }
  pNode->fnAddr = (PROC)HookNativeIsUnitType;
}

HCode CreateJassCallback(const std::function<void()>& callback) {
  // always try to install again.
  // todo: only install when the new game loads
  InstallNativeCallbackHook();

  size_t hookNativeFuncId = 0;
  // todo: query symbols hashtable
  hookNativeFuncId = g_pSymTable->find("IsUnitType")->value;

  assert(hookNativeFuncId && "Invalid hook native func id!");
  assert(g_pVM && g_pCodeRel && "Invalid VM or CodeRel pointer!");

  callbackMap[++callbackTopCount] = callback;

  /*
  * Bytecode Template
  */
  auto bytecodeBuff = new size_t[]{
    0,
    0x13C10000, 0x00000000,
    0x0CC10400, callbackTopCount,
    0x13C10000, 0x00000000,
    0x0CC10400, HOOK_NATIVE_CALLBACK_MAGIC,
    0x13C10000, 0x00000000,
    0x15000000, hookNativeFuncId,
    0x14C10000, 0x00000000,
    0x27000000, 0x00000000,
    0x04000000, 0x00000000,
  };

  assert((g_pCodeRel.address & 3) == 0);
  bytecodeBuff[0] = MemPtr(bytecodeBuff).address + 4;

  return MemPtr((MemPtr(bytecodeBuff) - g_pCodeRel.address) / 4).handle;
}

void DestroyJassCallback(HCode code) {
  // todo: create a code list, and destroy all codes when the game resets
  assert(g_pCodeRel && "Invalid code relative base address!");

  auto bytecodeBuff = g_pCodeRel.address + 4 * MemPtr(code).address;
  callbackMap.erase(bytecodeBuff + 4 * sizeof(size_t));
  delete reinterpret_cast<void*>(bytecodeBuff);
}
