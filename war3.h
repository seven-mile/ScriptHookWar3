#pragma once

// 1.20e
/* internal call stack construction information storage
grammar:
Hxxx; -> handle
B     -> boolean
R     -> real
S     -> string (special handle)
I     -> integer
V     -> void

function_name1 [function_names...] ( [argument_type_list] ) return_type ;
(align 4 bytes using spaces)
*/

struct JassFnRegNode {
  LPVOID vtable;
  UINT32 unused1, unused2, unused3, unused4;
  _Maybenull_ JassFnRegNode* nxtNode;
  char* fnName;
  PROC fnAddr;
  UINT32 argCount;
  char* protoStr;
  UINT32 argNameArrCount, argNameArrNonNullCount;
  _Maybenull_  char** fnArgNames; // c_str array
  UINT32 unused5, unused6;
};

static_assert(sizeof(JassFnRegNode) == 60);

inline std::unordered_map<std::string, JassFnRegNode*> mapFunc;
inline void RegenerateFunctionMap() {
  mapFunc.clear();

  // "war3.exe"+0005D428
  //    -> 14
  //    -> 24
  //    -> 0
  MemPtr base = (size_t)GetModuleHandle(nullptr);
  JassFnRegNode *head = MemRead<JassFnRegNode*>(MemRead(MemRead(
    base.address + 0x5D428) + 0x14) + 0x24);

  assert(head && "Invalid function list head pointer!");

  for (JassFnRegNode* node = head; (INT32)node > 0; node = node->nxtNode) {
    mapFunc[node->fnName] = node;
  }
}
