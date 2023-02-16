#include "string.hh"

fell::types::string::string(str str) : variable(str) {}

fell::types::variable::var fell::types::string::operator + (const var & rhs) {
    return new string{
        *std::any_cast<str>(&this->value) +
        *std::any_cast<str>(&rhs->value)
    };
}

fell::types::variable::var fell::types::string::operator - (const var &) {
    throw std::runtime_error{"Variable of type String can't be substracted."};
}

fell::types::variable::var fell::types::string::operator * (const var &) {
    throw std::runtime_error{"Variable of type String can't be multiplied."};
}

fell::types::variable::var fell::types::string::operator / (const var &) {
    throw std::runtime_error{"Variable of type String can't be divided."};
}

fell::types::variable::var fell::types::string::operator > (const var & rhs) {
    return new number{
        static_cast<number::num>(
            *std::any_cast<str>(&this->value) >
            *std::any_cast<str>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator >= (const var & rhs) {
    return new number{
        static_cast<number::num>(
            *std::any_cast<str>(&this->value) >=
            *std::any_cast<str>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator < (const var & rhs) {
    return new number{
        static_cast<number::num>(
            *std::any_cast<str>(&this->value) <
            *std::any_cast<str>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator <= (const var & rhs) {
    return new number{
        static_cast<number::num>(
            *std::any_cast<str>(&this->value) <=
            *std::any_cast<str>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator == (const var & rhs) {
    return new number{
        static_cast<number::num>(
            *std::any_cast<str>(&this->value) ==
            *std::any_cast<str>(&rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator != (const var & rhs) {
    return new number{
        static_cast<number::num>(
            *std::any_cast<str>(&this->value) !=
            *std::any_cast<str>(&rhs->value)
        )
    };
}

fell::types::variable::var & fell::types::string::operator [] (const var &) {
    throw std::runtime_error{"No subscript operator on variable of type String."};
}

fell::types::variable::var & fell::types::string::operator [] (const str) {
    throw std::runtime_error{"No subscript operator on variable of type String."};
}
