#include "string.hh"

fell::types::string::string(std::string str) : variable(str) {}

fell::types::variable::var fell::types::string::operator + (const var & rhs) {
    return new string{
        std::any_cast<std::string>(this->value) +
        std::any_cast<std::string>(rhs->value)
    };
}

fell::types::variable::var fell::types::string::operator - (const var & rhs) {
    throw "Variable of type string can't be minus'd";
}

fell::types::variable::var fell::types::string::operator * (const var & rhs) {
    throw "Variable of type string can't be multiplied.";
}

fell::types::variable::var fell::types::string::operator / (const var & rhs) {
    throw "Variable of type string can't be divided.";
}

fell::types::variable::var fell::types::string::operator > (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<std::string>(this->value) >
            std::any_cast<std::string>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator >= (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<std::string>(this->value) >=
            std::any_cast<std::string>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator < (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<std::string>(this->value) <
            std::any_cast<std::string>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator <= (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<std::string>(this->value) <=
            std::any_cast<std::string>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator == (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<std::string>(this->value) ==
            std::any_cast<std::string>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator != (const var & rhs) {
    return new number{
        static_cast<long double>(
            std::any_cast<std::string>(this->value) !=
            std::any_cast<std::string>(rhs->value)
        )
    };
}

fell::types::variable::var fell::types::string::operator [] (const var & offset) {
    throw "No subscript operator on string.";
}
