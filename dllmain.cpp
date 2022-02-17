#include "pch.h"
#include "war3/jass_string.h"

#pragma comment(lib, "version.lib")

// Define your custom script in script.cpp!
// This function will be called periodly, every ~10ms?
void ScriptLoop(void);

void WINAPI HookFunctionLoop(void) {
  static bool bExec = false;
  // GetInstance is useful for accessing internal resources
  // war3.exe#497 for 1.20e
  static size_t(__fastcall * GetInstance)(int) = reinterpret_cast<decltype(GetInstance)>(GetProcAddress(::GetModuleHandle(NULL), MAKEINTRESOURCEA(497)));

  { // Exec Once
    if (!bExec) {
      bExec = true;

      if (GetInstance) {
        MemPtr ptr5 = GetInstance(5);
        g_pVM = MemRead(MemRead(ptr5.address + 0x90) + 0x4 * 1); // 1 is the first vm index
        // Get symbol table pointer
        // 0x2854 for 1.20e, 0x2858 for higher
        const int offset = g_gameVersion > GameVersion::V120E ? 4 : 0;
        g_pSymTable = reinterpret_cast<decltype(g_pSymTable)>
          (MemRead(MemRead(g_pVM.address + 0x2854 + offset) + 0x8));

        // Get code table relative pointer
        // *(_DWORD*)(this[2593] + 8) + 4 * v25
        // 0x2884 for 1.20e, 0x2888 for higher
        g_pCodeRel = MemRead(MemRead(g_pVM.address + 0x2884 + offset) + 0x8);
        // the native functions access *(struct opcode**)(g_pCodeRel + codeArg)
      }
    }
  }

  ScriptLoop();
}

DWORD WINAPI ScriptThread(LPVOID) {
  { // Install Hook
    size_t hGameModule = 0;
    while (!hGameModule) {
      hGameModule = (size_t)::GetModuleHandle(L"Game.dll");
      Sleep(50);
    }
    { // Get Game Version
      
      WCHAR gamePath[1024];
      ::GetModuleFileName((HMODULE)hGameModule, gamePath, _countof(gamePath));
      DWORD dwHandle{};
      DWORD dwVerSize = ::GetFileVersionInfoSize(gamePath, &dwHandle);
      if (!dwVerSize) {
        ::MessageBox(nullptr,
          L"Failed to determine game.dll version,\n"
          L"ScriptHookWar3 is disabled.\n"
          L"We only support 1.20e and 1.24e\n"
          L"But you can continue the game without the plugin.",
          L"ScriptHookWar3", MB_ICONERROR);
        return HRESULT_FROM_WIN32(GetLastError());
      }
      auto pBuffer = new BYTE[dwVerSize];
      if (!::GetFileVersionInfo(gamePath, 0, dwVerSize, pBuffer)) {
        ::MessageBox(nullptr,
          L"Failed to determine game.dll version,\n"
          L"ScriptHookWar3 is disabled.\n"
          L"We only support 1.20e and 1.24e\n"
          L"But you can continue the game without the plugin.",
          L"ScriptHookWar3", MB_ICONERROR);
        return HRESULT_FROM_WIN32(GetLastError());
      }
      UINT nQuerySize;
      VS_FIXEDFILEINFO* pVsffi;
      if (!::VerQueryValue((void**)pBuffer, L"\\",
        (void**)&pVsffi, &nQuerySize))
      {
        ::MessageBox(nullptr,
          L"Failed to determine game.dll version,\n"
          L"ScriptHookWar3 is disabled.\n"
          L"We only support 1.20e and 1.24e\n"
          L"But you can continue the game without the plugin.",
          L"ScriptHookWar3", MB_ICONERROR);
        return HRESULT_FROM_WIN32(GetLastError());
      }
      
      DWORD A = HIWORD(pVsffi->dwProductVersionMS),
            B = LOWORD(pVsffi->dwProductVersionMS),
            C = HIWORD(pVsffi->dwProductVersionLS),
            D = LOWORD(pVsffi->dwProductVersionLS);

      /*
      * 1.20e = 1.20.4.6074
      * 1.24e = 1.24.4.6387
      */

      if (A == 1 && B == 20 && C == 4 && D == 6074) {
        g_gameVersion = GameVersion::V120E;
      } else if (A == 1 && B == 24 && C == 4 && D == 6387) {
        g_gameVersion = GameVersion::V124E;
      } else {
        g_gameVersion = GameVersion::UNKNOWN;
      }
    }
    { // Unprotect the game.dll module
      IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)(hGameModule + ((IMAGE_DOS_HEADER*)hGameModule)->e_lfanew);
      SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
      
      DWORD oldProtect;
      VirtualProtect((VOID*)hGameModule, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    }

    /* 1.20e as example
    * game.dll+4dd90:
    * mov dword ptr ds:[game.dll+85FF20], ecx
    * game.dll+4dd96
    * ret
    */

    MemPtr codePtr = nullptr;
    if (g_gameVersion == GameVersion::V120E) {
      codePtr = hGameModule + 0x4dd96;
    } else if (g_gameVersion == GameVersion::V124E) {
      codePtr = hGameModule + 0x4d3e36;
    } else {
      MessageBox(nullptr,
        L"Unsupported game version, ScriptHookWar3 is disabled.\n"
        L"We only support 1.20e and 1.24e\n"
        L"But you can continue the game without the plugin.",
        L"ScriptHookWar3", MB_ICONERROR);
      return HRESULT_FROM_WIN32(ERROR_UNSUPPORTED_TYPE);
    }

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
    {
      wchar_t fileName[MAX_PATH]{};
      ::GetModuleFileName(::GetModuleHandle(NULL), fileName, MAX_PATH);
      if (!std::wstring(fileName).ends_with(L"war3.exe")) {
        // not war3.exe, maybe worldedit, stop injecting
        return FALSE;
      }
    }

    // Unprotect the module NOW
    auto hExecutableInstance = (size_t)::GetModuleHandle(NULL);

    IMAGE_NT_HEADERS* ntHeader = (IMAGE_NT_HEADERS*)(hExecutableInstance + ((IMAGE_DOS_HEADER*)hExecutableInstance)->e_lfanew);
    SIZE_T size = ntHeader->OptionalHeader.SizeOfImage;
    DWORD oldProtect;
    ::VirtualProtect((VOID*)hExecutableInstance, size, PAGE_EXECUTE_READWRITE, &oldProtect);

    // Create Script Thread
    ::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&ScriptThread, NULL, 0, NULL);
  }
  return TRUE;
}

