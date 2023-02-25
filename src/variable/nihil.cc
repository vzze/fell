#include "variable.hh"
#include "util.hh"

fell::types::nihil::nihil(nil none) : variable(none) {}

fell::types::variable::var fell::types::nihil::operator + (const variable *) {
    throw std::runtime_error{"Nil variable can't be added."};
}

fell::types::variable::var fell::types::nihil::operator - (const variable *) {
    throw std::runtime_error{"Nil variable can't be substracted."};
}

fell::types::variable::var fell::types::nihil::operator * (const variable *) {
    throw std::runtime_error{"Nil variable can't be multiplied."};
}

fell::types::variable::var fell::types::nihil::operator / (const variable *) {
    throw std::runtime_error{"Nil variable can't be divided."};
}

fell::types::variable::var fell::types::nihil::operator % (const variable *) {
    throw std::runtime_error{"Nil variable can't have a remainder."};
}

fell::types::variable::var fell::types::nihil::operator > (const variable *) {
    throw std::runtime_error{"Nil variable can't be compared."};
}

fell::types::variable::var fell::types::nihil::operator >= (const variable *) {
    throw std::runtime_error{"Nil variable can't be compared."};
}

fell::types::variable::var fell::types::nihil::operator < (const variable *) {
    throw std::runtime_error{"Nil variable can't be compared."};
}

fell::types::variable::var fell::types::nihil::operator <= (const variable *) {
    throw std::runtime_error{"Nil variable can't be compared."};
}

fell::types::variable::var fell::types::nihil::operator == (const variable *) {
    throw std::runtime_error{"Nil variable can't be compared."};
}

fell::types::variable::var fell::types::nihil::operator != (const variable *) {
    throw std::runtime_error{"Nil variable can't be compared."};
}

fell::types::variable::var fell::types::nihil::operator && (const variable *) {
    return util::make_var<number>(0);
}

fell::types::variable::var fell::types::nihil::operator || (const variable * rhs) {
    try {
        util::get_value<nihil::nil>(rhs);
        return fell::util::make_var<number>(0);
    } catch(...) {
        try {
            return fell::util::make_var<number>(
                static_cast<number::num>(
                    false || (util::get_value<number::num>(rhs) != 0.0)
                )
            );
        } catch(...) {
            return util::make_var<number>(1);
        }
    }
}

fell::types::variable::var & fell::types::nihil::operator [] (const variable *) {
    throw std::runtime_error{"Nil variable has no subscript operator."};
}

fell::types::variable::var & fell::types::nihil::operator [] (const string::str) {
    throw std::runtime_error{"Nil variable has no subscript operator."};
}

fell::types::variable::var fell::types::nihil::call(std::vector<lex::inmemory> &&) {
    throw std::runtime_error{"Nil variable can't be called."};
}
