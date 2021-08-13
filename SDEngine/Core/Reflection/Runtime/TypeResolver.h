#pragma once
#include <type_traits>
#include "Core/Reflection/Types/ReflectableTypes.h"

template <typename T>
struct TypeResolver {
	static ITypeDescriptor* Get() {
		return DefaultResolver::Get<T>();
	}
};

struct DefaultResolver {
	template <typename T> static char func(decltype(&T::Reflection));
	template <typename T> static int func(...);
	template <typename T>
	struct IsReflected {
		enum { value = (sizeof(func<T>(nullptr)) == sizeof(char)) };
	};

	// This version is called if T has a static member named "Reflection":
	template <typename T, typename std::enable_if<IsReflected<T>::value, int>::type = 0>
	static ITypeDescriptor * Get() {
		return &T::Reflection;
	}

	// This version is called otherwise:
	template <typename T, typename std::enable_if<!IsReflected<T>::value, int>::type = 0>
	static ITypeDescriptor * Get() {
		return GetPrimitiveDescriptor<T>();
	}
};