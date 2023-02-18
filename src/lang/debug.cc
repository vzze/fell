#include "lang.hh"
#include "util.hh"

#include <iostream>

void fell::lang::dump_table(types::variable::var & table, std::string space) {
    for(auto & [k, v] : *fell::util::get_value<types::table::tbl>(table)) {
        try {
            const auto & ref = util::get_value<types::number::num>(v);
            std::cout << space << k << ": " << ref << '\n';
        } catch(...) {
            try {
                const auto & ref = util::get_value<types::string::str>(v);
                std::cout << space << k << ": " << ref << '\n';
            } catch(...) {
                std::cout << k << ":\n";
                space += "  ";
                dump_table(v, space);
            }
        }
    }
}
