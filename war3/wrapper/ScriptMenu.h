#pragma once
#include "JassDialog.h"

#define str std::string const&
#define cbt std::function<void()> const&
struct ScriptMenu
{
  ScriptMenu& Clear();
  ScriptMenu const& Display(JassPlayer ply = JassPlayer::LocalPlayer()) const;
  ScriptMenu& AddActionButton(str text, cbt clickEvent);
  ScriptMenu& AddSwitchButton(std::string onText, std::string offText,
    std::function<void()> onEvent, std::function<void()> offEvent);
  ScriptMenu& AddSubMenuButton(str text, ScriptMenu& subMenu);
  ScriptMenu& Rebuild();

private:

  JassDialog dialog = JassDialog::Create();
  struct ButtonCtx {
    std::string text;
    std::function<void()> clickEvent;
  };
  struct SwitchCtx : ButtonCtx {
    bool state;
  };
  struct SubMenuCtx : ButtonCtx {
    JassDialog dialog;
  };
  std::vector<ButtonCtx*> list;
};
#undef cbt
