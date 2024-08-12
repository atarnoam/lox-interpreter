#pragma once
#include <type_traits>

template <typename S> constexpr bool are_all_eq() { return true; }

template <typename S, typename T, typename... Args>
constexpr bool are_all_eq() {
    return std::is_same_v<S, T> && are_all_eq<S, Args...>();
}