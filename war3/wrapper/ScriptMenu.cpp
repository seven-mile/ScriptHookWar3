#include "../../pch.h"
#include "ScriptMenu.h"

#define str std::string const&
#define cbt std::function<void()> const&

ScriptMenu& ScriptMenu::Clear()
{
  for (auto z : list)
    delete z;
  list.clear();
  return *this;
}

ScriptMenu const& ScriptMenu::Display(JassPlayer ply) const
{
  dialog.Display(ply);
  return *this;
}

ScriptMenu& ScriptMenu::AddActionButton(str text, cbt clickEvent)
{
  list.emplace_back(new ButtonCtx {text, clickEvent});
  dialog.AddButton(text, clickEvent);

  return *this;
}

ScriptMenu& ScriptMenu::AddSwitchButton(
  std::string onText, std::string offText,
  std::function<void()> onEvent, std::function<void()> offEvent)
{
  auto ctx = new SwitchCtx{ offText, nullptr, false };
  auto func = [=, this](){
    if (ctx->state) {
      // turn off
      ctx->text = offText;
      ctx->state = false;
    } else {
      // turn on
      ctx->text = onText;
      ctx->state = true;
    }
    // update the dialog
    this->Rebuild();

    // call
    if (ctx->state) offEvent();
    else onEvent();
  };
  ctx->clickEvent = func;
  list.emplace_back(ctx);
  dialog.AddButton(offText, func);

  return *this;
}

ScriptMenu& ScriptMenu::AddSubMenuButton(str text, ScriptMenu& subMenu)
{
  auto ctx = new SubMenuCtx{ text, nullptr, subMenu.dialog };
  auto func = [this, ctx]() { ctx->dialog.Display(); };
  ctx->clickEvent = func;
  list.emplace_back(ctx);
  dialog.AddButton(text, func);

  return *this;
}

ScriptMenu& ScriptMenu::Rebuild()
{
  // clear all
  dialog.Clear();
  // now ctx is updated, rebuild all buttons
  for (auto ptr : list) {
    dialog.AddButton(ptr->text, ptr->clickEvent);
  }
  return *this;
}
