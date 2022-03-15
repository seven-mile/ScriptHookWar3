#include "../../pch.h"
#include "JassPlayer.h"
#include "../func_call.h"

JassPlayer JassPlayer::EnumPlayer()
{
  return CallFn<HPlayer>("GetEnumPlayer");
}

JassPlayer JassPlayer::FilterPlayer()
{
  return CallFn<HPlayer>("GetFilterPlayer");
}

JassPlayer JassPlayer::TriggerPlayer()
{
  return CallFn<HPlayer>("GetTriggerPlayer");
}

JassPlayer JassPlayer::LocalPlayer()
{
  return CallFn<HPlayer>("GetLocalPlayer");
}

JassPlayer JassPlayer::NeutralPassive()
{
  return CallFn<HPlayer>("Player", PLAYER_ID::NEUTRAL_PASSIVE);
}

JassPlayer JassPlayer::NeutralAggressive()
{
  return CallFn<HPlayer>("Player", PLAYER_ID::NEUTRAL_AGGRESSIVE);
}

const JassPlayer& JassPlayer::SetTechResearchLevel(int techCode, int level) const
{
  CallFn<void>("SetPlayerTechResearched", handle, techCode, level);
  return *this;
}

bool JassPlayer::GetTechResearchState(int techCode) const
{
  return CallFn<bool>("GetPlayerTechResearched", handle, techCode);
}

std::string JassPlayer::GetName() const
{
  return CallFn<const char*>("GetPlayerName", handle);
}

const JassPlayer& JassPlayer::SetName(std::string const& name) const
{
  CallFn<void>("SetPlayerName", handle, name.c_str());
  return *this;
}

PLAYER_COLOR JassPlayer::GetColor() const
{
  return CallFn<PLAYER_COLOR>("SetPlayerRace", handle);
}

const JassPlayer& JassPlayer::SetColor(PLAYER_COLOR const& color) const
{
  CallFn<void>("SetPlayerColor", handle, color);
  return *this;
}

int JassPlayer::GetState(PLAYER_STATE const& state) const
{
  return CallFn<int>("GetPlayerState", handle, state);
}

const JassPlayer& JassPlayer::SetState(PLAYER_STATE const& state, int value) const
{
  CallFn<void>("SetPlayerState", handle, state, value);
  return *this;
}

float JassPlayer::GetHandicapHP() const
{
  return CallFn<float>("GetPlayerHandicap", handle);
}

const JassPlayer& JassPlayer::SetHandicapHP(float handicap_hp) const
{
  CallFn<void>("SetPlayerHandicap", handle, handicap_hp);
  return *this;
}

float JassPlayer::GetHandicapXP() const
{
  return CallFn<float>("GetPlayerHandicapXP", handle);
}

const JassPlayer& JassPlayer::SetHandicapXP(float handicap_xp) const
{
  CallFn<void>("SetPlayerHandicapXP", handle, handicap_xp);
  return *this;
}

RACE JassPlayer::GetRace() const
{
  return CallFn<RACE>("GetPlayerRace", handle);
}

int JassPlayer::GetTeam() const
{
  return CallFn<int>("GetPlayerTeam", handle);
}

JassUnitGroup JassPlayer::GetAllUnits() const
{
  auto grp = CallFn<HGroup>("CreateGroup");
  CallFn<void>("GroupEnumUnitsOfPlayer", grp, handle, 0);
  return grp;
}

JassUnitGroup JassPlayer::GetSelectedUnits() const
{
  auto grp = CallFn<HGroup>("CreateGroup");
  CallFn<void>("GroupEnumUnitsSelected", grp, handle, 0);
  return grp;
}

const JassPlayer& JassPlayer::DisplayText(std::string const& text, float duration, float x, float y) const
{
  if (fabs(duration) < 1e-7) {
    CallFn<void>("DisplayTextToPlayer", handle, x, y, text.c_str());
  } else {
    CallFn<void>("DisplayTimedTextToPlayer", handle, x, y, duration, text.c_str());
  }
  return *this;
}
