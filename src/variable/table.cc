#include "table.hh"

fell::types::table::table(std::unordered_map<std::string, variable::var> tbl) : variable(tbl) {}

fell::types::variable::var fell::types::table::operator + (const var &) {
    throw std::runtime_error{"Variable of type Table can't be added."};
}

fell::types::variable::var fell::types::table::operator - (const var &) {
    throw std::runtime_error{"Variable of type Table can't be substracted."};
}

fell::types::variable::var fell::types::table::operator * (const var &) {
    throw std::runtime_error{"Variable of type Table can't be multiplied."};
}

fell::types::variable::var fell::types::table::operator / (const var &) {
    throw std::runtime_error{"Variable of type Table can't be divided."};
}

fell::types::variable::var fell::types::table::operator > (const var &) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator >= (const var &) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator < (const var &) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator <= (const var &) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator == (const var &) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator != (const var &) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var & fell::types::table::operator [] (const var & offset) {
    try {
        return std::any_cast<std::unordered_map<std::string, var>>(&this->value)->at(*std::any_cast<std::string>(&offset->value));
    } catch(...) {
        (*std::any_cast<std::unordered_map<std::string, var>>(&this->value))[*std::any_cast<std::string>(&offset->value)] = nullptr;

        return std::any_cast<std::unordered_map<std::string, var>>(&this->value)->at(*std::any_cast<std::string>(&offset->value));
    }
}

fell::types::table::~table() {
    for(auto & [k, v] : *std::any_cast<std::unordered_map<std::string, variable::var>>(&this->value))
        delete v;
}
