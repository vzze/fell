#include "lang.hh"
#include "util.hh"

#include <iostream>

void fell::lang::dump_table(types::variable::var & table, std::string space) {
    for(auto & [k, v] : *fell::util::get_value<types::table::tbl>(table)) {
        if(v == nullptr)
            std::cout << space << k << ": " << "UNDEFINED" << '\n';
        else
            try {
                const auto & ref = util::get_value<types::number::num>(v);
                std::cout << space << k << ": " << ref << '\n';
            } catch(...) {
                try {
                    const auto & ref = util::get_value<types::string::str>(v);
                    std::cout << space << k << ": " << ref << '\n';
                } catch(...) {
                    try {
                        [[maybe_unused]] const auto & ref = util::get_value<types::nihil::nil>(v);
                        std::cout << space << k << ": " << "NIL" << '\n';
                    } catch(...) {
                        std::cout << k << ":\n";
                        space += "  ";
                        dump_table(v, space);
                    }
                }
            }
    }
}
