#include "variable.hh"

fell::types::variable::variable(std::any var) : value(var) {}

fell::types::variable::~variable() {}

void fell::types::override(variable::var & a, variable::var b) {
    delete a;
    constexpr auto size = sizeof(variable);
    a = reinterpret_cast<variable*>(new std::byte[size]);
    std::memcpy(static_cast<void*>(a), static_cast<void*>(b), size);
}
