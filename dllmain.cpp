#include "pch.h"

template<typename U>
inline void CollectArg(LPVOID* stackBin, std::vector<std::function<void()>>& deleterBin, const U& arg) {
  if constexpr (simple_string<U>) {
    MemPtr obj = CreateJassString(arg);
    *stackBin = obj.pointer;
    deleterBin.push_back([str = obj.handle]() {
      DestroyJassString(str);
    });
  } else if constexpr (std::is_floating_point_v<U>) {
    auto parg = new float { static_cast<float>(arg) };
    *stackBin = parg;
    deleterBin.push_back([parg]() { delete parg; });
  } else if constexpr (std::is_convertible_v<U, INT32>) {
    *stackBin = reinterpret_cast<LPVOID>(static_cast<INT32>(arg));
  } else if constexpr (std::is_convertible_v<U, UINT32>) {
    *stackBin = reinterpret_cast<LPVOID>(static_cast<UINT32>(arg));
  } else if constexpr (std::is_same_v<U, JassObject>) {
    *stackBin = *reinterpret_cast<const LPVOID*>(&arg);
  } else {
    []() {
      static_assert(false, "Invalid arg type provided for CallFn! Check type U:");
    }();
  }
}

template<typename U>
inline void CollectArgs(LPVOID* stackBin, std::vector<std::function<void()>>& deleterBin, const U& carg) {
  CollectArg(stackBin, deleterBin, carg);
}

template<typename U, typename... T>
inline void CollectArgs(LPVOID* stackBin, std::vector<std::function<void()>>& deleterBin, const U& carg, T... rargs) {
  CollectArgs(stackBin + 1, deleterBin, rargs...);
  CollectArg(stackBin, deleterBin, carg);
}

template<typename R, size_t W>
struct FnCallType { };

template<typename R>
struct FnCallType<R, 0> {
  using type = R();
};

template<typename R>
struct FnCallType<R, 1> {
  using type = R(LPVOID);
};

template<typename R>
struct FnCallType<R, 2> {
  using type = R(LPVOID, LPVOID);
};

template<typename R>
struct FnCallType<R, 3> {
  using type = R(LPVOID, LPVOID, LPVOID);
};

template<typename R>
struct FnCallType<R, 4> {
  using type = R(LPVOID, LPVOID, LPVOID, LPVOID);
};

template<typename R>
struct FnCallType<R, 5> {
  using type = R(LPVOID, LPVOID, LPVOID, LPVOID, LPVOID);
};

template<typename R>
R CallFn(const std::string& func_name) {
  // call
  auto itRes = mapFunc.find(func_name);
  LPVOID fnPtr = nullptr;
  if (itRes != mapFunc.end())
    fnPtr = itRes->second->fnAddr;
  else {
    // wait 50ms and try once again
    Sleep(50);
    RegenerateFunctionMap();
    itRes = mapFunc.find(func_name);
    if (itRes != mapFunc.end())
      fnPtr = itRes->second->fnAddr;
    else {
      assert(false && "Cannot find specified jass function!");
    }
  }

  MemPtr retVal = nullptr;
  __asm {
    call fnPtr
    mov retVal, eax
  }

  if constexpr (!std::is_void_v<R>) {
    if constexpr (simple_string<R>) {
      return UnpackJassString(retVal.handle);
    } else if constexpr (std::is_floating_point_v<R>) {
      return static_cast<R>(MemRead<float>(retVal));
    } else if constexpr (std::is_convertible_v<R, int>) {
      return retVal.address;
    } else if constexpr (std::is_same_v<R, JassObject>) {
      return retVal.handle;
    } else {
      []() {
        static_assert(false, "Invalid return type provided for CallFn! Check type R:");
      }();
    }
  }
}

template<typename R, typename... T>
R CallFn(const std::string& func_name, T... args) {
  constexpr int argSize = sizeof...(T);
  LPVOID stackBin[argSize];
  std::vector<std::function<void()>> deleterBin;

  CollectArgs(stackBin, deleterBin, args...);

  // call
  auto itRes = mapFunc.find(func_name);
  LPVOID fnPtr = nullptr;
  if (itRes != mapFunc.end())
    fnPtr = itRes->second->fnAddr;
  else {
    // wait 50ms and try once again
    Sleep(50);
    RegenerateFunctionMap();
    itRes = mapFunc.find(func_name);
    if (itRes != mapFunc.end())
      fnPtr = itRes->second->fnAddr;
    else {
      assert(false && "Cannot find specified jass function!");
    }
  }

  MemPtr argsPtr = nullptr;
  MemPtr retVal = nullptr;
  constexpr int allocaSize = sizeof stackBin;
  __asm {
    sub esp, allocaSize
    mov argsPtr, esp
  }
  memcpy(argsPtr, stackBin, sizeof stackBin);
  __asm {
    call fnPtr
    mov retVal, eax
    add esp, allocaSize
  }

  // delete
  for (auto&& del : deleterBin) {
    del();
  }

  if constexpr (!std::is_void_v<R>) {
    if constexpr (simple_string<R>) {
      return UnpackJassString(retVal.handle);
    } else if constexpr (std::is_floating_point_v<R>) {
      return static_cast<R>(MemRead<float>(retVal));
    } else if constexpr (std::is_convertible_v<R, int>) {
      return retVal.address;
    } else if constexpr (std::is_same_v<R, JassObject>) {
      return retVal.handle;
    } else {
      []() {
        static_assert(false, "Invalid return type provided for CallFn! Check type R:");
      }();
    }
  }
}

void WINAPI HookFunctionLoop(void) {
  // Activate Cheat Menu
  auto keyState = GetAsyncKeyState(VK_F5);
  if ((keyState & (1u << 31)) /* todo: && game is active */) {

    // GetInstance is useful for accessing internal resources
    // war3.exe#497 for 1.20e
    size_t (_fastcall * GetInstance)(int) = reinterpret_cast<decltype(GetInstance)>(GetProcAddress(GetModuleHandle(NULL), MAKEINTRESOURCEA(497)));
    MemPtr pvm = nullptr, pCodeRel = nullptr;
    if (GetInstance) {
      MemPtr ptr5 = GetInstance(5);
      pvm = MemRead(MemRead(ptr5.address + 0x90) + 0x4 * 1); // 1 is the first vm index
      // Get code table relative pointer
      // *(_DWORD*)(this[2593] + 8) + 4 * v25
      pCodeRel = MemRead(MemRead(pvm.address + 10372) + 8);
      // the native functions access *(struct Code)(pCodeRel + codeArg)

      // output it
      auto resStr = std::format("pVM = 0x{:x}, pCodeRel = 0x{:x}\n", pvm.address, pCodeRel.address);;
      CallFn<void>(
        "DisplayTimedTextToPlayer",
        CallFn<JassPlayer>("Player", 0),
        0.0f,
        0.0f,
        120.0f,
        resStr.c_str()
      );
      OutputDebugStringA(resStr.c_str());
    }


    const int PLAYERID_LOCAL = 0;
    auto ply = CallFn<JassPlayer>("Player", PLAYERID_LOCAL);
    auto grp = CallFn<JassGroup>("CreateGroup");
    CallFn<void>("GroupEnumUnitsSelected", grp, ply, false);
    auto unit = CallFn<JassUnit>("FirstOfGroup", grp);
    const int UNIT_TYPE_HERO = 0;
    if (CallFn<bool>("IsUnitType", unit, UNIT_TYPE_HERO)) {
      CallFn<void>("AddHeroXP", unit, 1000, true);

      CallFn<void>(
        "DisplayTimedTextToPlayer",
        CallFn<JassPlayer>("Player", 0),
        0.0f,
        0.0f,
        10.0f,
        "ScriptHookWar3: Added!"
      );
    } else {
      CallFn<void>(
        "DisplayTimedTextToPlayer",
        CallFn<JassPlayer>("Player", 0),
        0.0f,
        0.0f,
        10.0f,
        "ScriptHookWar3: The selected unit is not hero."
      );
    }
    Sleep(200);
  }
}

DWORD WINAPI ScriptThread(LPVOID) {
  { // Install Hook
    size_t hGameModule = 0;
    while (!hGameModule) {
      hGameModule = (size_t)GetModuleHandle(L"Game.dll");
      Sleep(50);
    }
    { // Unprotect the game module
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
    MemWrite(codePtr.address + callInsLen, OP_RET);
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

