#pragma once

#pragma once

template <typename To, typename From>
To* Cast(From* p) {
	return dynamic_cast<To*>(p);
}
template <typename To>
To* RemoveConst(const To* p) {
	return const_cast<To*>(p);
}
template <typename To, typename From>
bool IsA(From* p) {
	return Cast<To>(p) != nullptr;
}
class EngineFunctionLibrary {


};

