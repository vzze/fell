#include "std.hh"

std::vector<std::pair<std::string, std::function<fell::types::variable::var(fell::api::params)>>> fell::std::io = {
    {
        "open",
        [](api::params params) -> types::variable::var {
            ::std::FILE * file;

            if(fopen_s(
                &file,
                (lex::project_root / params.get_param(0).get_value<api::param::str>()).string().c_str(),
                params.get_param(1).get_value<api::param::str>().c_str()
            ) == 0)
                return api::make_var<types::file>(file);
            else
                return api::make_var<types::nihil>();
        }
    },
    {
        "close",
        [](api::params params) -> types::variable::var {
            if(fclose(params.get_param(0).get_value<api::param::fil>()) == 0)
                return api::make_var<types::number>(1);
            else
                return api::make_var<types::nihil>();
        }
    },
    {
        "read",
        [](api::params params) -> types::variable::var {
            const auto value = params.get_param(1).get_value<api::param::str>();
            if(value == "num") {
                double d;

                if(fscanf_s(params.get_param(0).get_value<api::param::fil>(), "%lf", &d) > 0)
                    return api::make_var<types::number>(d);
                else
                    return api::make_var<types::nihil>();
            } else if(value == "str") {
                ::std::string str; str.resize(4208);

                if(fscanf_s(params.get_param(0).get_value<api::param::fil>(), "%s", str.data()) > 0) {
                    str.shrink_to_fit();
                    return api::make_var<types::string>(str);
                } else
                    return api::make_var<types::nihil>();

            } else if(value == "line") {
                ::std::string str; str.resize(4208);
                if(fgets(str.data(), 4208, params.get_param(0).get_value<api::param::fil>()) != nullptr) {
                    str.shrink_to_fit();
                    return api::make_var<types::string>(str);
                } else
                    return api::make_var<types::nihil>();
            } else if(value == "all") {
                const auto file = params.get_param(0).get_value<api::param::fil>();
                fseek(file, 0, SEEK_END);

                long sz = ftell(file);
                ::std::string str; str.resize(static_cast<::std::size_t>(sz));

                fseek(file, 0, SEEK_SET);
                fread(&str[0], static_cast<::std::size_t>(sz), 1, file);

                return api::make_var<types::string>(str);
            } else {
                return api::make_var<types::nihil>();
            }
        }
    },
    {
        "write",
        [](api::params params) -> types::variable::var {
            [[maybe_unused]] const auto file = params.get_param(0).get_value<api::param::fil>();
            auto fmt = ::std::string{""};
            if(params.number_of_params() >= 3)
                fmt = params.get_param(2).get_value<api::param::str>();
            [[maybe_unused]] const auto par  = params.get_param(1);

            try {
                if(fmt != "")
                    fprintf_s(file, fmt.c_str(), par.get_value<api::param::num>());
                else
                    fprintf_s(file, "%lf", par.get_value<api::param::num>());
            } catch(...) {
                try {
                    if(fmt != "")
                        fprintf_s(file, fmt.c_str(), par.get_value<api::param::str>().c_str());
                    else
                        fprintf_s(file, "%s", par.get_value<api::param::str>().c_str());
                } catch(...) {
                    try {
                        if(fmt != "")
                            fprintf_s(file, fmt.c_str(), &par.get_value<api::param::fun>());
                        else
                            fprintf_s(file, "%p", &par.get_value<api::param::fun>());
                    } catch(...) {
                        try {
                            if(fmt != "")
                                fprintf_s(file, fmt.c_str(), par.get_value<api::param::tbl>());
                            else
                                fprintf_s(file, "%p", par.get_value<api::param::tbl>());
                        } catch(...) {
                            try {
                                if(fmt != "")
                                    fprintf_s(file, fmt.c_str(), par.get_value<api::param::fil>());
                                else
                                    fprintf_s(file, "%p", par.get_value<api::param::fil>());
                            } catch(...) {
                                fprintf_s(file, "nil");
                            }
                        }
                    }
                }
            }

            return api::make_var<types::nihil>();
        }
    }
};

void fell::std::init_io() {
    auto tbl = api::make_var<types::table>();

    (*tbl)["stdin"] = api::make_var<types::file>(stdin);
    (*tbl)["stdout"] = api::make_var<types::file>(stdout);
    (*tbl)["stderr"] = api::make_var<types::file>(stderr);

    for(auto & [n, f] : io)
        (*tbl)[n] = api::make_func(f);

    api::set_global_var("io", ::std::move(tbl));
}
