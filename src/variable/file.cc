#include "variable.hh"
#include "util.hh"

fell::types::file::file(fl file) : variable(file) {}

fell::types::variable::var fell::types::file::operator + (const variable *) {
    throw std::runtime_error{"File can't be added."};
}

fell::types::variable::var fell::types::file::operator - (const variable *) {
    throw std::runtime_error{"File can't be substracted."};
}

fell::types::variable::var fell::types::file::operator * (const variable *) {
    throw std::runtime_error{"File can't be multiplied."};
}

fell::types::variable::var fell::types::file::operator / (const variable *) {
    throw std::runtime_error{"File can't be divided."};
}

fell::types::variable::var fell::types::file::operator % (const variable *) {
    throw std::runtime_error{"File can't have a remainder."};
}

fell::types::variable::var fell::types::file::operator > (const variable *) {
    throw std::runtime_error{"File can't be compared."};
}

fell::types::variable::var fell::types::file::operator >= (const variable *) {
    throw std::runtime_error{"File can't be compared."};
}

fell::types::variable::var fell::types::file::operator < (const variable *) {
    throw std::runtime_error{"File can't be compared."};
}

fell::types::variable::var fell::types::file::operator <= (const variable *) {
    throw std::runtime_error{"File can't be compared."};
}

fell::types::variable::var fell::types::file::operator == (const variable *) {
    throw std::runtime_error{"File can't be compared."};
}

fell::types::variable::var fell::types::file::operator != (const variable *) {
    throw std::runtime_error{"File can't be compared."};
}

fell::types::variable::var fell::types::file::operator && (const variable *) {
    throw std::runtime_error{"File can't be converted to boolean."};
}

fell::types::variable::var fell::types::file::operator || (const variable *) {
    throw std::runtime_error{"File can't be converted to boolean."};
}

fell::types::variable::var & fell::types::file::operator [] (const variable *) {
    throw std::runtime_error{"File has no subscript operator."};
}

fell::types::variable::var & fell::types::file::operator [] (const string::str) {
    throw std::runtime_error{"File has no subscript operator."};
}

fell::types::variable::var & fell::types::file::operator [] (const std::size_t) {
    throw std::runtime_error{"File has no subscript operator."};
}

fell::types::variable::var fell::types::file::call(std::vector<lex::inmemory> &&, bool) {
    throw std::runtime_error{"File can't be called."};
}
