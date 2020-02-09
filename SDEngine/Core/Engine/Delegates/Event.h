#include "Core/Engine/Delegates/EventDelegate.h"
#include "Core/DataStructures/Array.h"

template<typename T>
class Event;

#define EVENT(EventName, Signature, TargetType, MethodAddress, Instance) Event<Signature> EventName;

template<typename Ret, typename ...Args>
class Event<Ret(Args...)> {
public:
	Event() {

	}
	~Event() {

	}

	template<Ret(*FunctionPointer)(Args...)>
	bool Add() {
		return Add(EventDelegate<Ret(Args...)>::Create<FunctionPointer>());
	}
	template<Ret(*FunctionPointer)(Args...)>
	bool Remove() {
		return Remove(EventDelegate<Ret(Args...)>::Create<FunctionPointer>());
	}

	template<typename T, Ret(T::* FunctionPointer)(Args...)>
	bool Add(T* Instance) {
		return Add(EventDelegate<Ret(Args...)>::Create<T, FunctionPointer>(Instance));
	}
	template<typename T, Ret(T::* FunctionPointer)(Args...)>
	bool Remove(T* Instance) {
		return Remove(EventDelegate<Ret(Args...)>::Create<T, FunctionPointer>(Instance));
	}

	template<typename T>
	bool Add(T* Functor) {
		return Add(EventDelegate<Ret(Args...)>::Create(Functor));
	}
	template<typename T>
	bool Remove(T* Functor) {
		return Remove(EventDelegate<Ret(Args...)>::Create(Functor));
	}

	/**
	 * Fires the event with the provided arguments.
	 *
	 * @param 	{Args...}	Arguments	Variable arguments providing the arguments.
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