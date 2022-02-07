#include "../../pch.h"
#include "JassTrigger.h"
#include "../func_call.h"

JassTrigger JassTrigger::Create()
{
  return CallFn<HTrigger>("CreateTrigger");
}

JassTrigger JassTrigger::CreateForGameEvent(EVENT_GAME event)
{
  auto trg = JassTrigger::Create();
  CallFn<HEvent>("TriggerRegisterGameEvent", trg.handle, event);
  return trg;
}

JassTrigger JassTrigger::CreateForPlayerEvent(JassPlayer player, EVENT_PLAYER event)
{
  auto trg = JassTrigger::Create();
  CallFn<HEvent>("TriggerRegisterPlayerEvent", trg.handle, player.handle, event);
  return trg;
}

JassTrigger JassTrigger::CreateForPlayerUnitsEvent(JassPlayer player, EVENT_PLAYER_UNIT event) {
  auto trg = JassTrigger::Create();
  CallFn<HEvent>("TriggerRegisterPlayerUnitEvent", trg.handle, player.handle, event, 0);
  return trg;
}

JassTrigger JassTrigger::CreateForPlayerHeroesEvent(JassPlayer player, EVENT_PLAYER_HERO event) {
  return JassTrigger::CreateForPlayerUnitsEvent(player, static_cast<EVENT_PLAYER_UNIT>(event));
}

JassTrigger JassTrigger::CreateForUnitEvent(JassUnit unit, EVENT_UNIT event) {
  auto trg = JassTrigger::Create();
  CallFn<HEvent>("TriggerRegisterUnitEvent", trg.handle, unit.handle, event);
  return trg;
}

JassTrigger JassTrigger::CreateForHeroEvent(JassHero hero, EVENT_HERO event)
{
  return JassTrigger::CreateForUnitEvent(hero, static_cast<EVENT_UNIT>(event));
}

JassTrigger JassTrigger::CreateForWidget(JassWidget widget, EVENT_WIDGET event) {
  auto trg = JassTrigger::Create();
  CallFn<HEvent>("TriggerRegisterWidgetEvent", trg.handle, widget.handle, event);
  return trg;
}

JassTrigger JassTrigger::CreateForDialog(HDialog dialog, EVENT_DIALOG event) {
  auto trg = JassTrigger::Create();
  CallFn<HEvent>("TriggerRegisterDialogEvent", trg.handle, dialog, event);
  return trg;
}

JassTrigger JassTrigger::CreateForDialogButton(HDialogButton dialogBtn, EVENT_DIALOG event) {
  auto trg = JassTrigger::Create();
  CallFn<HEvent>("TriggerRegisterDialogButtonEvent", trg.handle, dialogBtn, event);
  return trg;
}

const JassTrigger& JassTrigger::Reset() const
{
  CallFn<void>("ResetTrigger", handle);
  return *this;
}

const JassTrigger& JassTrigger::Sleep(float ms) const
{
  CallFn<void>("TriggerSleepAction", handle, ms);
  return *this;
}

const JassTrigger& JassTrigger::AddAction(std::function<void()> const& action) const
{
  CallFn<HTriggerAction>("TriggerAddAction", handle, action);
  return *this;
}

const JassTrigger& JassTrigger::ClearActions() const
{
  CallFn<void>("TriggerClearActions", handle);
  return *this;
}

const JassTrigger& JassTrigger::ExecuteActions() const
{
  CallFn<void>("TriggerExecute", handle);
  return *this;
}

void JassTrigger::Destroy() const
{
  CallFn<void>("DestroyTrigger", handle);
}

bool JassTrigger::GetIsEnabled() const
{
  return CallFn<bool>("IsTriggerEnabled", handle);
}

const JassTrigger& JassTrigger::SetIsEnabled(bool value) const
{
  CallFn<void>(value ? "EnableTrigger" : "DisableTrigger", handle);
  return *this;
}

