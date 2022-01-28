#include "pch.h"
#include "war3/jass_string.h"
#include "war3/hash_table.h"
#include "war3/native_table.h"
#include "war3/func_callback.h"
#include "war3/func_call.h"
#include "war3/jass_helper.h"

void ScriptLoop(void) {
  auto tickCount = GetTickCount64();

  // Activate Cheat Menu
  auto keyState = GetAsyncKeyState(VK_F5);
  static decltype(tickCount) lastTickCount = 0;
  const decltype(tickCount) noRespTimeInMs = 100;
  if ((keyState & (1u << 31)) && lastTickCount + noRespTimeInMs < tickCount /* todo: && game is active */) {
    lastTickCount = tickCount;
    const int PLAYERID_LOCAL = 0;
    auto ply = CallFn<HPlayer>("Player", PLAYERID_LOCAL);
    auto grp = CallFn<HGroup>("CreateGroup");
    CallFn<void>("GroupEnumUnitsSelected", grp, ply, false);
    auto unit = CallFn<HUnit>("FirstOfGroup", grp);
    CallFn<void>("ForGroup", grp, []() {
      const int UNIT_TYPE_HERO = 0;
      auto unit = CallFn<HUnit>("GetEnumUnit");
      assert(unit && "Got invalid enum unit!");
      if (CallFn<bool>("IsUnitType", unit, UNIT_TYPE_HERO)) {
        CallFn<void>("AddHeroXP", unit, 10000, true);
        DebugOutput("ScriptHookWar3: Added!");
      } else {
        DebugOutput("ScriptHookWar3: The selected unit is not a hero.");
      }
    });


    // Try add action
    //auto trg = CallFn<HTrigger>("CreateTrigger");
    //auto act1 = CallFn<HTriggerAction>("TriggerAddAction", trg, []() {
    //  DebugOutput("ScriptHookWar3: Action triggered!");
    //});
    //CallFn<HEvent>("TriggerRegisterPlayerChatEvent", trg, ply, "war", true);

    // Try show in-game dialog box!
    static HDialog dlg = 0;
    if (!dlg) {
      dlg = CallFn<HDialog>("DialogCreate");
      CallFn<void>("DialogSetMessage", dlg, "ScriptHookWar3\nNative Trainer ver.β\nMain Menu");
      // Direct Unicode Text Support (Save Source using UTF8)
      auto btnRes = CallFn<HDialogButton>("DialogAddButton", dlg, "资源", 0);
      auto btnResTrg = CallFn<HTrigger>("CreateTrigger");
      auto btnResTrgAct = CallFn<HTriggerAction>("TriggerAddAction", btnResTrg, [ply]() {
        CallFn<void>("DialogDisplay", ply, dlg, false);
      });
      auto btn2 = CallFn<HDialogButton>("DialogAddButton", dlg, "自动化", 0);
      auto btn3 = CallFn<HDialogButton>("DialogAddButton", dlg, "选定单位特殊性", 0);
      auto btnClose = CallFn<HDialogButton>("DialogAddButton", dlg, "退出", 0);
      CallFn<HEvent>("TriggerRegisterDialogButtonEvent", btnResTrg, btnClose);
    }
    CallFn<void>("DialogDisplay", ply, dlg, true);
  }
}
