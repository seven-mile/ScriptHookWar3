#pragma once

// handle
using hotkey_t = size_t;

struct KeyboardManager
{
  static hotkey_t Register(const std::function<void()> &handler,
    int vk, bool ctrl = false, bool alt = false, bool shift = false);
  static void Unregister(hotkey_t hotkey);

  static void Check(int noRespTimeInMs = 100);
};

