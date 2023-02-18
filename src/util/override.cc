#include "util.hh"

void fell::util::override(types::variable::var & a, const types::variable::var & b) {
    try {
        a = make_var<types::number>(get_value<types::number::num>(b));
    } catch(...) {
        try {
            a = make_var<types::string>(get_value<types::string::str>(b));
        } catch(...) {
            try {
                a = make_var<types::nihil>(get_value<types::nihil::nil>(b));
            } catch(...) {
                a = make_var<types::table>();

                for(const auto & kv : *get_value<types::table::tbl>(b))
                    override((*a)[kv.first], kv.second);
            }
        }
    }
}
