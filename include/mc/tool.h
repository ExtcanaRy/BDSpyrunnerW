#pragma once
#pragma execution_character_set("utf-8")
#pragma warning(disable:4996)
#pragma comment(lib, "littlehooker.lib")
#include <iostream>
#include <littlehooker/littlehooker.h>
#include "Level.h"
#include "NetWork.h"

//#define Dereference<type>( ptr) (*reinterpret_cast<type*>(ptr))
template <typename T>
inline T& Dereference(void* ptr, uintptr_t offset) {
	return *reinterpret_cast<T*>(uintptr_t(ptr) + offset);
}
template <typename T>
inline T& Dereference(uintptr_t ptr, uintptr_t offset) {
	return *reinterpret_cast<T*>(uintptr_t(ptr) + offset);
}
template <typename T>
inline T& Dereference(void* ptr) {
	return *reinterpret_cast<T*>(ptr);
}
template <typename T>
inline T& Dereference(uintptr_t ptr) {
	return *reinterpret_cast<T*>(ptr);
}
#define SYM dlsym

// call a virtual function
// _this: this ptr, off: offsetof function
template<typename ReturnType = void, typename... Args>
inline ReturnType VirtualCall(uintptr_t off, void* _this, Args... args) {
	return (*reinterpret_cast<ReturnType(**)(void*, Args...)>(Dereference<uintptr_t>( _this) + off))(_this, args...);
}
// call a function by symbol string
template<typename ReturnType = void, typename... Args>
inline ReturnType SymCall(const char* sym, Args... args) {
	static auto func = SYM(sym);
	if (func == nullptr) {
		std::cerr << "SymbolNotFound: " << sym << std::endl;
	}
	return reinterpret_cast<ReturnType(*)(Args...)>(func)(std::forward<Args>(args)...);
}

template <typename T>
inline T* global = nullptr;

