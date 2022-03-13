#pragma once

void InitGameEndEvent();
size_t RegisterGameEndEvent(std::function<void()> const &event);
void UnregisterGameEndEvent(size_t id);
void RegisterOnlyNextGameEndEvent(std::function<void()> const& event);
