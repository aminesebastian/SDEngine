#pragma once
#include "Core/DataStructures/Array.h"

/**
 * A basic templated stack.
 *
 * @author	Amine
 * @date	3/19/2020
 *
 * @tparam	T	Generic type parameter.
 */
template<typename T>
class SStack {
public:
	/** Default constructor */
	SStack() {}
	/** Destructor */
	virtual ~SStack() {}

	/**
	 * Removes and returns the top-of-stack object
	 *
	 * @returns	The previous top-of-stack object.
	 */
	T& Pop() {
		T& temp = Data[Data.LastIndex()];
		Data.RemoveAt(Data.LastIndex());
		return temp;
	}

	/**
	 * Pushes an object onto this stack
	 *
	 * @param 	Element	The element to push.
	 */
	void Push(const T& Element) {
		Data.Add(Element);
	}

	/**
	 * Returns the top-of-stack object without removing it
	 *
	 * @returns	The current top-of-stack object.
	 */
	const T& Peek() const {
		return Data[Data.LastIndex()];
	}

	/**
	 * Pre allocates memory capacity for the provided number of elements.
	 *
	 * @param 	Size	The expected number of elements to preallocate for.
	 */
	void PreAllocate(const int64& Size) {
		Data.PreAllocate(Size);
	}

	/** Clears this object to its blank/initial state */
	void Clear() {
		Data.Clear();
	}

	/** Reverses the order of elements in the stack. */
	void Reverse() {
		std::reverse(Data.begin(), Data.end());
	}

	/**
	 * Gets the count of elements in the stack.
	 *
	 * @returns	A const int32.
	 */
	const int32 Count() {
		return Data.Count();
	}

	/**
	 * Checks if the stack is empty.
	 *
	 * @returns	A const bool.
	 */
	const bool IsEmpty() const {
		return Data.IsEmpty();
	}
private:
	SArray<T> Data;
};

