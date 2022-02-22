#include "pch.h"
#include "keyboard_manager.h"

#include <ranges>

using tick_t = decltype(GetTickCount64());

struct hotkey_struct
{
  int vk;
  bool ctrl, alt, shift;
  std::function<void()> handler;
  tick_t last_tick;
};

static size_t cntHotkeys = 0;
static std::map<hotkey_t, hotkey_struct> hotkeys;

hotkey_t KeyboardManager::Register(
  const std::function<void()>& handler,
  int vk, bool ctrl, bool alt, bool shift)
{
  hotkeys[++cntHotkeys] = hotkey_struct{ vk, ctrl, alt, shift, handler };
  return cntHotkeys;
}

void KeyboardManager::Unregister(hotkey_t hotkey)
{
  hotkeys.erase(hotkey);
}

inline bool IsKeyDown(int code) {
  return GetAsyncKeyState(code) & (1u << 31);
}

void KeyboardManager::Check(int noRespTimeInMs)
{
  auto curTick = GetTickCount64();
  for (auto& hotkey : hotkeys | std::views::values)
  {
    if (hotkey.last_tick + noRespTimeInMs < curTick
      && IsKeyDown(hotkey.vk)
      && (!hotkey.ctrl || IsKeyDown(VK_CONTROL))
      && (!hotkey.alt  || IsKeyDown(VK_MENU))
      && (!hotkey.shift || IsKeyDown(VK_SHIFT)))
    {
      hotkey.handler();
    }
  }
}
