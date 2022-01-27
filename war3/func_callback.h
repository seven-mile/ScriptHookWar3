#pragma once
#include "../pch.h"

JassCode CreateJassCallback(const std::function<void()>& callback);
void DestroyJassCallback(JassCode code);
