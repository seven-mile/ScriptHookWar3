#pragma once
#include "../pch.h"
#include "func_call.h"

inline void DebugOutput(std::string const &str, float time = 10.0f) {
  CallFn<void>(
    "DisplayTimedTextToPlayer",
    CallFn<JassPlayer>("Player", 0),
    0.0f, 0.0f, time, str.c_str());
}
