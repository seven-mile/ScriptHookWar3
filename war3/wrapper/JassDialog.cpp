#include "../../pch.h"
#include "../func_callback.h"
#include "JassDialog.h"

JassDialog JassDialog::Create()
{
  return CallFn<HDialog>("DialogCreate");
}

void JassDialog::Destroy() const
{
  CallFn<void>("DialogDestroy", handle);
}

JassDialog const& JassDialog::Clear() const
{
  CallFn<void>("DialogClear", handle);

  // clear resources
  //for (auto&& trg : btnTrgs)
  //  CallFn<void>("DestroyTrigger", trg);
  //btnTrgs.clear();
  //for (auto &&code : btnTrgCodes)
  //  DestroyJassCallback(code);
  //btnTrgCodes.clear();

  return *this;
}

JassDialog const& JassDialog::SetText(std::string const& text) const
{
  CallFn<void>("DialogSetMessage", handle, text.c_str());
  return *this;
}

JassDialog const& JassDialog::AddButton(std::string const& text, int hotkey)
{
  CallFn<HDialogButton>("DialogAddButton", handle, text.c_str(), hotkey);
  return *this;
}

JassDialog const& JassDialog::AddButton(std::string const& text, std::function<void()> const& callback, int hotkey)
{
  auto btn = CallFn<HDialogButton>("DialogAddButton", handle, text.c_str(), hotkey);
  auto btnTrg = CallFn<HTrigger>("CreateTrigger");
  btnTrgs.push_back(btnTrg);
  auto cb = CreateJassCallback(callback);
  btnTrgCodes.push_back(cb);
  CallFn<HTriggerAction>("TriggerAddAction", btnTrg, cb);
  CallFn<HEvent>("TriggerRegisterDialogButtonEvent", btnTrg, btn);
  return *this;
}

void JassDialog::Display(JassPlayer ply) const
{
  CallFn<void>("DialogDisplay", ply.handle, handle, true);
}

void JassDialog::Hide(JassPlayer ply) const
{
  CallFn<void>("DialogDisplay", ply.handle, handle, false);
}


