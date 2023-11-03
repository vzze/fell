#ifndef FELL_VAR_UNDERLYING_HH
#define FELL_VAR_UNDERLYING_HH

#include <unordered_map>
#include <variant>
#include <utility>
#include <vector>
#include <string>

#include <fell/var/helpers/object.hh>

namespace fell {
    using nil = std::monostate;
    using num = double;
    using str = object<std::string>;

    struct var;

    using arr = object<std::vector<var>>;
    using tbl = object<std::unordered_map<std::variant<num, std::string>, var>>;

    template<typename ... Args> str make_str(Args && ... args);
    template<typename ... Args> arr make_arr(Args && ... args);
    template<typename ... Args> tbl make_tbl(Args && ... args);
}

template<typename ... Args>
fell::str fell::make_str(Args && ... args) {
    return make_object<std::string>(std::forward<Args>(args)...);
}

template<typename ... Args>
fell::arr fell::make_arr(Args && ... args) {
    return make_object<std::vector<var>>(std::forward<Args>(args)...);
}

template<typename ... Args>
fell::tbl fell::make_tbl(Args && ... args) {
    return make_object<std::unordered_map<std::variant<num, std::string>, var>>(std::forward<Args>(args)...);
}

#endif
