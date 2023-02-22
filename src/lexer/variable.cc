#include "lexer.hh"

fell::types::variable::var fell::lex::check_for_constant_expression(const std::string_view expr) {
    if(expr == keywords::FALSE)
        return util::make_var<fell::types::number>(0);
    else if(expr == keywords::TRUE)
        return util::make_var<fell::types::number>(1);
    else if(expr == keywords::NIHIL)
        return util::make_var<fell::types::nihil>();

    return util::make_var<fell::types::number>(util::str_view_tod(expr));
}

void fell::lex::solve_variable(const std::string_view expr, std::stack<types::variable::var> & vars, std::size_t & i, bool & alternance) {
    if(alternance == true)
        throw std::runtime_error{"Extra token."};
    alternance = true;

    if(expr[i] == '"') {
        solve_string(expr, vars, i);
    } else if(expr[i] == '{') {
        solve_table(expr, vars, i);
    } else {
        const std::size_t j = i;

        while(std::strchr(" +-%*/)", expr[i]) == 0 && i < expr.length()) {
            ++i;
        }

        const auto var = std::string_view{expr.data() + j, i - j};
        --i;

        types::variable::var intermediary;

        try {
            intermediary = check_for_constant_expression(var);
        } catch(...) {
            const auto & ref = (*lang::global_table)[std::string{var}];

            if(ref == nullptr)
                throw std::runtime_error{"Undefined variable: " + std::string{var}};

            util::copy(intermediary, ref);
        }

        vars.push(std::move(intermediary));
    }
}
