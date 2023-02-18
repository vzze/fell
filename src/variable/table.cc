#include "variable.hh"
#include "util.hh"

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

fell::types::variable::var & fell::types::table::operator [] (const var & key) {
    try {
        return util::get_value<tbl>(this)->at(util::get_value<string::str>(key));
    } catch(...) {
        (*util::get_value<tbl>(this))[util::get_value<string::str>(key)] = nullptr;
        return util::get_value<tbl>(this)->at(util::get_value<string::str>(key));
    }
}

fell::types::variable::var & fell::types::table::operator [] (const var && key) {
    try {
        return util::get_value<tbl>(this)->at(util::get_value<string::str>(key));
    } catch(...) {
        (*util::get_value<tbl>(this))[util::get_value<string::str>(key)] = nullptr;
        return util::get_value<tbl>(this)->at(util::get_value<string::str>(key));
    }
}

fell::types::variable::var & fell::types::table::operator [] (const string::str key) {
    try {
        return util::get_value<tbl>(this)->at(key);
    } catch(...) {
        (*util::get_value<tbl>(this))[key] = nullptr;
        return util::get_value<tbl>(this)->at(key);
    }
}

fell::types::table::~table() {
    delete util::get_value<tbl>(this);
}
