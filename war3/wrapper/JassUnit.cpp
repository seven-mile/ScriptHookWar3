#include "../../pch.h"
#include "JassUnit.h"
#include "JassPlayer.h"
#include "../func_call.h"

JassUnit::JassUnit(JassPlayer owner, int unitId, float x, float y, float face, bool isCorpse) 
  : JassWidget(CallFn<HUnit>(
    isCorpse ? "CreateCorpse" : "CreateUnit",
    unitId, x, y, face)) {  }

JassUnit JassUnit::EnumUnit()
{
  return CallFn<HUnit>("GetEnumUnit");
}

JassUnit JassUnit::FilterUnit()
{
  return CallFn<HUnit>("GetFilterUnit");
}

void JassUnit::Kill()
{
  CallFn<void>("KillUnit", handle);
}

void JassUnit::Remove()
{
  CallFn<void>("RemoveUnit", handle);
}

void JassUnit::SetShow(bool show)
{
  CallFn<void>("ShowUnit", handle, show);
}

bool JassUnit::IsType(UNIT_TYPE type)
{
  return CallFn<bool>("IsUnitType", handle, type);
}

RACE JassUnit::GetRace()
{
  return CallFn<RACE>("GetUnitRace", handle);
}

void JassUnit::SetState(UNIT_STATE const& state, float value)
{
  CallFn<void>("SetUnitState", handle, (size_t)state, value);
}

float JassUnit::GetState(UNIT_STATE const& state)
{
  return CallFn<float>("GetUnitState", handle, (size_t)state);
}

void JassUnit::SetOwner(JassPlayer owner, bool changeColor)
{
  CallFn<void>("SetUnitOwner", handle, owner.handle, changeColor);
}

JassPlayer JassUnit::GetOwner()
{
  return CallFn<HPlayer>("GetOwningPlayer", handle);
}

void JassUnit::SetColor(PLAYER_COLOR const& color)
{
  CallFn<void>("SetUnitColor", handle, (size_t)color);
}

PLAYER_COLOR JassUnit::GetColor()
{
  return CallFn<PLAYER_COLOR>("GetUnitColor", handle);
}

void JassUnit::SetX(float x)
{
  CallFn<void>("SetUnitX", handle, x);
}

float JassUnit::GetX()
{
  return CallFn<float>("GetUnitX", handle);
}

void JassUnit::SetY(float y)
{
  CallFn<void>("SetUnitY", handle, y);
}

float JassUnit::GetY()
{
  return CallFn<float>("GetUnitY", handle);
}

void JassUnit::SetPosition(float x, float y)
{
  this->SetX(x), this->SetY(y);
}

void JassUnit::SetFacing(float facing)
{
  CallFn<void>("SetUnitFacing", handle, facing);
}

float JassUnit::GetFacing()
{
  return CallFn<float>("GetUnitFacing", handle);
}

void JassUnit::SetMoveSpeed(float speed)
{
  CallFn<void>("SetUnitMoveSpeed", handle, speed);
}

float JassUnit::GetMoveSpeed()
{
  return CallFn<float>("GetUnitMoveSpeed", handle);
}

void JassUnit::SetTurnSpeed(float speed)
{
  CallFn<void>("SetUnitTurnSpeed", handle, speed);
}

float JassUnit::GetTurnSpeed()
{
  return CallFn<float>("GetUnitTurnSpeed", handle);
}

void JassUnit::SetFlyHeight(float height)
{
  CallFn<void>("SetUnitFlyHeight", handle, height);
}

float JassUnit::GetFlyHeight()
{
  return CallFn<float>("GetUnitFlyHeight", handle);
}

void JassUnit::SetScale(float scale)
{
  CallFn<void>("SetUnitScale", handle, scale);
}

float JassUnit::GetScale()
{
  return CallFn<float>("GetUnitScale", handle);
}
