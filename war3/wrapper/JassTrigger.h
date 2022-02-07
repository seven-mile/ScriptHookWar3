#pragma once
#include "JassHandle.h"
#include "JassPlayer.h"
#include "JassUnit.h"
#include "JassHero.h"
#include "../enum.h"

struct JassTrigger : public JassHandle
{
  JassTrigger(HObject handle) : JassHandle(handle) { }

  static JassTrigger Create();
  static JassTrigger CreateForGameEvent(EVENT_GAME event);
  static JassTrigger CreateForPlayerEvent(JassPlayer player, EVENT_PLAYER event);
  static JassTrigger CreateForPlayerUnitsEvent(JassPlayer player, EVENT_PLAYER_UNIT event);
  static JassTrigger CreateForPlayerHeroesEvent(JassPlayer player, EVENT_PLAYER_HERO event);
  static JassTrigger CreateForUnitEvent(JassUnit unit, EVENT_UNIT event);
  static JassTrigger CreateForHeroEvent(JassHero hero, EVENT_HERO event);
  static JassTrigger CreateForWidget(JassWidget widget, EVENT_WIDGET event);
  static JassTrigger CreateForDialog(HDialog dialog, EVENT_DIALOG event);
  static JassTrigger CreateForDialogButton(HDialogButton dialogBtn, EVENT_DIALOG event);

  const JassTrigger& Reset() const;
  const JassTrigger& Sleep(float ms) const;
  const JassTrigger& AddAction(std::function<void()> const &action) const;
  const JassTrigger& ClearActions() const;
  const JassTrigger& ExecuteActions() const;
  void Destroy() const;


  bool GetIsEnabled() const;
  const JassTrigger& SetIsEnabled(bool value) const;
};

