#include "std.hh"

std::vector<std::pair<std::string, std::function<fell::types::variable::var(fell::api::params)>>> fell::std::general = {
    {
        "print",
        [](fell::api::params params) -> fell::types::variable::var {
            params.for_each_param([](api::param par) {
                try {
                    ::std::cout << par.get_value<api::param::num>() << ' ';
                } catch(...) {
                    try {
                        ::std::cout << par.get_value<api::param::str>() << ' ';
                    } catch(...) {
                        try {
                            ::std::cout << &par.get_value<api::param::fun>() << ' ';
                        } catch(...) {
                            try {
                                ::std::cout << par.get_value<api::param::tbl>() << ' ';
                            } catch(...) {
                                ::std::cout << "nil ";
                            }
                        }
                    }
                }
            });
            ::std::cout << '\n';
            return util::make_var<types::nihil>();
        }
    }
};

void fell::std::init() {
    for(auto & [n, f] : general) {
        (*lex::global_table)[n] = api::make_func(f);
    }
}
