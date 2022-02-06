#include "pch.h"
#include "war3/jass_string.h"
#include "war3/hash_table.h"
#include "war3/native_table.h"
#include "war3/func_callback.h"
#include "war3/func_call.h"
#include "war3/jass_helper.h"

#include "war3/wrapper/JassPlayer.h"
#include "war3/wrapper/JassHero.h"
#include "war3/wrapper/ScriptMenu.h"

void ScriptLoop(void) {
  auto tickCount = GetTickCount64();

  // Activate Cheat Menu
  auto keyState = GetAsyncKeyState(VK_F5);
  static decltype(tickCount) lastTickCount = 0;
  const decltype(tickCount) noRespTimeInMs = 100;
  if ((keyState & (1u << 31)) && lastTickCount + noRespTimeInMs < tickCount /* todo: && game is active */) {
    lastTickCount = tickCount;
    
    // c++ api call
    //{
    //  auto ply = JassPlayer::LocalPlayer();
    //  if (JassHero hero = ply.GetSelectedUnits().FirstUnit()) {
    //    hero.SetMoveSpeed(2 * hero.GetMoveSpeed());
    //  } else {
    //    DebugOutput("oops, it is NOT a HERO.");
    //  }
    //}
    
    // Try add action
    //auto trg = CallFn<HTrigger>("CreateTrigger");
    //auto act1 = CallFn<HTriggerAction>("TriggerAddAction", trg, []() {
    //  DebugOutput("ScriptHookWar3: Action triggered!");
    //});
    //CallFn<HEvent>("TriggerRegisterPlayerChatEvent", trg, ply, "war", true);

    static ScriptMenu mainMenu, resMenu, unitMenu;
    static bool init = false;
    if (!init) {
      init = true;

      mainMenu.AddSubMenuButton("资源", resMenu);
      mainMenu.AddSubMenuButton("单位特殊性", unitMenu);
      mainMenu.AddActionButton("退出", [](){});

      resMenu.AddActionButton("加9999钱", []() {
        auto ply = JassPlayer::LocalPlayer();
        ply.SetState(PLAYER_STATE::RESOURCE_GOLD,
          ply.GetState(PLAYER_STATE::RESOURCE_GOLD) + 9999);
      });
      resMenu.AddActionButton("加9999木", []() {
        auto ply = JassPlayer::LocalPlayer();
        ply.SetState(PLAYER_STATE::RESOURCE_LUMBER,
          ply.GetState(PLAYER_STATE::RESOURCE_LUMBER) + 9999);
      });
      resMenu.AddSubMenuButton("返回", mainMenu);
      resMenu.AddActionButton("退出", [](){});

      unitMenu.AddSwitchButton("缩小为正常", "变大为2倍",
      []() {
        auto unit = JassPlayer::LocalPlayer().GetSelectedUnits().FirstUnit();
        unit.SetScale(1.0);
      },
      []() {
        auto unit = JassPlayer::LocalPlayer().GetSelectedUnits().FirstUnit();
        unit.SetScale(2.0);
      });
      unitMenu.AddSubMenuButton("返回", mainMenu);
      unitMenu.AddActionButton("退出", [](){});
    }

    mainMenu.Display();
  }
}
