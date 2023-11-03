#ifndef FELL_VAR_HELPERS_CONCEPTS_HH
#define FELL_VAR_HELPERS_CONCEPTS_HH

#include <fell/var/underlying/underlying.hh>

#include <concepts>
#include <type_traits>

namespace fell::concepts {
    template<typename T>
    concept variable = std::is_same_v<std::decay_t<T>, var>;

template<typename T>
    concept underlying =
        std::is_same_v<T, nil> ||
        std::is_same_v<T, num> ||
        std::is_same_v<T, str> ||
        std::is_same_v<T, arr> ||
        std::is_same_v<T, tbl>;

    template<typename T> concept held = underlying<std::decay_t<T>>;

    template<typename T> concept nihil  = std::is_same_v<std::decay_t<T>, nil>;
    template<typename T> concept number = std::is_same_v<std::decay_t<T>, num>;
    template<typename T> concept string = std::is_same_v<std::decay_t<T>, str>;
    template<typename T> concept array  = std::is_same_v<std::decay_t<T>, arr>;
    template<typename T> concept table  = std::is_same_v<std::decay_t<T>, tbl>;

    template<typename F>
    concept callable =
        std::invocable<F, nil> ||
        std::invocable<F, num> ||
        std::invocable<F, str> ||
        std::invocable<F, arr> ||
        std::invocable<F, tbl>;

    template<typename ... Ts>
    struct visitor : Ts... { using Ts::operator()...; };
}

#endif
