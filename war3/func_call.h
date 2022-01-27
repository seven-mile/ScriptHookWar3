#pragma once
#include "../pch.h"
#include "jass_string.h"
#include "func_callback.h"

inline void CollectArgs(LPVOID* stackBin, std::vector<std::function<void()>>& deleterBin) {  }

template<typename U, typename... T>
inline void CollectArgs(LPVOID* stackBin, std::vector<std::function<void()>>& deleterBin, const U& carg, T... rargs) {
  CollectArgs(stackBin + 1, deleterBin, rargs...);
  if constexpr (simple_string<U>) {
    MemPtr obj = CreateJassString(carg);
    *stackBin = obj.pointer;
    deleterBin.push_back([str = obj.handle]() {
      DestroyJassString(str);
    });
  } else if constexpr (std::is_floating_point_v<U>) {
    auto parg = new float{ static_cast<float>(carg) };
    *stackBin = parg;
    deleterBin.push_back([parg]() { delete parg; });
  } else if constexpr (std::is_convertible_v<U, INT32>) {
    *stackBin = reinterpret_cast<LPVOID>(static_cast<INT32>(carg));
  } else if constexpr (std::is_convertible_v<U, UINT32>) {
    *stackBin = reinterpret_cast<LPVOID>(static_cast<UINT32>(carg));
  } else if constexpr (std::is_same_v<U, JassObject>) {
    *stackBin = *reinterpret_cast<const LPVOID*>(&carg);
  } else {
    []() {
      static_assert(false, "Invalid arg type provided for CallFn! Check type U:");
    }();
  }
}

template<typename R, typename... T>
R CallFn(const std::string& func_name, T... args) {
  constexpr int argSize = sizeof...(T);
  LPVOID stackBin[argSize + 1];
  std::vector<std::function<void()>> deleterBin;

  CollectArgs(stackBin, deleterBin, args...);
  // the last arg: func name
  *std::rbegin(stackBin) = const_cast<char*>(func_name.c_str());

  // call
  auto itRes = mapFunc.find(func_name);
  LPVOID fnPtr = nullptr;
  if (itRes != mapFunc.end())
    fnPtr = itRes->second->fnAddr;
  else {
    // wait 50ms and try once again
    Sleep(50);
    RegenerateFunctionMap();
    itRes = mapFunc.find(func_name);
    if (itRes != mapFunc.end())
      fnPtr = itRes->second->fnAddr;
    else {
      assert(false && "Cannot find specified jass function!");
    }
  }

  MemPtr argsPtr = nullptr;
  MemPtr retVal = nullptr;
  constexpr int allocaSize = sizeof stackBin;
  __asm {
    sub esp, allocaSize
    mov argsPtr, esp
  }
  memcpy(argsPtr, stackBin, sizeof stackBin);
  __asm {
    call fnPtr
    mov retVal, eax
    add esp, allocaSize
  }

  // delete
  for (auto&& del : deleterBin) {
    del();
  }

  // deal with return unmarshalling
  if constexpr (!std::is_void_v<R>) {
    if constexpr (simple_string<R>) {
      return UnpackJassString(retVal.handle);
    } else if constexpr (std::is_floating_point_v<R>) {
      return static_cast<R>(MemRead<float>(retVal));
    } else if constexpr (std::is_convertible_v<R, int>) {
      return retVal.address;
    } else if constexpr (std::is_same_v<R, JassObject>) {
      return retVal.handle;
    } else {
      []() {
        static_assert(false, "Invalid return type provided for CallFn! Check type R:");
      }();
    }
  }
}
