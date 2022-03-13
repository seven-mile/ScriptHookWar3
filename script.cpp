#include "pch.h"
#include "war3/jass_string.h"
#include "war3/hash_table.h"
#include "war3/native_table.h"
#include "war3/func_callback.h"
#include "war3/func_call.h"
#include "war3/jass_helper.h"
#include "war3/game_end_event.h"

#include "war3/wrapper/JassPlayer.h"
#include "war3/wrapper/JassHero.h"
#include "war3/wrapper/JassTrigger.h"
#include "war3/wrapper/ScriptMenu.h"
#include "war3/wrapper/JassContext.h"


#include "keyboard_manager.h"

void ScriptInit(void) {
  // F5 -> display menu
  static ScriptMenu* pMainMenu{ nullptr }, * pResMenu{ nullptr }, * pUnitMenu{ nullptr }, * pUnit2Menu{ nullptr };
  static bool init = false;

  // discard return value
  RegisterGameEndEvent([]() {
    init = false;
    if (pMainMenu) pMainMenu->Clear();
    if (pResMenu) pResMenu->Clear();
    if (pUnitMenu) pUnitMenu->Clear();
    if (pUnit2Menu) pUnit2Menu->Clear();
    delete pMainMenu;
    delete pResMenu;
    delete pUnitMenu;
    delete pUnit2Menu;
    pMainMenu = nullptr;
    pResMenu = nullptr;
    pUnitMenu = nullptr;
    pUnit2Menu = nullptr;
  });

  KeyboardManager::Register([]()
  {
    if (!init) {
      init = true;
      pMainMenu = new ScriptMenu{};
      pResMenu = new ScriptMenu{};
      pUnitMenu = new ScriptMenu{};
      pUnit2Menu = new ScriptMenu{};
      ScriptMenu &mainMenu = *pMainMenu,
                 &resMenu = *pResMenu,
                 &unitMenu = *pUnitMenu,
                 &unit2Menu = *pUnit2Menu;

      mainMenu.AddSubMenuButton("资源", resMenu);
      mainMenu.AddSubMenuButton("单位特殊性", unitMenu);
      mainMenu.AddActionButton("退出", []() {});

      { // Map Visiblity
        auto fogMod = CallFn<HFogModifier>(
          "CreateFogModifierRect",
          JassPlayer::LocalPlayer().handle,
          FOG_STATE::VISIBLE,
          JCtx::GetWorldBounds(),
          true,
          false
          );

        resMenu.AddSwitchButton("关闭地图", "打开地图", [=]() {
          CallFn<void>("FogModifierStop", fogMod);
          resMenu.Display();
        }, [=]() {
          CallFn<void>("FogModifierStart", fogMod);
          resMenu.Display();
        });
      }
      resMenu.AddActionButton("加100000钱", [=]() {
        auto ply = JassPlayer::LocalPlayer();
        ply.SetState(PLAYER_STATE::RESOURCE_GOLD,
          ply.GetState(PLAYER_STATE::RESOURCE_GOLD) + 100000);
        resMenu.Display();
      });
      resMenu.AddActionButton("加100000木", [=]() {
        auto ply = JassPlayer::LocalPlayer();
        ply.SetState(PLAYER_STATE::RESOURCE_LUMBER,
          ply.GetState(PLAYER_STATE::RESOURCE_LUMBER) + 100000);
        resMenu.Display();
      });

      { // fast train
        auto trgTrain = JassTrigger::CreateForPlayerUnitsEvent(JassPlayer::LocalPlayer(), EVENT_PLAYER_UNIT::TRAIN_START);
        trgTrain.SetIsEnabled(false).AddAction([]() {
          auto trainer = JassUnit::TriggerUnit();
          JassUnit::Create(
            JassPlayer::LocalPlayer(),
            JCtx::GetTrainedUnitType(),
            trainer.GetRallyPoint(),
            0.0
          );
          // cancel current training
          trainer.IssueOrder(ORDER_ID::cancel);
          JassPlayer::LocalPlayer()
            .SetState(PLAYER_STATE::RESOURCE_FOOD_USED, 0)
            .SetState(PLAYER_STATE::RESOURCE_FOOD_CAP, 100);
        });
        resMenu.AddSwitchButton("取消快速训练单位", "快速训练单位",
          [=]() { trgTrain.SetIsEnabled(false); resMenu.Display(); },
          [=]() { trgTrain.SetIsEnabled(true);  resMenu.Display(); });
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
      resMenu.AddActionButton("退出", []() {});

      { // auto full unit state
        auto trgLife = JassTrigger::CreateForPlayerUnitsEvent(
          JassPlayer::LocalPlayer(), EVENT_PLAYER_UNIT::ATTACKED);
        trgLife.SetIsEnabled(false).AddAction([]() {
          auto unit = JassUnit::TriggerUnit();
          unit.SetState(UNIT_STATE::LIFE, unit.GetState(UNIT_STATE::MAX_LIFE));
        });
        auto trgMana = JassTrigger::CreateForPlayerUnitsEvent(
          JassPlayer::LocalPlayer(), EVENT_PLAYER_UNIT::SPELL_ENDCAST);
        trgMana.SetIsEnabled(false).AddAction([]() {
          auto unit = JassUnit::TriggerUnit();
          unit.SetState(UNIT_STATE::MANA, unit.GetState(UNIT_STATE::MAX_MANA))
            .ResetCooldown();
        });
        unitMenu.AddSwitchButton("取消全部己方单位无敌", "全部己方单位无敌",
          [=]() {
          trgLife.SetIsEnabled(false);
          trgMana.SetIsEnabled(false);
          unitMenu.Display();
        },
          [=]() {
          trgLife.SetIsEnabled(true);
          trgMana.SetIsEnabled(true);
          unitMenu.Display();
        });
      }

      { // patrol to teleport
        auto trg = JassTrigger::CreateForPlayerUnitsEvent(JassPlayer::LocalPlayer(), EVENT_PLAYER_UNIT::ISSUED_POINT_ORDER);
        trg.SetIsEnabled(false).AddAction([]() {
          if (JCtx::GetIssuedOrderId() == ORDER_ID::patrol) {
            auto unit = JCtx::GetOrderedUnit();
            auto point = JCtx::GetOrderPoint();
            unit.SetPosition(point.x, point.y);
          }
        });

        unitMenu.AddSwitchButton("关闭P闪", "开启P闪",
          [=]() { trg.SetIsEnabled(false); unitMenu.Display(); },
          [=]() { trg.SetIsEnabled(true);  unitMenu.Display(); });
      }

      unitMenu.AddSubMenuButton("下一页", unit2Menu);

      unitMenu.AddSubMenuButton("返回主菜单", mainMenu);
      unitMenu.AddActionButton("退出", []() {});

      unit2Menu.AddActionButton("控制所选单位", [=]() {
        auto ply = JassPlayer::LocalPlayer();
        ply.GetSelectedUnits()
          .ForEach([=](JassUnit unit) { unit.SetOwner(ply); unit2Menu.Display(); });
      });

      unit2Menu.AddActionButton("复制所选单位", [=]() {
        auto ply = JassPlayer::LocalPlayer();
        ply.GetSelectedUnits().ForEach([=](JassUnit unit) {
          auto new_unit = JassUnit::Create(
            ply, unit.GetTypeId(), unit.GetX(), unit.GetY(), unit.GetFacing());
          if (new_unit.IsType(UNIT_TYPE::HERO)) {
            auto orig_hero = JassHero(unit), new_hero = JassHero(new_unit);
            // silently copy all hero data
            new_hero.SetXP(orig_hero.GetXP(), false);
            new_hero.SetStrength(orig_hero.GetStrength());
            new_hero.SetAgility(orig_hero.GetAgility());
            new_hero.SetIntelligence(orig_hero.GetIntelligence());
          }
        });
        unit2Menu.Display();
      });

      unit2Menu.AddActionButton("增加所选英雄无敌三围", [=]() {
        JassPlayer::LocalPlayer().GetSelectedUnits().ForEach([](JassUnit unit) {
          if (JassHero hero = unit) {
            constexpr int value = 998244353;
            hero.SetStrength(value)
              .SetAgility(value)
              .SetIntelligence(value);
          }
        });
        unit2Menu.Display();
      });

      unit2Menu.AddSubMenuButton("上一页", unitMenu);

      unit2Menu.AddSubMenuButton("返回主菜单", mainMenu);
      unit2Menu.AddActionButton("退出", []() {});

    }

    pMainMenu->Display();
  }, VK_F5);
}

void ScriptLoop(void)
{
  KeyboardManager::Check();
}
