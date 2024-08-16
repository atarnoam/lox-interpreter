#pragma once
#include <concepts>
#include <type_traits>

template <typename TypeToCheck, typename TypeToCheckAgainst>
concept type_is =
    std::same_as<std::remove_cvref_t<TypeToCheck>, TypeToCheckAgainst>;

template <typename S, typename... Args>
concept are_all_eq = (type_is<S, Args> && ...);
