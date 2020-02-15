#pragma once

#include "Core/Engine/Delegates/EventDelegate.h"
#include "Core/DataStructures/Array.h"

template<typename T>
class Event;

/**
 * A macro that defines event.
 *
 * @param 	{void}	EventName	Name of the event.
 * @param 	{void}	Signature	The signature.
 */
#define EVENT(EventName, Signature) Event<Signature> EventName;

/**
 * Creates a usable lambda event listener.
 *
 * @tparam	T	Generic type parameter.
 * @tparam	L	Type of the l.
 * @param [in,out]	{T &&}	t	A T to process.
 *
 * @returns	{L}	A L.
 */
template <typename T, typename L = typename std::decay<T>::type>
L lambda_delegate(T && t) {
	return L(forward<T>(t));
}

/**
 * A listenable event. Objects can register listeners with this event to receive updates
 * whenever this event is fired. Handles multiple listeners.
 *
 * @author	Amine
 * @date	2/9/2020
 *
 * @tparam	Ret	   	The return type of the event.
 * @tparam	...Args	The arguments provided by the event.
 */
template<typename Ret, typename ...Args>
class Event<Ret(Args...)> {
public:
	Event() {}
	~Event() {}

	/**
	 * Adds a global/static event listener.
	 *
	 * @tparam	Ret(*FunctionPointer)(Args...)	Type of the ret(* function pointer)( args...)
	 *
	 * @returns	{bool}	True if it succeeds, false if it fails.
	 */
	template<Ret(*FunctionPointer)(Args...)>
	bool Add() const {
		return const_cast<Event*>(this)->Add(EventDelegate<Ret(Args...)>::Create<FunctionPointer>());
	}

	/**
	 * Removes the provided global/static function as an event listener.
	 *
	 * @tparam	Ret(*FunctionPointer)(Args...)	Type of the ret(* function pointer)( args...)
	 *
	 * @returns	{bool}	True if it succeeds, false if it fails.
	 */
	template<Ret(*FunctionPointer)(Args...)>
	bool Remove() const {
		return const_cast<Event*>(this)->Remove(EventDelegate<Ret(Args...)>::Create<FunctionPointer>());
	}

	/**
	 * Adds a method/class member as an event listener.
	 *
	 * @tparam	T						 	The type of the class that is listening.
	 * @tparam	FunctionPointer)(Args...)	Pointer to the method that should get fired when this
	 * 										event is raised.
	 * @param [in,out]	{T*}	Instance	T*}	Instance	The instance of the class that should
	 * 										receive the event.
	 *
	 * @returns	{bool}	True if it succeeds, false if it fails.
	 */
	template<typename T, Ret(T::* FunctionPointer)(Args...)>
	bool Add(T* Instance) const {
		return const_cast<Event*>(this)->Add(EventDelegate<Ret(Args...)>::Create<T, FunctionPointer>(Instance));
	}

	/**
	* Removes a method/class member as an event listener.
	*
	* @tparam	T						 	The type of the class that is listening.
	* @tparam	FunctionPointer)(Args...)	Pointer to the method that should get fired when this
	* 										event is raised.
	* @param [in,out]	{T*}	Instance	T*}	Instance	The instance of the class that should
	* 										receive the event.
	*
	* @returns	{bool}	True if it succeeds, false if it fails.
	*/
	template<typename T, Ret(T::* FunctionPointer)(Args...)>
	bool Remove(T* Instance) const {
		return const_cast<Event*>(this)->Remove(EventDelegate<Ret(Args...)>::Create<T, FunctionPointer>(Instance));
	}

	/**
	 * Adds a lambda as an event listener.
	 *
	 * @tparam	T	Generic type parameter.
	 * @param [in,out]	{T*}	Functor	If non-null, the functor.
	 *
	 * @returns	{bool}	True if it succeeds, false if it fails.
	 */
	template<typename T>
	bool Add(T* Functor) const {
		return const_cast<Event*>(this)->Add(EventDelegate<Ret(Args...)>::Create(Functor));
	}

	/**
	 * Removes a lambda as an event listener.
	 *
	 * @tparam	T	Generic type parameter.
	 * @param [in,out]	{T*}	Functor	If non-null, the functor.
	 *
	 * @returns	{bool}	True if it succeeds, false if it fails.
	 */
	template<typename T>
	bool Remove(T* Functor) const {
		return const_cast<Event*>(this)->Remove(EventDelegate<Ret(Args...)>::Create(Functor));
	}

	/**
	 * Fires the event with the provided arguments.
	 *
	 * @param 	{Args...}	Arguments	Arguments	Variable arguments providing the arguments.
	 */
	void Broadcast(Args... Arguments) {
		for (auto& delegate : Delegates) {
			delegate(Arguments...);
		};
	}
private:
	SArray<EventDelegate<Ret(Args...)>> Delegates;

	template<typename Ret, typename ...Args>
	bool Add(EventDelegate<Ret(Args...)> Delegate) {
		if (find(Delegates.begin(), Delegates.end(), Delegate) != Delegates.end()) {
			return false;
		}

		// This is safer than using AddUnqiue.
		Delegates.Add(Delegate);

		return true;
	}
	template<typename Ret, typename ...Args>
	bool Remove(EventDelegate<Ret(Args...)> Delegate) {
		auto it = find(Delegates.begin(), Delegates.end(), Delegate);

		if (it == Delegates.end()) {
			return false;
		}

		Delegates.erase(it);

		return true;
	}
};