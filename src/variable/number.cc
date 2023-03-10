#include "variable.hh"
#include "util.hh"


fell::types::number::number(num num) : variable(num) {}

fell::types::variable::var fell::types::number::operator + (const variable * rhs) {
    return fell::util::make_var<number>(
        util::get_value<num>(this) +
        util::get_value<num>(rhs)
    );
}

fell::types::variable::var fell::types::number::operator - (const variable * rhs) {
    return fell::util::make_var<number>(
        util::get_value<num>(this) -
        util::get_value<num>(rhs)
    );
}

fell::types::variable::var fell::types::number::operator * (const variable * rhs) {
    return fell::util::make_var<number>(
        util::get_value<num>(this) *
        util::get_value<num>(rhs)
    );
}

fell::types::variable::var fell::types::number::operator / (const variable * rhs) {
    return fell::util::make_var<number>(
        util::get_value<num>(this) /
        util::get_value<num>(rhs)
    );
}

fell::types::variable::var fell::types::number::operator % (const variable * rhs) {
    const auto & a = util::get_value<num>(this);
    const auto & b = util::get_value<num>(rhs);
    return fell::util::make_var<number>(
        a - std::floor(a / b) * b
    );
}

fell::types::variable::var fell::types::number::operator > (const variable * rhs) {
    return fell::util::make_var<number>(
        static_cast<num>(
            util::get_value<num>(this) >
            util::get_value<num>(rhs)
        )
    );
}

fell::types::variable::var fell::types::number::operator >= (const variable * rhs) {
    return fell::util::make_var<number>(
        static_cast<num>(
            util::get_value<num>(this) >=
            util::get_value<num>(rhs)
        )
    );
}

fell::types::variable::var fell::types::number::operator < (const variable * rhs) {
    return fell::util::make_var<number>(
        static_cast<num>(
            util::get_value<num>(this) <
            util::get_value<num>(rhs)
        )
    );
}

fell::types::variable::var fell::types::number::operator <= (const variable * rhs) {
    return fell::util::make_var<number>(
        static_cast<num>(
            util::get_value<num>(this) <=
            util::get_value<num>(rhs)
        )
    );
}

fell::types::variable::var fell::types::number::operator == (const variable * rhs) {
    return fell::util::make_var<number>(
        static_cast<num>(
            util::get_value<num>(this) ==
            util::get_value<num>(rhs)
        )
    );
}

fell::types::variable::var fell::types::number::operator != (const variable * rhs) {
    return fell::util::make_var<number>(
        static_cast<num>(
            util::get_value<num>(this) !=
            util::get_value<num>(rhs)
        )
    );
}

fell::types::variable::var fell::types::number::operator && (const variable * rhs) {
    try {
        util::get_value<nihil::nil>(rhs);
        return fell::util::make_var<number>(0);
    } catch(...) {
        try {
            return fell::util::make_var<number>(
                static_cast<num>(
                    (util::get_value<num>(this) != 0.0) &&
                    (util::get_value<num>(rhs) != 0.0)
                )
            );
        } catch(...) {
            return fell::util::make_var<number>(
                static_cast<num>(
                    (util::get_value<num>(this) != 0.0) && true
                )
            );
        }
    }
}

fell::types::variable::var fell::types::number::operator || (const variable * rhs) {
    try {
        util::get_value<nihil::nil>(rhs);
        return fell::util::make_var<number>(
            static_cast<num>(
                (util::get_value<num>(this) != 0.0) || false
            )
        );
    } catch(...) {
        try {
            return fell::util::make_var<number>(
                static_cast<num>(
                    (util::get_value<num>(this) != 0.0) ||
                    (util::get_value<num>(rhs) != 0.0)
                )
            );
        } catch(...) {
            return fell::util::make_var<number>(
                static_cast<num>(
                    (util::get_value<num>(this) != 0.0) || true
                )
            );
        }
    }
}

fell::types::variable::var & fell::types::number::operator [] (const variable *) {
    throw std::runtime_error{"No subscript operator on variable of type Number."};
}

fell::types::variable::var & fell::types::number::operator [] (const std::string) {
    throw std::runtime_error{"No subscript operator on variable of type Number."};
}

fell::types::variable::var & fell::types::number::operator [] (const std::size_t) {
    throw std::runtime_error{"No subscript operator on variable of type Number."};
}

fell::types::variable::var fell::types::number::call(std::vector<lex::inmemory> &&, bool) {
    throw std::runtime_error{"Number variable can't be called."};
}
