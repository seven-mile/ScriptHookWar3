#pragma once
#include "JassHandle.h"
#include "../enum.h"
#include "JassUnitGroup.h"

struct JassPlayer : public JassHandle
{
  JassPlayer(HPlayer handle) : JassHandle(handle) {  }

  // static function : special player value
  static JassPlayer EnumPlayer();
  static JassPlayer FilterPlayer();
  static JassPlayer TriggerPlayer();
  static JassPlayer LocalPlayer();

  static JassPlayer NeutralPassive();
  static JassPlayer NeutralAggressive();
  
  // property : getter/setter
  const JassPlayer& SetTechResearchLevel(int techCode, int level) const;
  bool GetTechResearchState(int techCode) const;

  std::string GetName() const;
  const JassPlayer& SetName(std::string const& name) const;

  PLAYER_COLOR GetColor() const;
  const JassPlayer& SetColor(PLAYER_COLOR const& color) const;

  int GetState(PLAYER_STATE const& state) const;
  const JassPlayer& SetState(PLAYER_STATE const& state, int value) const;

  // transform: normal_hp *= handicap_hp const;
  float GetHandicapHP() const;
  const JassPlayer& SetHandicapHP(float handicap_hp) const;

  // transform: normal_xp *= handicap_xp const;
  float GetHandicapXP() const;
  const JassPlayer& SetHandicapXP(float handicap_xp) const;

  RACE GetRace() const;

  int GetTeam() const;

  struct JassUnitGroup GetAllUnits() const;
  struct JassUnitGroup GetSelectedUnits() const;

  const JassPlayer& DisplayText(std::string const& text, float duration = 10.0f,
    float x = 0.0f, float y = 0.0f) const;

};

