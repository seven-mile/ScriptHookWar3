#include "../../pch.h"
#include "JassHero.h"
#include "../func_call.h"

const JassHero& JassHero::SetStrength(int value) const
{
  CallFn<void>("SetHeroStr", handle, value);
  return *this;
}

int JassHero::GetStrength() const
{
  return CallFn<int>("GetHeroStr", handle);
}

const JassHero& JassHero::SetAgility(int value) const
{
  CallFn<void>("SetHeroAgi", handle, value);
  return *this;
}

int JassHero::GetAgility() const
{
  return CallFn<int>("GetHeroAgi", handle);
}

const JassHero& JassHero::SetIntelligence(int value) const
{
  CallFn<void>("SetHeroInt", handle, value);
  return *this;
}

int JassHero::GetIntelligence() const
{
  return CallFn<int>("GetHeroInt", handle);
}

const JassHero& JassHero::SetXP(int value, bool eye_candy) const
{
  CallFn<void>("SetHeroXP", handle, value, eye_candy);
  return *this;
}

const JassHero& JassHero::AddXP(int value, bool eye_candy) const
{
  CallFn<void>("AddHeroXP", handle, value, eye_candy);
  return *this;
}

int JassHero::GetXP() const
{
  return CallFn<int>("GetHeroXP", handle);
}

const JassHero& JassHero::SetLevel(int value, bool eye_candy) const
{
  CallFn<void>("SetHeroLevel", handle, value, eye_candy);
  return *this;
}

int JassHero::GetLevel() const
{
  return CallFn<int>("GetHeroLevel", handle);
}
