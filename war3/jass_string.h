#pragma once
#include "../pch.h"

template<typename T>
concept simple_string = std::is_convertible_v<T, const char*> || std::is_same_v<std::remove_cvref_t<T>, std::string>;

inline std::unordered_map<std::string, HString> g_string_cache;

template<simple_string T>
inline HString CreateJassString(const T& arg) {
  if (g_string_cache.count(arg)) {
    return g_string_cache[arg];
  }

  MemPtr raw_ptr = nullptr;
  if constexpr (std::is_convertible_v<T, const char*>) raw_ptr = const_cast<char*>(static_cast<const char*>(arg));
  else if constexpr (std::is_same_v<std::remove_cvref<T>::type, std::string>) raw_ptr = arg.c_str();
  else {
    []() {
      static_assert(false, "Invalid input string to create jass string! Check type T:");
    }();
  }

  assert(raw_ptr.address && "Invalid input string to create jass string!");

  MemPtr ptr1 = nullptr, ptr2 = nullptr;
  ptr1 = new size_t{ raw_ptr };
  ptr2 = new size_t{ ptr1.address - 0x1C };
  ptr2.address -= 0x8;

  return g_string_cache[arg] = ptr2;
}

inline const char* UnpackJassString(const HString& str) {
  MemPtr p = str, ptr2 = p.address + 0x8;
  assert(ptr2.address && "Invalid address when accessing JassString handle for the second offset.");
  MemPtr ptr1 = MemRead(ptr2) + 0x1C;
  return MemRead<const char*>(ptr1);
}

inline void DestroyJassString(const HString& str) {

  MemPtr p = str, ptr2 = p.address + 0x8;
  assert(ptr2.address && "Invalid address when accessing JassString handle for the second offset.");
  MemPtr ptr1 = MemRead(ptr2) + 0x1C;

  // erase the cache key
  g_string_cache.erase(MemRead<const char*>(ptr1));

  delete ptr1.pointer;
  delete ptr2.pointer;
}
