#pragma once
#include "JassUnit.h"
struct JassHero : public JassUnit
{
  JassHero(HUnit handle) : JassUnit(handle) { }
  JassHero(JassUnit unit) : JassUnit(nullptr) {
    if (unit.IsType(UNIT_TYPE::HERO))
      handle = unit.handle;
  }

  const JassHero& SetStrength(int value) const;
  int GetStrength() const;
  const JassHero& SetAgility(int value) const;
  int GetAgility() const;
  const JassHero& SetIntelligence(int value) const;
  int GetIntelligence() const;
  const JassHero& SetXP(int value, bool eye_candy) const;
  const JassHero& AddXP(int value, bool eye_candy) const;
  int GetXP() const;
  const JassHero& SetLevel(int value, bool eye_candy) const;
  int GetLevel() const;
};

