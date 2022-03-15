// Reference: https://github.com/actboy168/YDWE/blob/master/Development/Core/ydwar3/warcraft3/jass/nf_register.cpp

#include "../pch.h"
#include "game_end_event.h"

#include <detours/detours.h>

#include <ranges>

static DWORD hScriptThread = static_cast<DWORD>(-1);
static size_t cntGameEndHandler = 0;
static std::unordered_map<size_t, std::function<void()>> mapGameEndHandler;
static std::vector<std::function<void()>> vecOnceGameEndHandler;

inline void CallGameEndHandlers() {
  for (auto&& handler : mapGameEndHandler | std::ranges::views::values) {
    handler();
  }
  for (auto&& handler : vecOnceGameEndHandler) {
    handler();
  }
  vecOnceGameEndHandler.clear();
}

static decltype(&::TlsGetValue) pfnRealTlsGetValue = &::TlsGetValue;
static uintptr_t(__stdcall* pfnRealStormAlloc)(uint32_t amount, const char* log_filename, uint32_t log_line, uint32_t default_value);
static int game_tls_state = 0;

LPVOID WINAPI HookTlsGetValue(
  _In_ DWORD dwTlsIndex
) {
  if (game_tls_state && ::GetCurrentThreadId() == hScriptThread) {
    if (game_tls_state == 2) CallGameEndHandlers();
    if (game_tls_state < 3) game_tls_state++;
  }
  return pfnRealTlsGetValue(dwTlsIndex);
}

uintptr_t __stdcall HookSMemAlloc(uint32_t amount, const char* log_filename, uint32_t log_line, uint32_t default_value)
{
  if ((amount == 176u)
    && (std::abs((int32_t)log_line - 668) <= 1)
    && (default_value == 0)
    && std::string_view(log_filename).ends_with("\\Jass.cpp"))
  {
    game_tls_state = 1;
    hScriptThread = ::GetCurrentThreadId();
  } else if (game_tls_state == 3)
  {
    if (hScriptThread == ::GetCurrentThreadId())
    {
      if ((amount == 140)
        && (log_line == 90)
        && (default_value == 0)
        && std::string_view(log_filename).ends_with("\\Agile.cpp"))
      {
        game_tls_state = 0;
        CallGameEndHandlers();
      }
    }
  }

  return pfnRealStormAlloc(amount, log_filename, log_line, default_value);
}

void InitGameEndEvent()
{
  // install hook
  ::DetourTransactionBegin();
  ::DetourUpdateThread(::GetCurrentThread());
  ::DetourAttach(&(PVOID&)pfnRealTlsGetValue, HookTlsGetValue);
  pfnRealStormAlloc = reinterpret_cast<decltype(pfnRealStormAlloc)>
    (/*(size_t)GetModuleHandle(L"Game.dll") + 0x6EBD52*/
      DetourFindFunction("Storm.dll", (LPCSTR)401));
  if (!pfnRealStormAlloc) {
    ::MessageBox(nullptr,
      L"Failed to install game reset hook,\n"
      L"ScriptHookWar3 is disabled.\n"
      L"We only support 1.20e and 1.24e\n"
      L"But you can continue the game without the plugin.",
      L"ScriptHookWar3", MB_ICONERROR);
    exit(-1);
  }
  ::DetourAttach(&(PVOID&)pfnRealStormAlloc, HookSMemAlloc);

  ::DetourTransactionCommit();
}

size_t RegisterGameEndEvent(std::function<void()> const& event)
{
  mapGameEndHandler[++cntGameEndHandler] = event;
  return cntGameEndHandler;
}

void UnregisterGameEndEvent(size_t id)
{
  mapGameEndHandler.erase(id);
}

void RegisterOnlyNextGameEndEvent(std::function<void()> const& event)
{
  vecOnceGameEndHandler.push_back(event);
}
