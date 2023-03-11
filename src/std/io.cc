#include "std.hh"

std::vector<std::pair<std::string, std::function<fell::types::variable::var(fell::api::params)>>> fell::std::io = {
    {
        "open",
        [](api::params params) -> types::variable::var {
            if(params.number_of_params() == 1) {
                const auto s = _fell__open(
                    (lex::project_root / params.get_param(0).get_value<api::param::str>()).string().c_str(),
                    "r"
                );

                if(s == nullptr)
                    return util::make_var<types::nihil>();
                else
                    return api::make_var<types::number>(
                        ::std::bit_cast<::std::intptr_t>(s)
                    );
            } else {
                const auto s = _fell__open(
                    (lex::project_root / params.get_param(0).get_value<api::param::str>()).string().c_str(),
                    params.get_param(1).get_value<api::param::str>().c_str()
                );

                if(s == nullptr)
                    return util::make_var<types::nihil>();
                else
                    return api::make_var<types::number>(
                        ::std::bit_cast<::std::intptr_t>(s)
                    );
            }
        }
    },
    {
        "close",
        [](api::params params) -> types::variable::var {
            const auto err = _fell__close( // this is not safe code
                ::std::bit_cast<::std::FILE*>( // but it works
                    static_cast<::std::intptr_t>(
                        params.get_param(0).get_value<api::param::num>()
                    )
                )
            );

            if(err == 0) {
                return util::make_var<types::number>(1);
            } else {
                return util::make_var<types::nihil>();
            }
        }
    },
    {
        "read",
        [](api::params params) -> types::variable::var {
            const auto file = ::std::bit_cast<::std::FILE*>(
                static_cast<::std::intptr_t>(
                    params.get_param(0).get_value<api::param::num>()
                )
            );

            const auto var_type = params.get_param(1).get_value<api::param::str>();

            if(var_type == "num") {
                return api::make_var<types::number>(_fell__read_number(file));
            } else if(var_type == "line") {
                return api::make_var<types::string>(::std::string{_fell__read_line(file)});
            } else if(var_type == "all") {
                char * buffer;
                _fell__read_file(file, &buffer);
                auto value = ::std::string{buffer};
                _fell__free(&buffer);
                return api::make_var<types::string>(value);
            } else {
                return api::make_var<types::nihil>();
            }
        }
    }
};

void fell::std::init_io() {
    auto tbl = api::make_var<types::table>();

    (*tbl)["stdin"] = api::make_var<types::number>(static_cast<double>(FILE::STDIN));
    (*tbl)["stdout"] = api::make_var<types::number>(static_cast<double>(FILE::STDOUT));
    (*tbl)["stderr"] = api::make_var<types::number>(static_cast<double>(FILE::STDERR));

    for(auto & [n, f] : io)
        (*tbl)[n] = api::make_func(f);

    api::set_global_var("io", ::std::move(tbl));
}
