#pragma once
#include "JassHandle.h"
#include "JassUnit.h"
struct JassUnitGroup : public JassHandle
{
  JassUnitGroup(HObject handle) : JassHandle(handle) {  }

  JassUnitGroup Create();
  void Destroy();

  JassUnit FirstUnit();
  JassUnitGroup& ForEach(std::function<void(JassUnit)> const& action);
  JassUnitGroup& Clear();
  JassUnitGroup& PushBack(JassUnit unit);
  size_t Size();
};

