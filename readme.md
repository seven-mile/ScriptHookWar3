# ScriptHookWar3

### Install

This is an **ASI Plugin**! You need to install an ASI Loader first, and put the output `ScriptHookWar3.asi` in a proper directory to load it.

### Usage

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

1.28 update: **C++ Callback is supported!**

```cpp
auto trg = CallFn<JassTrigger>("CreateTrigger");
auto act = CallFn<JassTriggerAction>("TriggerAddAction", trg, []() {
  DebugOutput("ScriptHookWar3: Action triggered!");
});
CallFn<JassEvent>("TriggerRegisterPlayerChatEvent", trg, ply, "war", true);
```

Yes, just pass lambda an it will work!

Oh, you don't need callback arguments, since you already have lambda capturing.

### Game Version

We only support 1.20e by now. But it's easy to adapt other versions if you just need `CallFn<RetTy>`(2 offsets to find: the timer code and the native function pool). The native function pool structure turns into a BST for 1.3+, which needs some extra efforts.

Anyway, I'm focusing on exploring more features rather than support more versions stably. Higher versions will come, but not now I think.

### Coming Refactor

I will refactor the code recently. The structure is kind of confusing, and performance optimization is also needed.

### Todo

Now I'm working on providing **C++ function callback binding**. Then you can pass C++ function pointer to a function taking `code`-typed arguments, like `TriggerAddAction`, which provides important event system!

After that, I'm considering write a wrapper for `Dialog`. And maybe OOP wrapper for more jass types, if I'd love to.

1.28 update: They are close to a finish. Now I will try to build a NativeTrainer, which providing similar interface to HKE script inject. But this time the injection happens at runtime and we don't need to modify the map.