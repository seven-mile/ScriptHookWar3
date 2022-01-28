#pragma once
#include "JassHandle.h"
#include "../enum.h"

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
  void SetTechResearchLevel(int techCode, int level);
  bool GetTechResearchState(int techCode);

  std::string GetName();
  void SetName(std::string const& name);

  PLAYER_COLOR GetColor();
  void SetColor(PLAYER_COLOR const& color);

  int GetState(PLAYER_STATE const& state);
  void SetState(PLAYER_STATE const& state, int value);

  // transform: normal_hp *= handicap_hp;
  float GetHandicapHP();
  void SetHandicapHP(float handicap_hp);

  // transform: normal_xp *= handicap_xp;
  float GetHandicapXP();
  void SetHandicapXP(float handicap_xp);

  RACE GetRace();

  int GetTeam();

  struct JassUnitGroup GetAllUnits();
  struct JassUnitGroup GetSelectedUnits();

  void DisplayText(std::string const& text, float duration = 10.0f,
    float x = 0.0f, float y = 0.0f);

};

