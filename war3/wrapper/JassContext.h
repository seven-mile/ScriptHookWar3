#pragma once
#include "JassWidget.h"
#include "JassUnit.h"
#include "JassItem.h"
#include "JassPlayer.h"
#include "../enum.h"
#include "../func_call.h"

#define DEFINE_CONTEXT(NAME, TYPE) \
static Jass##TYPE Get##NAME() { \
  return CallFn<HObject>("Get"#NAME); \
}

#define DEFINE_CONTEXT_PRIMITIVE(NAME, TYPE) \
static TYPE Get##NAME() { \
  return CallFn<TYPE>("Get"#NAME); \
}

#define DEFINE_CONTEXT_POINT(NAME) \
static struct { float x, y; } Get##NAME() { \
  return { \
    CallFn<float>("Get"#NAME"X"), \
    CallFn<float>("Get"#NAME"Y"), \
  }; \
}

struct JassContext
{
  // EVENT_PLAYER_HERO_LEVEL
  // EVENT_UNIT_HERO_LEVEL
  DEFINE_CONTEXT(LevelingUnit, Unit);
  
  // EVENT_PLAYER_HERO_SKILL
  // EVENT_UNIT_HERO_SKILL
  DEFINE_CONTEXT(LearningUnit, Unit);
  DEFINE_CONTEXT_PRIMITIVE(LearnedSkill, int);
  DEFINE_CONTEXT_PRIMITIVE(LearnedSkillLevel, int);

  // EVENT_PLAYER_HERO_REVIVABLE
  DEFINE_CONTEXT(RevivableUnit, Unit);

  // EVENT_PLAYER_HERO_REVIVE_START
  // EVENT_PLAYER_HERO_REVIVE_CANCEL
  // EVENT_PLAYER_HERO_REVIVE_FINISH
  // EVENT_UNIT_HERO_REVIVE_START
  // EVENT_UNIT_HERO_REVIVE_CANCEL
  // EVENT_UNIT_HERO_REVIVE_FINISH
  DEFINE_CONTEXT(RevivingUnit, Unit);

  // EVENT_PLAYER_UNIT_ATTACKED
  DEFINE_CONTEXT(Attacker, Unit);

  // EVENT_PLAYER_UNIT_RESCUED
  DEFINE_CONTEXT(Rescuer, Unit);

  // EVENT_PLAYER_UNIT_DEATH
  DEFINE_CONTEXT(DyingUnit, Unit);
  DEFINE_CONTEXT(KillingUnit, Unit);

  // EVENT_PLAYER_UNIT_DECAY
  DEFINE_CONTEXT(DecayingUnit, Unit);

  // EVENT_PLAYER_UNIT_SELECTED
  //DEFINE_CONTEXT(SelectedUnit , Unit);

  // EVENT_PLAYER_UNIT_CONSTRUCT_START
  DEFINE_CONTEXT(ConstructingStructure, Unit);

  // EVENT_PLAYER_UNIT_CONSTRUCT_FINISH
  // EVENT_PLAYER_UNIT_CONSTRUCT_CANCEL
  DEFINE_CONTEXT(CancelledStructure, Unit);
  DEFINE_CONTEXT(ConstructedStructure, Unit);

  // EVENT_PLAYER_UNIT_RESEARCH_START
  // EVENT_PLAYER_UNIT_RESEARCH_CANCEL
  // EVENT_PLAYER_UNIT_RESEARCH_FINISH
  DEFINE_CONTEXT(ResearchingUnit, Unit);
  DEFINE_CONTEXT_PRIMITIVE(Researched, int);

  // EVENT_PLAYER_UNIT_TRAIN_START
  // EVENT_PLAYER_UNIT_TRAIN_CANCEL
  DEFINE_CONTEXT_PRIMITIVE(TrainedUnitType, int);

  // EVENT_PLAYER_UNIT_TRAIN_FINISH
  DEFINE_CONTEXT(TrainedUnit, Unit);

  // EVENT_PLAYER_UNIT_DETECTED
  DEFINE_CONTEXT(DetectedUnit, Unit);

  // EVENT_PLAYER_UNIT_SUMMONED
  DEFINE_CONTEXT(SummoningUnit, Unit);
  DEFINE_CONTEXT(SummonedUnit, Unit);

  // EVENT_PLAYER_UNIT_LOADED
  DEFINE_CONTEXT(TransportUnit, Unit);
  DEFINE_CONTEXT(LoadedUnit, Unit);

  // EVENT_PLAYER_UNIT_SELL
  DEFINE_CONTEXT(SellingUnit, Unit);
  DEFINE_CONTEXT(SoldUnit, Unit);
  DEFINE_CONTEXT(BuyingUnit, Unit);

  // EVENT_PLAYER_UNIT_SELL_ITEM
  DEFINE_CONTEXT(SoldItem, Item);

  // EVENT_PLAYER_UNIT_CHANGE_OWNER
  DEFINE_CONTEXT(ChangingUnit, Unit);
  DEFINE_CONTEXT(ChangingUnitPrevOwner, Player);

  // EVENT_PLAYER_UNIT_DROP_ITEM
  // EVENT_PLAYER_UNIT_PICKUP_ITEM
  // EVENT_PLAYER_UNIT_USE_ITEM
  DEFINE_CONTEXT(ManipulatingUnit, Unit);
  DEFINE_CONTEXT(ManipulatedItem, Item);


  // EVENT_PLAYER_UNIT_ISSUED_ORDER
  // EVENT_UNIT_ISSUED_ORDER
  DEFINE_CONTEXT_PRIMITIVE(IssuedOrderId, ORDER_ID);

  DEFINE_CONTEXT(OrderedUnit, Unit);

  // EVENT_PLAYER_UNIT_ISSUED_POINT_ORDER
  // EVENT_UNIT_ISSUED_POINT_ORDER
  DEFINE_CONTEXT_POINT(OrderPoint);

  // EVENT_PLAYER_UNIT_ISSUED_TARGET_ORDER
  // EVENT_UNIT_ISSUED_TARGET_ORDER
  DEFINE_CONTEXT(OrderTarget, Widget);;
  DEFINE_CONTEXT(OrderTargetItem, Item);;
  DEFINE_CONTEXT(OrderTargetUnit, Unit);;

  // EVENT_UNIT_SPELL_CHANNEL
  // EVENT_UNIT_SPELL_CAST
  // EVENT_UNIT_SPELL_EFFECT
  // EVENT_UNIT_SPELL_FINISH
  // EVENT_UNIT_SPELL_ENDCAST
  // EVENT_PLAYER_UNIT_SPELL_CHANNEL
  // EVENT_PLAYER_UNIT_SPELL_CAST
  // EVENT_PLAYER_UNIT_SPELL_EFFECT
  // EVENT_PLAYER_UNIT_SPELL_FINISH
  // EVENT_PLAYER_UNIT_SPELL_ENDCAST
  DEFINE_CONTEXT(SpellAbilityUnit, Unit);
  DEFINE_CONTEXT_PRIMITIVE(SpellAbilityId, int);
  //DEFINE_CONTEXT(SpellAbility, Ability);
  DEFINE_CONTEXT_POINT(SpellTarget);
  DEFINE_CONTEXT(SpellTargetItem, Item);
  DEFINE_CONTEXT(SpellTargetUnit, Unit);
  
  // EVENT_PLAYER_CHAT

  // returns the actual string they typed in ( same as what you registered for
  // if you required exact match )
  static std::string GetEventPlayerChatString() {
    return CallFn<std::string>("GetEventPlayerChatString");
  }

  // returns the string that you registered for
  static std::string GetEventPlayerChatStringMatched() {
    return CallFn<std::string>("GetEventPlayerChatStringMatched");
  }

  DEFINE_CONTEXT(TriggerUnit, Unit);
  DEFINE_CONTEXT(EnumUnit, Unit);
  DEFINE_CONTEXT(FilterUnit, Unit);

  DEFINE_CONTEXT(LocalPlayer, Player);
  DEFINE_CONTEXT_PRIMITIVE(WorldBounds, HRect);
};

using JCtx = JassContext;
