#include "pch.h"
#include "war3/jass_string.h"
#include "war3/hash_table.h"
#include "war3/native_table.h"
#include "war3/func_callback.h"
#include "war3/func_call.h"

// Define your custom script in script.cpp!
// This function will be called periodly, every ~10ms?
void WINAPI HookFunctionLoop(void);

DWORD WINAPI ScriptThread(LPVOID) {
  { // Install Hook
    size_t hGameModule = 0;
    while (!hGameModule) {
      hGameModule = (size_t)GetModuleHandle(L"Game.dll");
      Sleep(50);
    }
    { // Unprotect the game.dll module
      IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)(hGameModule + ((IMAGE_DOS_HEADER*)hGameModule)->e_lfanew);
      SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
      DWORD oldProtect;
      VirtualProtect((VOID*)hGameModule, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    }

    /*
    * game.dll+4dd90:
    * mov dword ptr ds:[game.dll+85FF20], ecx
    * game.dll+4dd96
    * ret
    */
    MemPtr codePtr = hGameModule + 0x4dd96;
    //constexpr int codeLen = 7;
    //BYTE codes[codeLen];
    MemCall(codePtr.address, HookFunctionLoop);
    constexpr int callInsLen = 5;
    MemWrite(codePtr.address + callInsLen, X86_OP_RET);
  }
  return S_OK;
}

BOOL APIENTRY DllMain(
  HMODULE hModule,
  DWORD  ul_reason_for_call,
  LPVOID lpReserved
)
{
  if (ul_reason_for_call == DLL_PROCESS_ATTACH)
  {
    // Unprotect the module NOW
    auto hExecutableInstance = (size_t)GetModuleHandle(NULL);
    IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)(hExecutableInstance + ((IMAGE_DOS_HEADER*)hExecutableInstance)->e_lfanew);
    SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
    DWORD oldProtect;
    VirtualProtect((VOID*)hExecutableInstance, size, PAGE_EXECUTE_READWRITE, &oldProtect);

    // Create Script Thread
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&ScriptThread, NULL, 0, NULL);
  }
  return TRUE;
}

