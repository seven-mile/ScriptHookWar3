#pragma once

// Most helper functions from https://github.com/cleolibrary/CLEO4/blob/master/source/Mem.h
// Thanks CLEO4 library repo!

#pragma region jass object type
DECLARE_HANDLE(JassObject);
typedef JassObject JassPlayer, JassUnit, JassString, JassGroup;
static_assert(sizeof(JassObject) == 4);
#pragma endregion


#pragma region MemPtr union
// a pointer automatically convertible to integral types
union MemPtr
{
  size_t address;
  void* pointer;
  JassObject handle;

  inline constexpr MemPtr(std::nullptr_t) : pointer(nullptr) { }
  inline constexpr MemPtr(void* p) : pointer(p) { }
  inline constexpr MemPtr(size_t a) : address(a) { }
  inline constexpr MemPtr(const JassObject &o) : handle(o) { }
  inline constexpr operator void* () { return pointer; }
  inline constexpr operator size_t() { return address; }
  inline constexpr operator JassObject() { return handle; }
  //inline constexpr MemPtr& operator=(void* p) { return *this = p; }
  //inline constexpr MemPtr& operator=(size_t p) { return *this = p; }
  //inline constexpr MemPtr& operator=(const JassObject &p) { return *this = p; }

  // conversion to/from any-type pointer
  template<typename T>
  inline constexpr MemPtr(T* p) : pointer(p) {}
  template<typename T>
  inline constexpr operator T* () { return reinterpret_cast<T*>(pointer); }
  template<typename T>
  inline constexpr MemPtr& operator=(T* p) { this->pointer = p; return *this; }
};

static_assert(sizeof(MemPtr) == 4);
#pragma endregion

#pragma region x86 op codes
#define OP_NOP			0x90
#define OP_RET			0xC3
#define OP_CALL			0xE8
#define OP_JMP			0xE9
#define OP_JMPSHORT		0xEB
#pragma endregion


#pragma region memory helper functions
template<typename T, typename U>
inline void MemWrite(U p, const T v) { *(T*)p = v; }
template<typename T, typename U>
inline void MemWrite(U p, const T v, int n) { memcpy((void*)p, &v, n); }
template<typename T = size_t, typename U>
inline T MemRead(U p) { return *(T*)p; }
template<typename T, typename U>
inline void MemFill(U p, T v, int n) { memset((void*)p, v, n); }
template<typename T, typename U>
inline void MemCopy(U p, const T v) { memcpy((void*)p, &v, sizeof(T)); }
template<typename T, typename U>
inline void MemCopy(U p, const T v, int n) { memcpy((void*)p, &v, n); }
template<typename T, typename U>
inline void MemCopy(U p, const T* v) { memcpy((void*)p, v, sizeof(T)); }
template<typename T, typename U>
inline void MemCopy(U p, const T* v, int n) { memcpy((void*)p, v, n); }

// Write a jump to v to the address at p and copy the replaced call address to r
template<typename T, typename U>
inline void MemJump(U p, const T v, T* r = nullptr)
{
  MemWrite<BYTE>(p++, OP_JMP);
  if (r) *r = (T)(MemRead<DWORD>(p) + p + 4);
  MemWrite<DWORD>(p, ((DWORD)v - (DWORD)p) - 4);
}

// Write a call to v to the address at p and copy the replaced call address to r
template<typename T, typename U>
inline void MemCall(U p, const T v, T* r = nullptr)
{
  MemWrite<BYTE>(p++, OP_CALL);
  if (r) *r = (T)(MemRead<DWORD>(p) + p + 4);
  MemWrite<DWORD>(p, (DWORD)v - (DWORD)p - 4);
}

// Read and convert a relative offset to full
template<typename T, typename U>
inline T MemReadOffsetPtr(U p)
{
  return (T)((size_t)MemRead<T>(p) + p + sizeof(T));
}
#pragma endregion

#pragma region jass string stuff
template<typename T>
concept simple_string = std::is_convertible_v<T, const char*> || std::is_same_v<T, std::string>;

template<simple_string T>
inline JassString CreateJassString(const T& arg) {
  MemPtr raw_ptr = nullptr;
  if constexpr (std::is_convertible_v<T, const char*>) raw_ptr = const_cast<char*>(static_cast<const char*>(arg));
  else if constexpr (std::is_same_v<T, std::string>) raw_ptr = arg.c_str();

  assert(raw_ptr.address && "Invalid input string to create jass string!");

  MemPtr ptr1 = nullptr, ptr2 = nullptr;
  ptr1 = new size_t { raw_ptr };
  ptr2 = new size_t { ptr1.address - 0x1C };
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
#pragma endregion

