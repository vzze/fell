#include "variable.hh"
#include "util.hh"
#include "lexer.hh"
#include "api.hh"

fell::types::func::func(data f) : variable(f) {}

fell::types::variable::var fell::types::func::operator + (const variable *) {
    throw std::runtime_error{"Function can't be added."};
}

fell::types::variable::var fell::types::func::operator - (const variable *) {
    throw std::runtime_error{"Function can't be substracted."};
}

fell::types::variable::var fell::types::func::operator * (const variable *) {
    throw std::runtime_error{"Function can't be multiplied."};
}

fell::types::variable::var fell::types::func::operator / (const variable *) {
    throw std::runtime_error{"Function can't be divided."};
}

fell::types::variable::var fell::types::func::operator % (const variable *) {
    throw std::runtime_error{"Function can't have a remainder."};
}

fell::types::variable::var fell::types::func::operator > (const variable *) {
    throw std::runtime_error{"Function can't be compared."};
}

fell::types::variable::var fell::types::func::operator >= (const variable *) {
    throw std::runtime_error{"Function can't be compared."};
}

fell::types::variable::var fell::types::func::operator < (const variable *) {
    throw std::runtime_error{"Function can't be compared."};
}

fell::types::variable::var fell::types::func::operator <= (const variable *) {
    throw std::runtime_error{"Function can't be compared."};
}

fell::types::variable::var fell::types::func::operator == (const variable *) {
    throw std::runtime_error{"Function can't be compared."};
}

fell::types::variable::var fell::types::func::operator != (const variable *) {
    throw std::runtime_error{"Function can't be compared."};
}

fell::types::variable::var & fell::types::func::operator [] (const variable *) {
    throw std::runtime_error{"Function has no subscript operator."};
}

fell::types::variable::var & fell::types::func::operator [] (const string::str) {
    throw std::runtime_error{"Function has no subscript operator."};
}

fell::types::variable::var & fell::types::func::operator [] (const std::size_t) {
    throw std::runtime_error{"Function has no subscript operator."};
}

fell::types::variable::var fell::types::func::operator && (const variable * rhs) {
    try {
        util::get_value<nihil::nil>(rhs);
        return fell::util::make_var<number>(0.0);
    } catch(...) {
        try {
            return fell::util::make_var<number>(
                static_cast<number::num>(
                    true && (util::get_value<number::num>(rhs) != 0.0)
                )
            );
        } catch(...) {
            return fell::util::make_var<number>(1.0);
        }
    }
}

fell::types::variable::var fell::types::func::operator || (const variable *) {
    return fell::util::make_var<number>(1.0);
}

fell::types::variable::var fell::types::func::call(std::vector<lex::inmemory> && params, bool protected_call) {
    if(std::get<2>(util::get_value<types::func::data>(this))) {
        auto embed = std::get<2>(util::get_value<types::func::data>(this));
        return embed(api::params{std::move(params)});
    } else {
        lex::contexts.emplace_back();

        try {
            for(std::size_t i = 0; auto & param_name : std::get<0>(util::get_value<types::func::data>(this))) {
                (*lex::contexts.rbegin())[param_name] = std::move(params.at(i++));
            }
        } catch(...) {
            lex::contexts.pop_back();
            throw std::runtime_error{"Not enough arguments to call function."};
        }

        if(!protected_call) {
            lex::eval_code(std::get<1>(util::get_value<types::func::data>(this)));

            auto & context = *lex::contexts.rbegin();

            lex::inmemory ret;
            ret = std::exchange(context["ret"], lex::inmemory{});

            lex::contexts.pop_back();

            if(ret.non_reference) {
                return std::move(ret.non_reference);
            } else if(ret.reference) {
                types::variable::var v;
                util::copy(v, *ret.reference);
                return v;
            } else {
                return util::make_var<nihil>();
            }
        } else {
            auto tbl = api::make_var<types::table>();

            (*tbl)["errmsg"] = util::make_var<types::string>("");
            (*tbl)["errc"] = util::make_var<types::number>(0);

            try {
                lex::eval_code(std::get<1>(util::get_value<types::func::data>(this)), protected_call);

                auto & context = *lex::contexts.rbegin();

                lex::inmemory ret;
                ret = std::exchange(context["ret"], lex::inmemory{});

                lex::contexts.pop_back();

                if(ret.non_reference) {
                    (*tbl)["value"] = std::move(ret.non_reference);
                } else if(ret.reference) {
                    util::copy((*tbl)["value"], *ret.reference);
                } else {
                    (*tbl)["value"] = util::make_var<nihil>();
                }

                return tbl;
            } catch(std::exception & e) {
                lex::contexts.pop_back();

                (*tbl)["errmsg"] = util::make_var<types::string>(e.what());
                (*tbl)["errc"] = util::make_var<types::number>(static_cast<number::num>(::std::strlen(e.what())));
                (*tbl)["value"] = util::make_var<types::nihil>();

                return tbl;
            }
        }
    }
}
