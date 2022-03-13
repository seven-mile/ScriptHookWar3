#pragma once
#include "JassHandle.h"
#include "JassPlayer.h"
#include "../func_call.h"

struct JassDialog : public JassHandle
{
  JassDialog(HDialog handle) : JassHandle(handle) { }

  static JassDialog Create();
  void Destroy() const;
  JassDialog const& Clear() const;
  JassDialog const& SetText(std::string const& text) const;

  JassDialog const& AddButton(std::string const& text, int hotkey = 0);
  JassDialog const& AddButton(std::string const& text, std::function<void()> const& callback, int hotkey = 0);

  void Display(JassPlayer ply = JassPlayer::LocalPlayer()) const;
  void Hide(JassPlayer ply = JassPlayer::LocalPlayer()) const;

private:
  std::vector<HTrigger> btnTrgs;
  std::vector<HCode> btnTrgCodes;
};

