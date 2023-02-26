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
                try {
                    a = make_var<types::table>();

                    for(const auto & [k, v] : get_value<types::table::tbl>(b.get())->first)
                        copy((*a)[k], v);

                    for(std::size_t i = 0; const auto & v : get_value<types::table::tbl>(b.get())->second)
                        copy((*a)[i++], v);

                } catch(...) {
                    a = make_var<types::func>(get_value<types::func::data>(b.get()));
                }
            }
        }
    }
}
