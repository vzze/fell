#include "variable.hh"
#include "util.hh"

fell::types::table::table(tbl table) : variable(table) {}

fell::types::variable::var fell::types::table::operator + (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be added."};
}

fell::types::variable::var fell::types::table::operator - (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be substracted."};
}

fell::types::variable::var fell::types::table::operator * (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be multiplied."};
}

fell::types::variable::var fell::types::table::operator / (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be divided."};
}

fell::types::variable::var fell::types::table::operator % (const variable *) {
    throw std::runtime_error{"Variable of type Table can't have a remainder."};
}

fell::types::variable::var fell::types::table::operator > (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator >= (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator < (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator <= (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator == (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator != (const variable *) {
    throw std::runtime_error{"Variable of type Table can't be compared."};
}

fell::types::variable::var fell::types::table::operator && (const variable * rhs) {
    try {
        util::get_value<nihil::nil>(rhs);
        return fell::util::make_var<number>(0);
    } catch(...) {
        try {
            return fell::util::make_var<number>(
                static_cast<number::num>(
                    true && (util::get_value<number::num>(rhs) != 0.0)
                )
            );
        } catch(...) {
            return fell::util::make_var<number>(1);
        }
    }
}

fell::types::variable::var fell::types::table::operator || (const variable *) {
    return fell::util::make_var<number>(1);
}
fell::types::variable::var & fell::types::table::operator [] (const variable * key) {
    try {
        auto key_t = util::get_value<string::str>(key);
        auto & ref = util::get_value<tbl>(this)->first;

        return ref[util::get_value<string::str>(key)];
    } catch(...) {
        auto key_t = static_cast<std::size_t>(util::get_value<number::num>(key));
        auto & ref = util::get_value<tbl>(this)->second;

        if(ref.size() <= key_t) {
            auto sz = ref.size();

            ref.resize(key_t + 1);

            for(std::size_t i = sz; i < ref.size(); ++i)
                ref[i] = util::make_var<types::nihil>();
        }

        return ref[key_t];
    }
}

fell::types::variable::var & fell::types::table::operator [] (const string::str key) {
    return util::get_value<tbl>(this)->first[key];
}

fell::types::variable::var & fell::types::table::operator [] (const std::size_t key) {
    try {
        return util::get_value<tbl>(this)->second.at(key);
    } catch(...) {
        auto & ref = util::get_value<tbl>(this)->second;

        if(ref.size() <= key) {
            auto sz = ref.size();

            ref.resize(key + 1);

            for(std::size_t i = sz; i < ref.size(); ++i)
                ref[i] = util::make_var<types::nihil>();
        }

        return ref[key];
    }
}

fell::types::table::~table() {
    delete util::get_value<tbl>(this);
}

fell::types::variable::var fell::types::table::call(std::vector<lex::inmemory> &&, bool) {
    throw std::runtime_error{"Table variable can't be called."};
}
