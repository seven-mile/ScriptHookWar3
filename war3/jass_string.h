#pragma once
#include "../pch.h"

template<typename T>
concept simple_string = std::is_convertible_v<T, const char*> || std::is_same_v<T, std::string>;

template<simple_string T>
inline JassString CreateJassString(const T& arg) {
  MemPtr raw_ptr = nullptr;
  if constexpr (std::is_convertible_v<T, const char*>) raw_ptr = const_cast<char*>(static_cast<const char*>(arg));
  else if constexpr (std::is_same_v<T, std::string>) raw_ptr = arg.c_str();

  assert(raw_ptr.address && "Invalid input string to create jass string!");

  MemPtr ptr1 = nullptr, ptr2 = nullptr;
  ptr1 = new size_t{ raw_ptr };
  ptr2 = new size_t{ ptr1.address - 0x1C };
  ptr2.address -= 0x8;

  return ptr2;
}

inline const char* UnpackJassString(const JassString& str) {
  MemPtr p = str, ptr2 = p.address + 0x8;
  assert(ptr2.address && "Invalid address when accessing JassString handle for the second offset.");
  MemPtr ptr1 = MemRead(ptr2) + 0x1C;
  return MemRead<const char*>(ptr1);
}

inline void DestroyJassString(const JassString& str) {
  MemPtr p = str, ptr2 = p.address + 0x8;
  assert(ptr2.address && "Invalid address when accessing JassString handle for the second offset.");
  MemPtr ptr1 = MemRead(ptr2) + 0x1C;
  delete ptr1.pointer;
  delete ptr2.pointer;
}
