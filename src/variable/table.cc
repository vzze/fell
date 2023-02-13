#include "table.hh"

fell::types::table::table(std::unordered_map<variable::var, variable::var> tbl) : variable(tbl) {}

fell::types::variable::var fell::types::table::operator + (const var & rhs) {
    throw "Variable of type Table can't be added.";
}

fell::types::variable::var fell::types::table::operator - (const var & rhs) {
    throw "Variable of type Table can't be minus'd.";
}

fell::types::variable::var fell::types::table::operator * (const var & rhs) {
    throw "Variable of type Table can't be multiplied.";
}

fell::types::variable::var fell::types::table::operator / (const var & rhs) {
    throw "Variable of type Table can't be divided.";
}

fell::types::variable::var fell::types::table::operator > (const var & rhs) {
    throw "Variable of type Table can't be compared.";
}

fell::types::variable::var fell::types::table::operator >= (const var & rhs) {
    throw "Variable of type Table can't be compared.";
}

fell::types::variable::var fell::types::table::operator < (const var & rhs) {
    throw "Variable of type Table can't be compared.";
}

fell::types::variable::var fell::types::table::operator <= (const var & rhs) {
    throw "Variable of type Table can't be compared.";
}

fell::types::variable::var fell::types::table::operator == (const var & rhs) {
    throw "Variable of type Table can't be compared.";
}

fell::types::variable::var fell::types::table::operator != (const var & rhs) {
    throw "Variable of type Table can't be compared.";
}

fell::types::variable::var fell::types::table::operator [] (const var & rhs) {
    try {
        return std::any_cast<std::unordered_map<var, var>>(&this->value)->at(rhs);
    } catch(...) {
        (*std::any_cast<std::unordered_map<var, var>>(&this->value))[rhs] = new number{
            0
        };

        return std::any_cast<std::unordered_map<var, var>>(&this->value)->at(rhs);
    }
}
