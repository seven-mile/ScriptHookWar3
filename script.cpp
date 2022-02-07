#include "pch.h"
#include "war3/jass_string.h"
#include "war3/hash_table.h"
#include "war3/native_table.h"
#include "war3/func_callback.h"
#include "war3/func_call.h"
#include "war3/jass_helper.h"

#include "war3/wrapper/JassPlayer.h"
#include "war3/wrapper/JassHero.h"
#include "war3/wrapper/JassTrigger.h"
#include "war3/wrapper/ScriptMenu.h"

inline bool IsKeyDown(int code) {
  return GetAsyncKeyState(code) & (1u << 31);
}

void ScriptLoop(void) {
  auto tickCount = GetTickCount64();
  static decltype(tickCount) lastTickCount = 0;
  const decltype(tickCount) noRespTimeInMs = 100;

  // Activate Cheat Menu
  if (IsKeyDown(VK_F5) && lastTickCount + noRespTimeInMs < tickCount) {
    lastTickCount = tickCount;
    
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

      resMenu.AddActionButton("加100000钱", []() {
        auto ply = JassPlayer::LocalPlayer();
        ply.SetState(PLAYER_STATE::RESOURCE_GOLD,
          ply.GetState(PLAYER_STATE::RESOURCE_GOLD) + 100000);
        resMenu.Display();
      });
      resMenu.AddActionButton("加100000木", []() {
        auto ply = JassPlayer::LocalPlayer();
        ply.SetState(PLAYER_STATE::RESOURCE_LUMBER,
          ply.GetState(PLAYER_STATE::RESOURCE_LUMBER) + 100000);
        resMenu.Display();
      });

      { // fast train
        auto trgTrain = JassTrigger::CreateForPlayerUnitsEvent(JassPlayer::LocalPlayer(), EVENT_PLAYER_UNIT::TRAIN_START);
        trgTrain.SetIsEnabled(false).AddAction([]() {
          auto trainer = JassUnit::TriggerUnit();
          auto trainedUnitType = CallFn<int>("GetTrainedUnitType");
          auto rallyLoc = CallFn<HLocation>("GetUnitRallyPoint", trainer.handle);
          CallFn<HUnit>("CreateUnitAtLoc",
            JassPlayer::LocalPlayer().handle, trainedUnitType, rallyLoc, 0.0);
          // cancel current training
          trainer.IssueOrder(ORDER_ID::cancel);
          JassPlayer::LocalPlayer()
            .SetState(PLAYER_STATE::RESOURCE_FOOD_USED, 0)
            .SetState(PLAYER_STATE::RESOURCE_FOOD_CAP, 100);
        });
        resMenu.AddSwitchButton("取消快速训练单位", "快速训练单位",
          [=]() { trgTrain.SetIsEnabled(false); },
          [=]() { trgTrain.SetIsEnabled(true); });
      }

      { // left arrow key to fast construction / upgrade
        JassTrigger::CreateForPlayerEvent(JassPlayer::LocalPlayer(), EVENT_PLAYER::ARROW_LEFT_DOWN)
          .AddAction([]() {
          JassPlayer::LocalPlayer().GetSelectedUnits().FirstUnit()
            .SetConstructionProgress(100)
            .SetUpgradeProgress(100);
        });
      }

      resMenu.AddSubMenuButton("返回", mainMenu);
      resMenu.AddActionButton("退出", [](){});

      { // auto full unit state
        auto trgLife = JassTrigger::CreateForPlayerUnitsEvent(
          JassPlayer::LocalPlayer(), EVENT_PLAYER_UNIT::ATTACKED);
        trgLife.SetIsEnabled(false).AddAction([]() {
          auto unit = JassUnit::TriggerUnit();
          unit.SetState(UNIT_STATE::LIFE, unit.GetState(UNIT_STATE::MAX_LIFE));
        });
        auto trgMana = JassTrigger::CreateForPlayerUnitsEvent(
          JassPlayer::LocalPlayer(), EVENT_PLAYER_UNIT::SPELL_CAST);
        trgMana.SetIsEnabled(false).AddAction([]() {
          auto unit = JassUnit::TriggerUnit();
          unit.SetState(UNIT_STATE::MANA, unit.GetState(UNIT_STATE::MAX_MANA))
              .ResetCooldown();
        });
        unitMenu.AddSwitchButton("取消全部己方单位无敌", "全部己方单位无敌", 
          [=]() { trgLife.SetIsEnabled(false); trgMana.SetIsEnabled(false); },
          [=]() { trgLife.SetIsEnabled(true);  trgMana.SetIsEnabled(true); });
      }

      unitMenu.AddSubMenuButton("返回", mainMenu);
      unitMenu.AddActionButton("退出", [](){});
    }

    mainMenu.Display();
  }
}
