#pragma once
#include "JassUnit.h"
struct JassHero : public JassUnit
{
  JassHero(HUnit handle) : JassUnit(handle) { }
  JassHero(JassUnit unit) : JassUnit(nullptr) {
    if (unit.IsType(UNIT_TYPE::HERO))
      handle = unit.handle;
  }
};

