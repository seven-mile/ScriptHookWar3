#include "../pch.h"
#include "func_callback.h"
#include "hash_table.h"
#include "native_table.h"
#include "jass_vm.h"
#include "game_end_event.h"

// for passing c++ functions
static size_t callbackTopCount = 0;
static std::unordered_map<size_t, std::function<void()>> callbackMap;

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
  /* optional code injection for ignore register availability check */
  static bool initedRegCheat = false;
  if (!initedRegCheat) {
    initedRegCheat = true;
    if (g_gameVersion == GameVersion::V124E) {
      // hack reg availability
      // 1. set vm + 0x48 to 0 [selected]
      //MemWrite(g_pVM.address + 0x48, 0);

      // 2. ignore unavailablity
      // Game.dll+45F288 - 75 05 - jne Game.dll+45F28F
      // Game.dll+45F288 - EB 05 - jmp Game.dll+45F28F

      MemPtr jneAsmcode{ (size_t)GetModuleHandle(L"Game.dll") + 0x45F288 };
      MemWrite<char>(jneAsmcode, static_cast<char>(0xEB));
    }
  }

  auto pNode = GetNativeFuncNode("IsUnitType");
  // assert(pNode && "Failed to install native call back hook!");

  if (pNode) {
    if (!OriginalHookNativeFunc.address) {
      OriginalHookNativeFunc = pNode->fnAddr;
    }
    pNode->fnAddr = (PROC)HookNativeIsUnitType;
    OutputDebugString(L"NativeTable Hook installed.\n");
  } else {
    // sometimes the native func table is not inited.
    // then we can ignore it, we will have another chance
    // to install the hook after its initialization.
    OutputDebugString(L"NativeTable Hook installation missed.\n");
  }

}

HCode CreateJassCallback(const std::function<void()>& callback) {
  static bool run_once = false;
  if (!run_once) {
    run_once = true;
    RegisterGameEndEvent([]() {
      callbackMap.clear();
      callbackTopCount = 0;
      InstallNativeCallbackHook();
    });
    InstallNativeCallbackHook();
  }

  size_t hookNativeFuncId = GetSymbolTable()->find("IsUnitType")->value;
  assert(hookNativeFuncId && "Invalid hook native func id!");

  callbackMap[++callbackTopCount] = callback;

  /*
  * Bytecode Template
  */
  auto bytecodeBuff = new size_t[]{
    0,
    0x13000000, 0x00000000,                 // push reg
    0x0C000400, callbackTopCount,           // mov reg, callbackTopCount
    0x13000000, 0x00000000,                 // push reg
    0x0C000400, HOOK_NATIVE_CALLBACK_MAGIC, // mov reg, HOOK_NATIVE_CALLBACK_MAGIC
    0x13000000, 0x00000000,                 // push reg
    0x15000000, hookNativeFuncId,           // call_native hookNativeFuncId
    0x14000000, 0x00000000,                 // pop reg
    0x27000000, 0x00000000,                 // ret
    0x04000000, 0x00000000,                 // end function
  };

  RegisterOnlyNextGameEndEvent([bytecodeBuff]() { delete bytecodeBuff; });
  
  bytecodeBuff[0] = MemPtr(bytecodeBuff).address + 4;
  return MemPtr((MemPtr(bytecodeBuff) - GetCurrentCodeRelativeAddr()) / 4).handle;
}
