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
    auto ply = CallFn<JassPlayer>("Player", PLAYERID_LOCAL);
    auto grp = CallFn<JassGroup>("CreateGroup");
    CallFn<void>("GroupEnumUnitsSelected", grp, ply, false);
    auto unit = CallFn<JassUnit>("FirstOfGroup", grp);

    CallFn<void>(
      "DisplayTimedTextToPlayer",
      CallFn<JassPlayer>("Player", 0),
      0.0f,
      0.0f,
      120.0f,
      "This is a test string from ScriptHookWar3"
    );

    CallFn<void>("ForGroup", grp, []() {
      const int UNIT_TYPE_HERO = 0;
      auto unit = CallFn<JassUnit>("GetEnumUnit");
      assert(unit && "Got invalid enum unit!");
      if (CallFn<bool>("IsUnitType", unit, UNIT_TYPE_HERO)) {
        CallFn<void>("AddHeroXP", unit, 10000, true);
        DebugOutput("ScriptHookWar3: Added!");
      } else {
        DebugOutput("ScriptHookWar3: The selected unit is not a hero.");
      }
    });


    // Try add action
    //auto trg = CallFn<JassTrigger>("CreateTrigger");
    //auto act1 = CallFn<JassTriggerAction>("TriggerAddAction", trg, []() {
    //  DebugOutput("ScriptHookWar3: Action triggered!");
    //});
    //CallFn<JassEvent>("TriggerRegisterPlayerChatEvent", trg, ply, "war", true);

    // Try show in-game dialog box!
    static JassDialog dlg = 0;
    if (!dlg) {
      dlg = CallFn<JassDialog>("DialogCreate");
      CallFn<void>("DialogSetMessage", dlg, "ScriptHookWar3\nNative Trainer ver.β\nMain Menu");
      // Direct Unicode Text Support (Save Source using UTF8)
      auto btn1 = CallFn<JassDialogButton>("DialogAddButton", dlg, "资源", 0);
      auto btn2 = CallFn<JassDialogButton>("DialogAddButton", dlg, "自动化", 0);
      auto btn3 = CallFn<JassDialogButton>("DialogAddButton", dlg, "选定单位特殊性", 0);
      auto btnClose = CallFn<JassDialogButton>("DialogAddButton", dlg, "退出", 0);
      auto btnCloseTrg = CallFn<JassTrigger>("CreateTrigger");
      auto btnCloseTrgAct = CallFn<JassTriggerAction>("TriggerAddAction", btnCloseTrg, [ply]() {
        CallFn<void>("DialogDisplay", ply, dlg, false);
      });
      CallFn<JassEvent>("TriggerRegisterDialogButtonEvent", btnCloseTrg, btnClose);
    }
    CallFn<void>("DialogDisplay", ply, dlg, true);
  }
}
