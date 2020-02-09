#pragma once
#include <functional>
#include <memory>
#include "Core/Utilities/Logger.h"

template<typename T>
class EventDelegate;

template<typename ReturnType, typename ...Args>
class EventDelegate<ReturnType(Args...)> {
	using FunctionPointerType = ReturnType(*)(void*, Args...);

public:
	~EventDelegate() {
		Invalidate();
	}

	/**
	 * Creates a new EventDelegate for a global/static function.
	 *
	 * @tparam	ReturnType(*FunctionPointer)(Args...)	Type of the ret(*func ptr)( args...)
	 *
	 * @returns	{EventDelegate}	An EventDelegate.
	 */
	template<ReturnType(*FunctionPointer)(Args...)>
	static EventDelegate Create() {
		return EventDelegate{ nullptr, &globalCaller<FunctionPointer> };
	}

	/**
	 * Creates a new EventDelegate for a class member method.
	 *
	 * @tparam	T						 	The class type.
	 * @tparam	FunctionPointer)(Args...)	The return type of the function &amp; the parameters
	 * 										involved.
	 * @param [in,out]	{T*}	ObjectInstance	obj	The instance of the class object.
	 *
	 * @returns	{EventDelegate}	An EventDelegate.
	 */
	template<typename T, ReturnType(T::* FunctionPointer)(Args...)>
	static EventDelegate Create(T* ObjectInstance) {
		return EventDelegate{ObjectInstance, &MemberCaller<T, FunctionPointer> };
	}

	/**
	 * Creates a new EventDelegate for a functor object.
	 *
	 * @tparam	T	The class type.
	 * @param [in,out]	{T*}	Functor	Functor	A pointer to the functor.
	 *
	 * @returns	{EventDelegate}	An EventDelegate.
	 */
	template<typename T>
	static EventDelegate Create(T* Functor) {
		return EventDelegate{Functor, &FunctorCaller<T> };
	}

	/**
	 * Fires the callback
	 *
	 * @param 	{Args...}	Arguments	args	Variable arguments providing the arguments.
	 *
	 * @returns	{ReturnType}	The return value of the callback.
	 */
	ReturnType operator()(Args... Arguments) {
		if (bIsValid) {
			return FunctionPointer(TargetInstance, Arguments...);
		} else {
			SD_ENGINE_WARN("There was an attempt to call an invalid delegate.");
			throw;
		}
	}

	/** Invalidates this delegate. No further calls will be accepted. */
	void Invalidate() {
		bIsValid = false;
	}

	/**
	 * Checks to see if this delegate is valid.
	 *
	 * @returns	{const bool&}	True if the delegate is valid, false otherwise.
	 */
	const bool& IsValid() {
		return bIsValid;
	}
	bool operator==(const EventDelegate& OtherDelegate) {
		return TargetInstance == OtherDelegate.TargetInstance && FunctionPointer == OtherDelegate.FunctionPointer;
	}
private:
	EventDelegate(void* TargetInstance, FunctionPointerType FunctionPointer) : TargetInstance(TargetInstance), FunctionPointer(FunctionPointer), bIsValid(true) {}

	template<ReturnType(*FunctionPointer)(Args...)>
	static ReturnType GlobalCaller(void*, Args... Arguments) {
		return FunctionPointer(Arguments...);
	}
	template<typename T, ReturnType(T::* FunctionPointer)(Args...)>
	static ReturnType MemberCaller(void* TargetInstance, Args... Arguments) {
		return (static_cast<T*>(TargetInstance)->*FunctionPointer)(Arguments...);
	}
	template<typename T>
	static ReturnType FunctorCaller(void* Functor, Args... Arguments) {
		return (*static_cast<T*>(Functor))(Arguments...);
	}

	void* TargetInstance{ nullptr };
	FunctionPointerType FunctionPointer{ nullptr };
	bool bIsValid;
};

