#pragma once
#include "JassHandle.h"
#include "JassWidget.h"

struct JassUnit : JassWidget
{
  JassUnit(HUnit handle) : JassWidget(handle) {  }
  // create JassUnit
  static JassUnit Create(struct JassPlayer owner, int unitId, float x, float y, float face, bool isCorpse = false);

  static JassUnit TriggerUnit();
  static JassUnit EnumUnit();
  static JassUnit FilterUnit();

  // action : management
  const JassUnit& Kill() const;
  const JassUnit& Remove() const;
  const JassUnit& ResetCooldown() const;

  const JassUnit& IssueOrder(std::string const&order) const;
  const JassUnit& IssueOrder(int orderId) const;

  const JassUnit& SetConstructionProgress(int percentage) const;
  const JassUnit& SetUpgradeProgress(int percentage) const;

  // property : getter/setter
  const JassUnit& SetShow(bool show) const;

  bool IsType(UNIT_TYPE type) const;

  RACE GetRace() const;

  const JassUnit& SetState(UNIT_STATE const& state, float value) const;
  float GetState(UNIT_STATE const& state) const;

  const JassUnit& SetOwner(struct JassPlayer owner, bool changeColor = true) const;
  struct JassPlayer GetOwner() const;

  const JassUnit& SetColor(PLAYER_COLOR const& color) const;
  PLAYER_COLOR GetColor() const;

  const JassUnit& SetX(float x) const;
  float GetX() const;
  const JassUnit& SetY(float y) const;
  float GetY() const;
  const JassUnit& SetPosition(float x, float y) const;

  const JassUnit& SetFacing(float facing) const;
  float GetFacing() const;

  const JassUnit& SetMoveSpeed(float speed) const;
  float GetMoveSpeed() const;

  const JassUnit& SetTurnSpeed(float speed) const;
  float GetTurnSpeed() const;

  const JassUnit& SetFlyHeight(float height) const;
  float GetFlyHeight() const;

  const JassUnit& SetScale(float scale) const;
  float GetScale() const;
};

