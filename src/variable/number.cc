#include "number.hh"

fell::types::number::number(num num) : variable(num) {}

fell::types::variable::var fell::types::number::operator + (const var & rhs) {
    return new number{
        *std::any_cast<num>(&this->value) +
        *std::any_cast<num>(&rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator - (const var & rhs) {
    return new number{
        *std::any_cast<num>(&this->value) -
        *std::any_cast<num>(&rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator * (const var & rhs) {
    return new number{
        *std::any_cast<num>(&this->value) *
        *std::any_cast<num>(&rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator / (const var & rhs) {
    return new number{
        *std::any_cast<num>(&this->value) /
        *std::any_cast<num>(&rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator > (const var & rhs) {
    return new number{
        static_cast<num>(
            *std::any_cast<num>(&this->value) >
            *std::any_cast<num>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator >= (const var & rhs) {
    return new number{
        static_cast<num>(
            *std::any_cast<num>(&this->value) >=
            *std::any_cast<num>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator < (const var & rhs) {
    return new number{
        static_cast<num>(
            *std::any_cast<num>(&this->value) <
            *std::any_cast<num>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator <= (const var & rhs) {
    return new number{
        static_cast<num>(
            *std::any_cast<num>(&this->value) <=
            *std::any_cast<num>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator == (const var & rhs) {
    return new number{
        static_cast<num>(
            *std::any_cast<num>(&this->value) ==
            *std::any_cast<num>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator != (const var & rhs) {
    return new number{
        static_cast<num>(
            *std::any_cast<num>(&this->value) !=
            *std::any_cast<num>(&rhs->value)
        )
    };
}

fell::types::variable::var & fell::types::number::operator [] (const var &) {
    throw std::runtime_error{"No subscript operator on variable of type Number."};
}

fell::types::variable::var & fell::types::number::operator [] (const std::string) {
    throw std::runtime_error{"No subscript operator on variable of type Number."};
}
