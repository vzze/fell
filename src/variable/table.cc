#include "table.hh"

fell::types::table::table(tbl table) : variable(table) {}

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
        return std::any_cast<tbl>(&this->value)->at(*std::any_cast<string::str>(&offset->value));
    } catch(...) {
        (*std::any_cast<tbl>(&this->value))[*std::any_cast<string::str>(&offset->value)] = nullptr;

        return std::any_cast<tbl>(&this->value)->at(*std::any_cast<string::str>(&offset->value));
    }
}

fell::types::variable::var & fell::types::table::operator [] (const string::str offset) {
    try {
        return std::any_cast<tbl>(&this->value)->at(offset);
    } catch(...) {
        (*std::any_cast<tbl>(&this->value))[offset] = nullptr;

        return std::any_cast<tbl>(&this->value)->at(offset);
    }
}

fell::types::table::~table() {
    for(auto & [k, v] : *std::any_cast<tbl>(&this->value)) {
        delete v;
    }
}
