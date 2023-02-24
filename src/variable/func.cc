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

fell::types::variable::var fell::types::func::call(std::vector<lex::inmemory> && params) {
    if(std::get<2>(util::get_value<types::func::data>(this))) {
        auto embed = std::get<2>(util::get_value<types::func::data>(this));
        return embed(api::params{std::move(params)});
    } else {
        lex::contexts.push_back({});

        try {
            for(std::size_t i = 0; auto & param_name : std::get<0>(util::get_value<types::func::data>(this))) {
                (*lex::contexts.rbegin())[param_name] = std::move(params.at(i++));
            }
        } catch(...) {
            lex::contexts.pop_back();
            throw std::runtime_error{"Not enough arguments to call function."};
        }

        lex::eval_code(std::get<1>(util::get_value<types::func::data>(this)));

        lex::contexts.pop_back();
    }

    return util::make_var<nihil>();
}
