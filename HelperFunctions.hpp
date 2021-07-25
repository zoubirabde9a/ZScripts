#pragma once

namespace ZScript {
	template < typename T, typename ...Args >
	T* shared_create(Args &&... args) {
		char* data = new char[sizeof(T) + sizeof(size_t)];
		(*(size_t*)(data)) = 1;
		data += sizeof(size_t);
		new (data) T(args...);
		return (T*)data;
	}
	template < typename T >
	void shared_ref(T* ptr) {
		(*shared_getRefPtr(ptr))++;
	}
	template < typename T >
	void shared_release(T* ptr) {
		size_t* refPtr = shared_getRefPtr(ptr);
		(*refPtr)--;
		if (*refPtr == 0) {
			ptr->~T();
			char* data = (char*)ptr;
			data -= sizeof(size_t);
			delete[] data;
		}
	}
	template < typename T >
	size_t* shared_getRefPtr(T* ptr) {
		return ((size_t*)(ptr)) - 1;
	}
}