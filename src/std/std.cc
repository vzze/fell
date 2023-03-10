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
    {
        "while",
        [](fell::api::params params) -> fell::types::variable::var {
            if(params.number_of_params() < 2)
                throw ::std::runtime_error{"while expects 2 parameters."};

            auto condition = params.get_param(0).expose()->call({});

            bool value = true;

            try {
                api::get_value<api::param::nil>(condition);
                value = false;
            } catch(...) {
                try {
                    value = api::get_value<api::param::num>(condition) != 0.0;
                } catch(...) {}
            }

            while(value) {
                [[maybe_unused]] auto u = params.get_param(1).expose()->call({});

                condition = params.get_param(0).expose()->call({});

                try {
                    api::get_value<api::param::nil>(condition);
                    value = false;
                } catch(...) {
                    try {
                        value = api::get_value<api::param::num>(condition) != 0.0;
                    } catch(...) {}
                }
            }

            return api::make_var<types::nihil>();
        }
    },
    {
        "for_kv",
        [](fell::api::params params) -> fell::types::variable::var {
            if(params.number_of_params() < 2)
                throw ::std::runtime_error{"for_kv expects 2 parameters."};

            auto & table = params.get_param(0).get_value<api::param::tbl>()->first;

            for(auto & [k, v] : table) {
                ::std::vector<lex::inmemory> args;
                args.reserve(2);
                args.emplace_back(api::make_var<types::string>(k));
                args.emplace_back(&v);
                [[maybe_unused]] auto u = params.get_param(1).expose()->call(::std::move(args));
            }

            return api::make_var<types::nihil>();
        }
    },
    {
        "for_each",
        [](fell::api::params params) -> fell::types::variable::var {
            if(params.number_of_params() < 2)
                throw ::std::runtime_error{"for_each expects 2 parameters."};

            auto & array = params.get_param(0).get_value<api::param::tbl>()->second;

            for(auto & v : array) {
                ::std::vector<lex::inmemory> args;
                args.reserve(1); args.emplace_back(&v);
                [[maybe_unused]] auto u = params.get_param(1).expose()->call(::std::move(args));
            }

            return api::make_var<types::nihil>();
        }
    },
    {
        "require",
        [](fell::api::params params) -> fell::types::variable::var {
            if(params.number_of_params() < 1)
                throw ::std::runtime_error{"require expects 1 parameter."};

            auto & module = params.get_param(0).get_value<api::param::str>();
            auto copy = lex::project_root;

            copy.append(module);
            lex::parse_file(copy);

            return api::make_var<types::nihil>();
        }
    },
    {
        "tonumber",
        [](fell::api::params params) -> fell::types::variable::var {
            return api::make_var<types::number>(::std::stod(params.get_param(0).get_value<api::param::str>()));
        }
    },
    {
        "tostring",
        [](fell::api::params params) -> fell::types::variable::var {
            return api::make_var<types::string>(::std::to_string(params.get_param(0).get_value<api::param::num>()));
        }
    },
    {
        "error",
        [](fell::api::params params) -> fell::types::variable::var {
            throw ::std::runtime_error{params.get_param(0).get_value<api::param::str>()};
        }
    },
    {
        "type",
        [](fell::api::params params) -> fell::types::variable::var {
            auto par = params.get_param(0);
            try {
                par.get_value<api::param::num>();
                return api::make_var<types::string>("num");
            } catch(...) {
                try {
                    par.get_value<api::param::str>();
                    return api::make_var<types::string>("str");
                } catch(...) {
                    try {
                        par.get_value<api::param::fun>();
                        return api::make_var<types::string>("fun");
                    } catch(...) {
                        try {
                            par.get_value<api::param::tbl>();
                            return api::make_var<types::string>("tbl");
                        } catch(...) {
                            return api::make_var<types::string>("nil");
                        }
                    }
                }
            }
        }
    },
    {
        "protected_call",
        [](fell::api::params params) -> fell::types::variable::var {
            if(params.number_of_params() < 1)
                throw ::std::runtime_error{"protected_call expects atleast 1 parameter."};

            auto exposed = ::std::move(params.expose());
            auto func = ::std::move(exposed[0]);

            exposed.erase(exposed.begin());

            if(func.non_reference) {
                return func.non_reference->call(::std::move(exposed), true);
            } else if(func.reference) {
                return (*func.reference)->call(::std::move(exposed), true);
            } else {
                throw ::std::runtime_error{"Unresolved parameter."};
            }
        }
    }
};

void fell::std::init() {
    for(auto & [n, f] : general)
        api::set_global_var(n, api::make_func(f));
}
