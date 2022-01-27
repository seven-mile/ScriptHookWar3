#include "pch.h"
#include "war3/jass_string.h"
#include "war3/hash_table.h"
#include "war3/native_table.h"
#include "war3/func_callback.h"
#include "war3/func_call.h"

void WINAPI HookFunctionLoop(void) {
  static bool bExec = false;
  // GetInstance is useful for accessing internal resources
  // war3.exe#497 for 1.20e
  static size_t(__fastcall * GetInstance)(int) = reinterpret_cast<decltype(GetInstance)>(GetProcAddress(GetModuleHandle(NULL), MAKEINTRESOURCEA(497)));

  // Activate Cheat Menu
  auto keyState = GetAsyncKeyState(VK_F5);
  if ((keyState & (1u << 31)) /* todo: && game is active */) {
    { // Exec Once
      if (!bExec) {
        bExec = true;

        if (GetInstance) {
          MemPtr ptr5 = GetInstance(5);
          g_pVM = MemRead(MemRead(ptr5.address + 0x90) + 0x4 * 1); // 1 is the first vm index
          // Get symbol table pointer
          // 0x2854 for 1.20e, 0x2858 for higher
          g_pSymTable = reinterpret_cast<decltype(g_pSymTable)>
            (MemRead(MemRead(g_pVM.address + 0x2854) + 0x8));

          // Get code table relative pointer
          // *(_DWORD*)(this[2593] + 8) + 4 * v25
          // 0x2884 for 1.20e, 0x2888 for higher
          g_pCodeRel = MemRead(MemRead(g_pVM.address + 0x2884) + 0x8);
          // the native functions access *(struct opcode**)(g_pCodeRel + codeArg)
        }
      }
    }
    // output it
    auto resStr = std::format("pVM = 0x{:x}, g_pCodeRel = 0x{:x}\n", g_pVM.address, g_pCodeRel.address);;
    CallFn<void>(
      "DisplayTimedTextToPlayer",
      CallFn<JassPlayer>("Player", 0),
      0.0f,
      0.0f,
      120.0f,
      resStr
      );
    OutputDebugStringA(resStr.c_str());


    // Script Starts
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
        "ScriptHookWar3: The selected unit is not a hero."
        );
    }

    // Try add action
    auto trg = CallFn<JassTrigger>("CreateTrigger");
    auto act1 = CallFn<JassTriggerAction>("TriggerAddAction", trg, CreateJassCallback([unit]() {
      CallFn<void>(
        "DisplayTimedTextToPlayer",
        CallFn<JassPlayer>("Player", 0),
        0.0f,
        0.0f,
        10.0f,
        "ScriptHookWar3: Action triggered!"
        );

      //auto res = CallFn<bool>("IsUnitType", unit, UNIT_TYPE_HERO);
      //OutputDebugString(std::format(L"res = {}", res).c_str());
    }));
    CallFn<JassEvent>("TriggerRegisterPlayerChatEvent", trg, ply, "war", true);


    Sleep(200);
  }
}
