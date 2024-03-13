#pragma once

#include <variant>

template<typename T, class... Types>
inline bool operator==(const T& t, const std::variant<Types...>& v) {
	const T* c = std::get_if<T>(&v);

	return c && *c == t;
}

template<typename T, class... Types>
inline bool operator==(const std::variant<Types...>& v, const T& t) {
	return t == v;
}