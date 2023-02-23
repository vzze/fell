#include "variable.hh"

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

fell::types::variable::var & fell::types::nihil::operator [] (const variable *) {
    throw std::runtime_error{"Nil variable has no subscript operator."};
}

fell::types::variable::var & fell::types::nihil::operator [] (const string::str) {
    throw std::runtime_error{"Nil variable has no subscript operator."};
}

fell::types::variable::var fell::types::nihil::call(std::vector<variable::var> &&, std::vector<bool> &&) {
    throw std::runtime_error{"Nil variable can't be called."};
}
