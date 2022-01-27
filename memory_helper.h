#pragma once

// Most helper functions from https://github.com/cleolibrary/CLEO4/blob/master/source/Mem.h
// Thanks CLEO4 library repo!

#pragma region jass object type
DECLARE_HANDLE(JassObject);
typedef JassObject 
  JassPlayer, JassUnit, JassString, JassGroup, JassCode,
  JassTrigger, JassTriggerAction, JassEvent, JassDialog,
  JassDialogButton;
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
  inline constexpr MemPtr& operator=(const T* p) { this->pointer = const_cast<T*>(p); return *this; }
};

static_assert(sizeof(MemPtr) == 4);
#pragma endregion

#pragma region x86 op codes
#define X86_OP_NOP			0x90
#define X86_OP_RET			0xC3
#define X86_OP_CALL			0xE8
#define X86_OP_JMP			0xE9
#define X86_OP_JMPSHORT		0xEB
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
  MemWrite<BYTE>(p++, X86_OP_JMP);
  if (r) *r = (T)(MemRead<DWORD>(p) + p + 4);
  MemWrite<DWORD>(p, ((DWORD)v - (DWORD)p) - 4);
}

// Write a call to v to the address at p and copy the replaced call address to r
template<typename T, typename U>
inline void MemCall(U p, const T v, T* r = nullptr)
{
  MemWrite<BYTE>(p++, X86_OP_CALL);
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
