#include "../../pch.h"
#include "JassUnit.h"
#include "JassPlayer.h"
#include "../func_call.h"

JassUnit JassUnit::Create(JassPlayer owner, int unitId, float x, float y, float face, bool isCorpse) {
  return CallFn<HUnit>(isCorpse ? "CreateCorpse" : "CreateUnit", owner.handle, unitId, x, y, face);
}

JassUnit JassUnit::Create(JassPlayer owner, int unitId, HLocation location, float face)
{
  return CallFn<HUnit>("CreateUnitAtLoc", owner.handle, unitId, location, face);
}

JassUnit JassUnit::TriggerUnit()
{
  return CallFn<HUnit>("GetTriggerUnit");
}

JassUnit JassUnit::EnumUnit()
{
  return CallFn<HUnit>("GetEnumUnit");
}

JassUnit JassUnit::FilterUnit()
{
  return CallFn<HUnit>("GetFilterUnit");
}

const JassUnit& JassUnit::Kill() const
{
  CallFn<void>("KillUnit", handle);
  return *this;
}

const JassUnit& JassUnit::Remove() const
{
  CallFn<void>("RemoveUnit", handle);
  return *this;
}

const JassUnit& JassUnit::ResetCooldown() const
{
  CallFn<void>("UnitResetCooldown", handle);
  return *this;
}

const JassUnit& JassUnit::IssueOrder(std::string const& order) const
{
  CallFn<bool>("IssueImmediateOrder", handle, order.c_str());
  return *this;
}

const JassUnit& JassUnit::IssueOrder(int orderId) const
{
  CallFn<bool>("IssueImmediateOrderById", handle, orderId);
  return *this;
}

const JassUnit& JassUnit::SetConstructionProgress(int percentage) const
{
  CallFn<void>("UnitSetConstructionProgress", handle, percentage);
  return *this;
}

const JassUnit& JassUnit::SetUpgradeProgress(int percentage) const
{
  CallFn<void>("UnitSetUpgradeProgress", handle, percentage);
  return *this;
}

const JassUnit& JassUnit::SetShow(bool show) const
{
  CallFn<void>("ShowUnit", handle, show);
  return *this;
}

bool JassUnit::IsType(UNIT_TYPE type) const
{
  return CallFn<bool>("IsUnitType", handle, type);
}

const JassUnit& JassUnit::AddType(UNIT_TYPE type) const
{
  CallFn<void>("UnitAddType", handle, type);
  return *this;
}

const JassUnit& JassUnit::RemoveType(UNIT_TYPE type) const
{
  CallFn<void>("UnitRemoveType", handle, type);
  return *this;
}

int JassUnit::GetTypeId() const
{
  return CallFn<int>("GetUnitTypeId", handle);
}

RACE JassUnit::GetRace() const
{
  return CallFn<RACE>("GetUnitRace", handle);
}

int JassUnit::GetLevel() const
{
    return CallFn<int>("GetUnitLevel", handle);
}

HLocation JassUnit::GetRallyPoint() const
{
  return CallFn<HLocation>("GetUnitRallyPoint", handle);
}

const JassUnit& JassUnit::SetState(UNIT_STATE const& state, float value) const
{
  CallFn<void>("SetUnitState", handle, (size_t)state, value);
  return *this;
}

float JassUnit::GetState(UNIT_STATE const& state) const
{
  return CallFn<float>("GetUnitState", handle, (size_t)state);
}

const JassUnit& JassUnit::SetOwner(JassPlayer owner, bool changeColor) const
{
  CallFn<void>("SetUnitOwner", handle, owner.handle, changeColor);
  return *this;
}

JassPlayer JassUnit::GetOwner() const
{
  return CallFn<HPlayer>("GetOwningPlayer", handle);
}

const JassUnit& JassUnit::SetColor(PLAYER_COLOR const& color) const
{
  CallFn<void>("SetUnitColor", handle, (size_t)color);
  return *this;
}

PLAYER_COLOR JassUnit::GetColor() const
{
  return CallFn<PLAYER_COLOR>("GetUnitColor", handle);
}

const JassUnit& JassUnit::SetX(float x) const
{
  CallFn<void>("SetUnitX", handle, x);
  return *this;
}

float JassUnit::GetX() const
{
  return CallFn<float>("GetUnitX", handle);
}

const JassUnit& JassUnit::SetY(float y) const
{
  CallFn<void>("SetUnitY", handle, y);
  return *this;
}

float JassUnit::GetY() const
{
  return CallFn<float>("GetUnitY", handle);
}

const JassUnit& JassUnit::SetPosition(float x, float y) const
{
  this->SetX(x), this->SetY(y);
  return *this;
}

const JassUnit& JassUnit::SetFacing(float facing) const
{
  CallFn<void>("SetUnitFacing", handle, facing);
  return *this;
}

float JassUnit::GetFacing() const
{
  return CallFn<float>("GetUnitFacing", handle);
}

const JassUnit& JassUnit::SetMoveSpeed(float speed) const
{
  CallFn<void>("SetUnitMoveSpeed", handle, speed);
  return *this;
}

float JassUnit::GetMoveSpeed() const
{
  return CallFn<float>("GetUnitMoveSpeed", handle);
}

const JassUnit& JassUnit::SetTurnSpeed(float speed) const
{
  CallFn<void>("SetUnitTurnSpeed", handle, speed);
  return *this;
}

float JassUnit::GetTurnSpeed() const
{
  return CallFn<float>("GetUnitTurnSpeed", handle);
}

const JassUnit& JassUnit::SetFlyHeight(float height) const
{
  CallFn<void>("SetUnitFlyHeight", handle, height);
  return *this;
}

float JassUnit::GetFlyHeight() const
{
  return CallFn<float>("GetUnitFlyHeight", handle);
}

const JassUnit& JassUnit::SetScale(float scale) const
{
  CallFn<void>("SetUnitScale", handle, scale);
  return *this;
}

float JassUnit::GetScale() const
{
  return CallFn<float>("GetUnitScale", handle);
}
