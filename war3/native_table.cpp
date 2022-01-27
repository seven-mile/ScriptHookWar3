#include "../pch.h"
#include "native_table.h"

std::unordered_map<std::string, JassFnRegNode*> mapFunc;

void RegenerateFunctionMap() {

  // "war3.exe"+0005D428
  //    -> 14
  //    -> 24
  MemPtr base = (size_t)GetModuleHandle(nullptr);
  JassFnRegNode* head = MemRead<JassFnRegNode*>(MemRead(MemRead(
    base.address + 0x5D428) + 0x14) + 0x24);

  assert(head && "Invalid function list head pointer!");

  static JassFnRegNode* lastHead = nullptr;
  if (lastHead == head) return;
  lastHead = head;

  // remap
  mapFunc.clear();
  for (JassFnRegNode* node = head; (INT32)node > 0; node = node->nxtNode) {
    mapFunc[node->fnName] = node;
  }
}
