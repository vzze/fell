#include "number.hh"

fell::types::number::number(long double num) : variable(num) {}

fell::types::variable::var fell::types::number::operator + (const var & rhs) {
    return new number{
        std::any_cast<long double>(this->value) +
        std::any_cast<long double>(rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator - (const var & rhs) {
    return new number{
        std::any_cast<long double>(this->value) -
        std::any_cast<long double>(rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator * (const var & rhs) {
    return new number{
        std::any_cast<long double>(this->value) *
        std::any_cast<long double>(rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator / (const var & rhs) {
    return new number{
        std::any_cast<long double>(this->value) /
        std::any_cast<long double>(rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator > (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<long double>(this->value) >
            std::any_cast<long double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator >= (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<long double>(this->value) >=
            std::any_cast<long double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator < (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<long double>(this->value) <
            std::any_cast<long double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator <= (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<long double>(this->value) <=
            std::any_cast<long double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator == (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<long double>(this->value) ==
            std::any_cast<long double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator != (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<long double>(this->value) !=
            std::any_cast<long double>(rhs->value)
        )
    };
}

fell::types::variable::var & fell::types::number::operator [] (const var & offset) {
    throw "No subscript operator on variable of type Number.";
}
