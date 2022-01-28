#pragma once
#include "../pch.h"

HCode CreateJassCallback(const std::function<void()>& callback);
void DestroyJassCallback(HCode code);
