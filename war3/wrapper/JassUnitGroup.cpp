#include "../../pch.h"
#include "JassUnitGroup.h"
#include "../func_call.h"

JassUnitGroup JassUnitGroup::Create()
{
  return CallFn<HGroup>("CreateGroup");
}

void JassUnitGroup::Destroy()
{
  CallFn<void>("DestroyGroup", handle);
}

JassUnit JassUnitGroup::FirstUnit()
{
  return CallFn<HUnit>("FirstOfGroup", handle);
}

JassUnitGroup& JassUnitGroup::ForEach(std::function<void(JassUnit)> const& action)
{
  CallFn<void>("ForGroup", handle, [action]() {
    action(JassUnit::EnumUnit());
  });
  return *this;
}

JassUnitGroup& JassUnitGroup::Clear()
{
  CallFn<void>("GroupClear", handle);
  return *this;
}

JassUnitGroup& JassUnitGroup::PushBack(JassUnit unit)
{
  CallFn<void>("GroupAddUnit", handle, unit.handle);
  return *this;
}

size_t JassUnitGroup::Size()
{
  size_t ret_val = 0;
  this->ForEach([&](JassUnit){ ret_val++; });
  return ret_val;
}
