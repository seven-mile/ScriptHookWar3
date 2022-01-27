#pragma once
#include "../pch.h"

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

extern std::unordered_map<std::string, JassFnRegNode*> mapFunc;
void RegenerateFunctionMap();
