#include "std.hh"

std::vector<std::pair<std::string, std::function<fell::types::variable::var(fell::api::params)>>> fell::std::table = {
    {
        "size",
        [](api::params params) -> fell::types::variable::var {
            return api::make_var<types::number>(
                params.get_param(0).get_value<api::param::tbl>()->second.size()
            );
        }
    },
    {
        "concat",
        [](api::params params) -> fell::types::variable::var {
            if(params.number_of_params() == 4) {
                auto start = params.get_param(2).get_value<api::param::num>();
                auto finish = params.get_param(3).get_value<api::param::num>();

                auto sep = params.get_param(1).get_value<api::param::str>();
                const auto & tbl = params.get_param(0).get_value<api::param::tbl>();

                ::std::string concatenated;

                for(::std::size_t i = static_cast<::std::size_t>(start); i < static_cast<::std::size_t>(finish); ++i) {
                    ::std::string str;

                    try {
                        str = api::get_value<api::param::str>(tbl->second.at(i));
                    } catch(...) {
                        str = ::std::to_string(api::get_value<api::param::num>(tbl->second.at(i)));
                    }

                    concatenated += str + sep;
                }

                concatenated.resize(concatenated.size() - sep.size());

                return api::make_var<types::string>(concatenated);
            } else if(params.number_of_params() == 3) {
                auto start = params.get_param(2).get_value<api::param::num>();

                auto sep = params.get_param(1).get_value<api::param::str>();
                const auto & tbl = params.get_param(0).get_value<api::param::tbl>();

                ::std::string concatenated;

                for(::std::size_t i = static_cast<::std::size_t>(start); i < tbl->second.size(); ++i) {
                    ::std::string str;

                    try {
                        str = api::get_value<api::param::str>(tbl->second.at(i));
                    } catch(...) {
                        str = ::std::to_string(api::get_value<api::param::num>(tbl->second.at(i)));
                    }

                    concatenated += str + sep;
                }

                return api::make_var<types::string>(concatenated);
            } else if(params.number_of_params() == 2) {
                auto sep = params.get_param(1).get_value<api::param::str>();
                const auto & tbl = params.get_param(0).get_value<api::param::tbl>();

                ::std::string concatenated;

                for(const auto & var : tbl->second) {
                    ::std::string str;

                    try {
                        str = api::get_value<api::param::str>(var);
                    } catch(...) {
                        str = ::std::to_string(api::get_value<api::param::num>(var));
                    }

                    concatenated += str + sep;
                }

                return api::make_var<types::string>(concatenated);
            } else {
                const auto & tbl = params.get_param(0).get_value<api::param::tbl>();

                ::std::string concatenated;

                for(const auto & var : tbl->second) {
                    ::std::string str;

                    try {
                        str = api::get_value<api::param::str>(var);
                    } catch(...) {
                        str = ::std::to_string(api::get_value<api::param::num>(var));
                    }

                    concatenated += str + " ";
                }

                return api::make_var<types::string>(concatenated);
            }

            return api::make_var<types::nihil>();
        }
    },
    {
        "move",
        [](api::params params) -> types::variable::var {
            if(params.number_of_params() < 3)
                throw ::std::runtime_error{"table.move requires at least 3 parameters"};

            auto & tbl = params.get_param(0).get_value<api::param::tbl>();

            auto start = params.get_param(1).get_value<api::param::num>();
            auto finish = params.get_param(2).get_value<api::param::num>();

            if(params.number_of_params() == 5) {
                auto ins = params.get_param(3).get_value<api::param::num>();
                auto tbl2 = params.get_param(4);

                for(::std::size_t i = static_cast<::std::size_t>(start); i < static_cast<::std::size_t>(finish); ++i) {
                    (*tbl2.expose())[static_cast<::std::size_t>(ins)] = ::std::exchange(tbl->second.at(i), nullptr);
                    ++ins;
                }

                tbl->second.erase(tbl->second.begin() + static_cast<::std::int64_t>(start), tbl->second.begin() + static_cast<::std::int64_t>(finish));
            } else {
                tbl->second.erase(tbl->second.begin() + static_cast<::std::int64_t>(start), tbl->second.begin() + static_cast<::std::int64_t>(finish));
            }


            return api::make_var<types::nihil>();
        }
    },
    {
        "sort",
        [](api::params params) -> types::variable::var {
            if(params.number_of_params() < 1)
                throw ::std::runtime_error{"sort expects at least 1 parameter."};

            if(params.number_of_params() == 1) {
                auto & param = params.get_param(0).get_value<api::param::tbl>();
                ::std::sort(
                    param->second.begin(),
                    param->second.end(),
                    [](types::variable::var & a, types::variable::var & b) -> bool {
                        return static_cast<bool>(api::get_value<api::param::num>(*a < b.get()));
                    }
                );
            } else {
                auto & param = params.get_param(0).get_value<api::param::tbl>();
                auto & func = params.get_param(1).expose();
                ::std::sort(
                    param->second.begin(),
                    param->second.end(),
                    [&](types::variable::var & a, types::variable::var & b) -> bool {
                        ::std::vector<lex::inmemory> x;
                        x.emplace_back(&a); x.emplace_back(&b);
                        return static_cast<bool>(
                            api::get_value<api::param::num>(
                                func->call(::std::move(x))
                            )
                        );
                    }
                );
            }

            return api::make_var<types::nihil>();
        }
    },
    {
        "remove",
        [](api::params params) -> types::variable::var {
            auto & tbl = params.get_param(0).get_value<api::param::tbl>();

            auto ind = params.get_param(1).get_value<api::param::num>();

            auto ret = ::std::exchange(tbl->second.at(static_cast<::std::size_t>(ind)), nullptr);
            tbl->second.erase(tbl->second.begin() + static_cast<::std::int64_t>(ind));
            return ret;
        }
    },
    {
        "insert",
        [](api::params params) -> types::variable::var {
            auto & tbl = params.get_param(0).get_value<api::param::tbl>();

            auto ind = params.get_param(1).get_value<api::param::num>();

            tbl->second.insert(tbl->second.begin() + static_cast<::std::int64_t>(ind), nullptr);

            api::copy_var(tbl->second[static_cast<::std::size_t>(ind)], params.get_param(2).expose());

            return api::make_var<types::nihil>();
        }
    },
    {
        "pack",
        [](api::params params) -> types::variable::var {
            auto tbl = api::make_var<types::table>();

            for(::std::size_t i = 0; const auto & [_, v] : params.get_param(0).get_value<api::param::tbl>()->first)
                api::copy_var((*tbl)[i++], v);

            return tbl;
        }
    }
};

void fell::std::init_table() {
    auto tbl = api::make_var<types::table>();

    for(auto & [n, f] : table)
        (*tbl)[n] = api::make_func(f);

    api::set_global_var("table", ::std::move(tbl));
}
