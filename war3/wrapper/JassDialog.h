#pragma once
#include "JassHandle.h"
#include "JassPlayer.h"
#include "../func_call.h"

struct JassDialog : public JassHandle
{
  JassDialog(HDialog handle) : JassHandle(handle) { }

  static JassDialog Create();
  void Destroy();
  JassDialog& Clear();
  JassDialog& SetText(std::string const& text);

  JassDialog& AddButton(std::string const& text, int hotkey = 0);
  JassDialog& AddButton(std::string const& text, std::function<void()> const& callback, int hotkey = 0);

  void Display(JassPlayer ply = JassPlayer::LocalPlayer());
  void Hide(JassPlayer ply = JassPlayer::LocalPlayer());

private:
  std::vector<HTrigger> btnTrgs;
  std::vector<HCode> btnTrgCodes;
};

