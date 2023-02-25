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
            return api::make_var<types::nihil>();
        }
    },
    {
        "assert",
        [](fell::api::params params) -> fell::types::variable::var {
            if(params.number_of_params() == 0)
                throw ::std::runtime_error{"assert expects at least 1 parameter."};

            auto s = params.get_param(0);

            bool assertion_failed = false;

            try {
                s.get_value<api::param::nil>();
                assertion_failed = true;
            } catch(...) {
                try {
                    assertion_failed = s.get_value<api::param::num>() == 0.0;
                } catch(...) {}
            }

            if(assertion_failed) {
                if(params.number_of_params() == 1)
                    ::std::cout << "Assertion failed.\n";
                else {
                    try {
                        ::std::cout << "Assertion failed: " + params.get_param(1).get_value<api::param::str>();
                    } catch(...) {
                        throw ::std::runtime_error{"assert expects String as second argument."};
                    }
                }
            }

            return api::make_var<types::nihil>();
        }
    },
    {
        "if",
        [](fell::api::params params) -> fell::types::variable::var {
            if(params.number_of_params() < 2)
                throw ::std::runtime_error{"if expects at least 2 parameters."};

            auto s = params.get_param(0);

            bool value = true;

            try {
                s.get_value<api::param::nil>();
                value = false;
            } catch(...) {
                try {
                    value = s.get_value<api::param::num>() != 0.0;
                } catch(...) {}
            }

            if(value == true)
                return params.get_param(1).expose()->call({});
            else
                return params.get_param(2).expose()->call({});

            return api::make_var<types::nihil>();
        }
    },
};

void fell::std::init() {
    for(auto & [n, f] : general)
        api::set_global_var(n, api::make_func(f));
}
