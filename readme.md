# ScriptHookWar3

### Install

This is an **ASI Plugin**! You need to install an ASI Loader first, and put the output `ScriptHookWar3.asi` in a proper directory to load it.

### Usage

#### Calling Jass Native Functions

With the C++ template, ScriptHookWar3 provides a graceful way to call native jass functions:

```cpp
const int PLAYERID_LOCAL = 0;
auto ply = CallFn<JassPlayer>("Player", PLAYERID_LOCAL);
auto grp = CallFn<JassGroup>("CreateGroup");
CallFn<void>("GroupEnumUnitsSelected", grp, ply, false);
auto unit = CallFn<JassUnit>("FirstOfGroup", grp);
const int UNIT_TYPE_HERO = 0;
if (CallFn<bool>("IsUnitType", unit, UNIT_TYPE_HERO)) {
  CallFn<void>("AddHeroXP", unit, 1000, true);

  CallFn<void>(
    "DisplayTimedTextToPlayer",
    CallFn<JassPlayer>("Player", 0),
    0.0f,
    0.0f,
    10.0f,
    "ScriptHookWar3: Added!"
  );
} else {
  CallFn<void>(
    "DisplayTimedTextToPlayer",
    CallFn<JassPlayer>("Player", 0),
    0.0f,
    0.0f,
    10.0f,
    "ScriptHookWar3: The selected unit is not hero."
  );
}


```

#### Function Callback

1.28 update: **C++ Callback is supported!**

```cpp
auto trg = CallFn<JassTrigger>("CreateTrigger");
auto act = CallFn<JassTriggerAction>("TriggerAddAction", trg, []() {
  DebugOutput("ScriptHookWar3: Action triggered!");
});
CallFn<JassEvent>("TriggerRegisterPlayerChatEvent", trg, ply, "war", true);
```

Yes, just pass lambda and it will work!

Oh, you don't need callback arguments, since you already have lambda capturing.

#### C++ OOP Wrapper

I wrote some class wrapper for Jass objects, so that you can use

```cpp
auto ply = JassPlayer::LocalPlayer();
if (JassHero hero = ply.GetSelectedUnits().FirstUnit()) {
  hero.SetMoveSpeed(2 * hero.GetMoveSpeed());
} else {
  DebugOutput("oops, it is NOT a HERO.");
}
```

#### Menu Creator API

It's an extremely powerful shortcut for game menu creation.

The so-called menu is based on game-native dialog and supports ordinary action buttons, switch buttons and navigation between menus.

For example, to create a menu structure like:

```
Resource
-- Add Money
-- Add Lumber

Selected Unit
-- Scale up / Scale down (change its text and behavior after clicked)
```

We can simply write

```cpp
// define your menus
static ScriptMenu mainMenu, resMenu, unitMenu;

// define main menu layout, there're two submenus
mainMenu.AddSubMenuButton("Resource", resMenu);
mainMenu.AddSubMenuButton("Selected Unit", unitMenu);
// this button exit the menu
mainMenu.AddActionButton("Exit Menu", [](){});

// define two action buttons
resMenu.AddActionButton("Add Money", []() {
    // It's simple, right? I love it.
    auto ply = JassPlayer::LocalPlayer();
    ply.SetState(PLAYER_STATE::RESOURCE_GOLD,
        ply.GetState(PLAYER_STATE::RESOURCE_GOLD) + 9999);
});
resMenu.AddActionButton("Add Lumber", []() {
    auto ply = JassPlayer::LocalPlayer();
    ply.SetState(PLAYER_STATE::RESOURCE_LUMBER,
        ply.GetState(PLAYER_STATE::RESOURCE_LUMBER) + 9999);
});
// this button return to the previous level
resMenu.AddSubMenuButton("Return", mainMenu);
resMenu.AddActionButton("Exit Menu", [](){});

unitMenu.AddSwitchButton("Scale Down", "Scale Up",
[]() {
    auto unit = JassPlayer::LocalPlayer().GetSelectedUnits().FirstUnit();
    unit.SetScale(1.0);
},
[]() {
    auto unit = JassPlayer::LocalPlayer().GetSelectedUnits().FirstUnit();
    unit.SetScale(2.0);
});
unitMenu.AddSubMenuButton("Return", mainMenu);
unitMenu.AddActionButton("Exit Menu", []() {});
```

### Game Version

We only support 1.20e by now. But it's easy to adapt other versions if you just need `CallFn<RetTy>`(2 offsets to find: the timer code and the native function pool). The native function pool structure turns into a BST for 1.3+, which needs some extra efforts.

Anyway, I'm focusing on exploring more features rather than support more versions stably. Higher versions will come, but not now I think.

### Coming Refactor

I will refactor the code recently. The structure is kind of confusing, and performance optimization is also needed.

2.7 update: I will probably update this project to C++ 20 Module architecture ;)

### Todo

Now I'm working on providing **C++ function callback binding**. Then you can pass C++ function pointer to a function taking `code`-typed arguments, like `TriggerAddAction`, which provides important event system!

After that, I'm considering write a wrapper for `Dialog`. And maybe OOP wrapper for more jass types, if I'd love to.

1.28 update: They are close to a finish. Now I will try to build a NativeTrainer, which providing similar interface to HKE script inject. But this time the injection happens at runtime and we don't need to modify the map.