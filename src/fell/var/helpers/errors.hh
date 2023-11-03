#ifndef VAR_HELPERS_ERRORS_HH
#define VAR_HELPERS_ERRORS_HH

#include <stdexcept>
#include <format>

#include <fell/var/helpers/concepts.hh>

namespace fell::err::var {
    template<concepts::underlying U>
    struct type_name;

    template<> struct type_name<fell::nil> { static consteval decltype(auto) name() { return "Nil"; } };
    template<> struct type_name<fell::num> { static consteval decltype(auto) name() { return "Num"; } };
    template<> struct type_name<fell::str> { static consteval decltype(auto) name() { return "Str"; } };
    template<> struct type_name<fell::arr> { static consteval decltype(auto) name() { return "Arr"; } };
    template<> struct type_name<fell::tbl> { static consteval decltype(auto) name() { return "Tbl"; } };

    template<concepts::held H>
    struct type : type_name<std::decay_t<H>> {};

    template<concepts::held H>
    auto has_no_operator(const auto op_type) noexcept;

    template<concepts::held H>
    auto cannot_be_used_as_key() noexcept;

    template<concepts::held H>
    auto cannot_be_used_as_index() noexcept;

    template<concepts::held H>
    auto not_comparable() noexcept;

    template<concepts::held H, concepts::held With>
    auto not_comparable_with() noexcept;
}

template<fell::concepts::held H>
auto fell::err::var::has_no_operator(const auto op_type) noexcept {
    return std::logic_error(
        std::format(
            "Variable of type {} has no {} operator.",
            type<H>::name(), op_type
        )
    );
}

template<fell::concepts::held H>
auto fell::err::var::cannot_be_used_as_key() noexcept {
    return std::logic_error(
        std::format(
            "Variable of type {} cannot be used as a key.",
            type<H>::name()
        )
    );
}

template<fell::concepts::held H>
auto fell::err::var::cannot_be_used_as_index() noexcept {
    return std::logic_error(
        std::format(
            "Variable of type {} cannot be used as an index.",
            type<H>::name()
        )
    );
}

template<fell::concepts::held H>
auto fell::err::var::not_comparable() noexcept {
    return std::logic_error(
        std::format(
            "Variable of type {} is not comparable.",
            type<H>::name()
        )
    );
}

template<fell::concepts::held H, fell::concepts::held With>
auto fell::err::var::not_comparable_with() noexcept {
    return std::logic_error(
        std::format(
            "Variable of type {} is not comparable with {}.",
            type<H>::name(), type<With>::name()
        )
    );
}

#endif
