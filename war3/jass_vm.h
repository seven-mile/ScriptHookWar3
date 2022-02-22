#pragma once
#include "hash_table.h"
#include "native_table.h"

size_t GetCurrentVM(int index = 1);
size_t GetCurrentCodeTable();
size_t GetCurrentCodeRelativeAddr();

reverse_table* GetSymbolTable();
table<>* GetNativeFuncTable();

