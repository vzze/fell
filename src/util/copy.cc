#include "util.hh"

void fell::util::copy(types::variable::var & a, const types::variable::var & b) {
    try {
        a = make_var<types::number>(get_value<types::number::num>(b.get()));
    } catch(...) {
        try {
            a = make_var<types::string>(get_value<types::string::str>(b.get()));
        } catch(...) {
            try {
                a = make_var<types::nihil>(get_value<types::nihil::nil>(b.get()));
            } catch(...) {
                a = make_var<types::table>();

                for(const auto & kv : *get_value<types::table::tbl>(b.get()))
                    copy((*a)[kv.first], kv.second);
            }
        }
    }
}
