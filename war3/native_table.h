#pragma once
#include "../pch.h"

// 1.20e
/* internal call stack construction information storage
grammar:
Hxxx; -> handle   7
B     -> boolean  8
C     -> code     3
R     -> real     5
S     -> string   6
I     -> integer  4
V     -> void     0

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
  UINT32 unused5, retType;
};

static_assert(sizeof(JassFnRegNode) == 60);

extern JassFnRegNode* GetNativeFuncNode(const char* name);
