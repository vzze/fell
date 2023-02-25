#include "fell.hh"

void dump_table(const fell::types::variable::var & table, std::string space = "") {
    for(auto & [k, v] : *fell::util::get_value<fell::types::table::tbl>(table.get()))
        if(v == nullptr)
            std::cout << space << k << ": " << "UNDEFINED" << '\n';
        else
            try {
                const auto & ref = fell::util::get_value<fell::types::number::num>(v.get());
                std::cout << space << k << ": " << ref << '\n';
            } catch(...) {
                try {
                    const auto & ref = fell::util::get_value<fell::types::string::str>(v.get());
                    std::cout << space << k << ": \"" << ref << "\"\n";
                } catch(...) {
                    try {
                        [[maybe_unused]] const auto & ref = fell::util::get_value<fell::types::nihil::nil>(v.get());
                        std::cout << space << k << ": " << "NIL" << '\n';
                    } catch(...) {
                        try {
                            fell::util::get_value<fell::types::table::tbl>(v.get());
                            std::cout << space << k << ":\n";
                            dump_table(v, space + "  ");
                        } catch(...) {
                            std::cout << space << k << ": Function\n";
                        }
                    }
                }
            }
}

int main(int argc, char ** argv) {
    fell::std::init();
    if(fell::util::parse_args(argc, argv))
        fell::lex::parse_file(argv[1]);
    /* dump_table(fell::lex::global_table); */
}
