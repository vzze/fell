#include "number.hh"

fell::types::number::number(double num) : variable(num) {}

fell::types::variable::var fell::types::number::operator + (const var & rhs) {
    return new number{
        std::any_cast<double>(this->value) +
        std::any_cast<double>(rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator - (const var & rhs) {
    return new number{
        std::any_cast<double>(this->value) -
        std::any_cast<double>(rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator * (const var & rhs) {
    return new number{
        std::any_cast<double>(this->value) *
        std::any_cast<double>(rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator / (const var & rhs) {
    return new number{
        std::any_cast<double>(this->value) /
        std::any_cast<double>(rhs->value)
    };
}

fell::types::variable::var fell::types::number::operator > (const var & rhs) {
    return new number{
        static_cast<double>(
            std::any_cast<double>(this->value) >
            std::any_cast<double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator >= (const var & rhs) {
    return new number{
        static_cast<double>(
            std::any_cast<double>(this->value) >=
            std::any_cast<double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator < (const var & rhs) {
    return new number{
        static_cast<double>(
            std::any_cast<double>(this->value) <
            std::any_cast<double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator <= (const var & rhs) {
    return new number{
        static_cast<double>(
            std::any_cast<double>(this->value) <=
            std::any_cast<double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator == (const var & rhs) {
    return new number{
        static_cast<double>(
            std::any_cast<double>(this->value) ==
            std::any_cast<double>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::number::operator != (const var & rhs) {
    return new number{
        static_cast<double>(
            std::any_cast<double>(this->value) !=
            std::any_cast<double>(rhs->value)
        )
    };
}

fell::types::variable::var & fell::types::number::operator [] (const var &) {
    throw std::runtime_error{"No subscript operator on variable of type Number."};
}
