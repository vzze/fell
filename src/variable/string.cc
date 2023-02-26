#include "variable.hh"
#include "util.hh"

fell::types::string::string(str str) : variable(str) {}

fell::types::variable::var fell::types::string::operator + (const variable * rhs) {
    return util::make_var<string>(
        util::get_value<str>(this) +
        util::get_value<str>(rhs)
    );
}

fell::types::variable::var fell::types::string::operator - (const variable *) {
    throw std::runtime_error{"Variable of type String can't be substracted."};
}

fell::types::variable::var fell::types::string::operator * (const variable *) {
    throw std::runtime_error{"Variable of type String can't be multiplied."};
}

fell::types::variable::var fell::types::string::operator / (const variable *) {
    throw std::runtime_error{"Variable of type String can't be divided."};
}

fell::types::variable::var fell::types::string::operator % (const variable *) {
    throw std::runtime_error{"Variable of type String can't have a remainder."};
}

fell::types::variable::var fell::types::string::operator > (const variable * rhs) {
    return util::make_var<number>(
        static_cast<number::num>(
            util::get_value<str>(this) >
            util::get_value<str>(rhs)
        )
    );
}

fell::types::variable::var fell::types::string::operator >= (const variable * rhs) {
    return util::make_var<number>(
        static_cast<number::num>(
            util::get_value<str>(this) >=
            util::get_value<str>(rhs)
        )
    );
}

fell::types::variable::var fell::types::string::operator < (const variable * rhs) {
    return util::make_var<number>(
        static_cast<number::num>(
            util::get_value<str>(this) <
            util::get_value<str>(rhs)
        )
    );
}

fell::types::variable::var fell::types::string::operator <= (const variable * rhs) {
    return util::make_var<number>(
        static_cast<number::num>(
            util::get_value<str>(this) <=
            util::get_value<str>(rhs)
        )
    );
}

fell::types::variable::var fell::types::string::operator == (const variable * rhs) {
    return util::make_var<number>(
        static_cast<number::num>(
            util::get_value<str>(this) ==
            util::get_value<str>(rhs)
        )
    );
}

fell::types::variable::var fell::types::string::operator != (const variable * rhs) {
    return util::make_var<number>(
        static_cast<number::num>(
            util::get_value<str>(this) !=
            util::get_value<str>(rhs)
        )
    );
}

fell::types::variable::var fell::types::string::operator && (const variable * rhs) {
    try {
        util::get_value<nihil::nil>(rhs);
        return fell::util::make_var<number>(0);
    } catch(...) {
        try {
            return fell::util::make_var<number>(
                static_cast<number::num>(
                    true && (util::get_value<number::num>(rhs) != 0.0)
                )
            );
        } catch(...) {
            return fell::util::make_var<number>(1);
        }
    }
}

fell::types::variable::var fell::types::string::operator || (const variable *) {
    return fell::util::make_var<number>(1);
}

fell::types::variable::var & fell::types::string::operator [] (const variable *) {
    throw std::runtime_error{"No subscript operator on variable of type String."};
}

fell::types::variable::var & fell::types::string::operator [] (const str) {
    throw std::runtime_error{"No subscript operator on variable of type String."};
}

fell::types::variable::var & fell::types::string::operator [] (const std::size_t) {
    throw std::runtime_error{"No subscript operator on variable of type String."};
}

fell::types::variable::var fell::types::string::call(std::vector<lex::inmemory> &&) {
    throw std::runtime_error{"String variable can't be called."};
}
