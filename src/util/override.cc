#include "override.hh"

void fell::util::override(types::variable::var & a, const types::variable::var b) {
    delete a;

    try {
        a = new types::number{std::any_cast<types::number::num>(b->value)};
    } catch(...) {
        try {
            a = new types::string{std::any_cast<types::string::str>(b->value)};
        } catch(...) {
            a = new types::table{std::any_cast<types::table::tbl>(b->value)};
        }
    }
}
