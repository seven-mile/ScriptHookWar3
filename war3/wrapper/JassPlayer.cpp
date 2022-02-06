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
  return CallFn<HPlayer>("Player", PLAYER_ID::LOCAL);
}

JassPlayer JassPlayer::NeutralPassive()
{
  return CallFn<HPlayer>("Player", PLAYER_ID::NEUTRAL_PASSIVE);
}

JassPlayer JassPlayer::NeutralAggressive()
{
  return CallFn<HPlayer>("Player", PLAYER_ID::NEUTRAL_AGGRESSIVE);
}

void JassPlayer::SetTechResearchLevel(int techCode, int level)
{
  CallFn<void>("SetPlayerTechResearched", handle, techCode, level);
}

bool JassPlayer::GetTechResearchState(int techCode)
{
  return CallFn<bool>("GetPlayerTechResearched", handle, techCode);
}

std::string JassPlayer::GetName()
{
  return CallFn<const char*>("GetPlayerName", handle);
}

void JassPlayer::SetName(std::string const& name)
{
  CallFn<void>("SetPlayerName", handle, name.c_str());
}

PLAYER_COLOR JassPlayer::GetColor()
{
  return CallFn<PLAYER_COLOR>("SetPlayerRace", handle);
}

void JassPlayer::SetColor(PLAYER_COLOR const& color)
{
  CallFn<void>("SetPlayerColor", handle, color);
}

int JassPlayer::GetState(PLAYER_STATE const& state)
{
  return CallFn<int>("GetPlayerState", handle, state);
}

void JassPlayer::SetState(PLAYER_STATE const& state, int value)
{
  CallFn<void>("SetPlayerState", handle, state, value);
}

float JassPlayer::GetHandicapHP()
{
  return CallFn<float>("GetPlayerHandicap", handle);
}

void JassPlayer::SetHandicapHP(float handicap_hp)
{
  CallFn<void>("SetPlayerHandicap", handle, handicap_hp);
}

float JassPlayer::GetHandicapXP()
{
  return CallFn<float>("GetPlayerHandicapXP", handle);
}

void JassPlayer::SetHandicapXP(float handicap_xp)
{
  CallFn<void>("SetPlayerHandicapXP", handle, handicap_xp);
}

RACE JassPlayer::GetRace()
{
  return CallFn<RACE>("GetPlayerRace", handle);
}

int JassPlayer::GetTeam()
{
  return CallFn<int>("GetPlayerTeam", handle);
}

JassUnitGroup JassPlayer::GetAllUnits()
{
  auto grp = CallFn<HGroup>("CreateGroup");
  CallFn<void>("GroupEnumUnitsOfPlayer", grp, handle, 0);
  return grp;
}

JassUnitGroup JassPlayer::GetSelectedUnits()
{
  auto grp = CallFn<HGroup>("CreateGroup");
  CallFn<void>("GroupEnumUnitsSelected", grp, handle, 0);
  return grp;
}

void JassPlayer::DisplayText(std::string const& text, float duration, float x, float y)
{
  if (fabs(duration) < 1e-7) {
    CallFn<void>("DisplayTextToPlayer", handle, x, y, text.c_str());
  } else {
    CallFn<void>("DisplayTimedTextToPlayer", handle, x, y, duration, text.c_str());
  }
}
