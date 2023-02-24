#include "variable.hh"
#include "util.hh"
#include "lexer.hh"
#include "lang.hh"

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

fell::types::variable::var fell::types::func::call(std::vector<variable::var> && params, std::vector<bool> && references) {
    lang::contexts.push_back(util::make_var<types::table>());

    try {
        for(auto & param_name : std::get<0>(util::get_value<types::func::data>(this))) {
            (**lang::contexts.rbegin())[param_name] = std::move(params.at(0));
            params.erase(params.begin());
        }
    } catch(...) {
        lang::contexts.pop_back();
        throw std::runtime_error{"Not enough arguments to call function."};
    }

    lang::dump_table(lang::global_table);
    lex::eval_code(std::get<1>(util::get_value<types::func::data>(this)));
    lang::dump_table(lang::global_table);

    for(std::size_t i = 0; auto & param_name : std::get<0>(util::get_value<types::func::data>(this))) {
        if(references[i++]) {
            std::cout << param_name << '\n';
            auto & ref = (**lang::contexts.rbegin())[param_name];
            [[maybe_unused]] auto releasing_ref = ref.release();
        }
    }
    lang::contexts.pop_back();
    return util::make_var<nihil>();
}
