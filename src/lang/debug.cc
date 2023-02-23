#include "lang.hh"

#include <iostream>

void fell::lang::dump_table(const types::variable::var & table, std::string space) {
    for(auto & [k, v] : *fell::util::get_value<types::table::tbl>(table.get()))
        if(v == nullptr)
            std::cout << space << k << ": " << "UNDEFINED" << '\n';
        else
            try {
                const auto & ref = util::get_value<types::number::num>(v.get());
                std::cout << space << k << ": " << ref << '\n';
            } catch(...) {
                try {
                    const auto & ref = util::get_value<types::string::str>(v.get());
                    std::cout << space << k << ": \"" << ref << "\"\n";
                } catch(...) {
                    try {
                        [[maybe_unused]] const auto & ref = util::get_value<types::nihil::nil>(v.get());
                        std::cout << space << k << ": " << "NIL" << '\n';
                    } catch(...) {
                        std::cout << space << k << ":\n";
                        dump_table(v, space + "  ");
                    }
                }
            }
}

void fell::lang::dump_var(const types::variable::var & var) {
        try {
            const auto & ref = util::get_value<types::number::num>(var.get());
            std::cout << ref << '\n';
        } catch(...) {
            try {
                const auto & ref = util::get_value<types::string::str>(var.get());
                std::cout << '"' << ref << "\"\n";
            } catch(...) {
                try {
                    [[maybe_unused]] const auto & ref = util::get_value<types::nihil::nil>(var.get());
                    std::cout << "NIL" << '\n';
                } catch(...) {
                    dump_table(var, "  ");
                }
            }
        }
}
