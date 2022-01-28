#pragma once
#include "JassHandle.h"
#include "JassWidget.h"

struct JassUnit : JassWidget
{
  JassUnit(HUnit handle) : JassWidget(handle) {  }
  // create JassUnit
  JassUnit(struct JassPlayer owner, int unitId, float x, float y, float face, bool isCorpse = false);

  // action : management
  void Kill();
  void Remove();


  // property : getter/setter
  void SetShow(bool show);

  RACE GetRace();

  void SetState(UNIT_STATE const& state, float value);
  float GetState(UNIT_STATE const& state);

  void SetOwner(struct JassPlayer owner, bool changeColor = true);
  struct JassPlayer GetOwner();

  void SetColor(PLAYER_COLOR const& color);
  PLAYER_COLOR GetColor();

  void SetX(float x);
  float GetX();
  void SetY(float y);
  float GetY();
  void SetPosition(float x, float y);

  void SetFacing(float facing);
  float GetFacing();

  void SetMoveSpeed(float speed);
  float GetMoveSpeed();

  void SetTurnSpeed(float speed);
  float GetTurnSpeed();

  void SetFlyHeight(float height);
  float GetFlyHeight();

  void SetScale(float scale);
  float GetScale();
};

