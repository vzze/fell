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

void fell::lex::solve_variable(
    const std::string_view expr,
    std::stack<inmemory> & vars,
    std::size_t & i,
    bool & alternance
) {
    if(alternance == true)
        throw std::runtime_error{"Extra token."};
    alternance = true;

    if(expr[i] == '"') {
        solve_string(expr, vars, i);
    } else if(expr[i] == '|') {
        solve_func(expr, vars, i);
    } else if(expr[i] == '{') {
        solve_table(expr, vars, i);
    } else {
        const std::size_t j = i;

        while((std::isalnum(expr[i]) || expr[i] == '_') && i < expr.length()) {
            ++i;
        }

        const auto var = std::string_view{expr.data() + j, i - j};

        if(var == "local") {
            local_value = true;
            alternance = false;
            return;
        }

        if(var == "Global") {
            vars.emplace(&global_table);
            return;
        }

        if(var == "ret") {
            if(contexts.rbegin() == contexts.rend())
                throw std::runtime_error{"Can't return from global context."};

            auto & ref = (*contexts.rbegin())[std::string{var}];

            ref.non_reference = util::make_var<types::nihil>();

            vars.emplace(&ref.non_reference);
            return;
        }

        --i;

        try {
            vars.emplace(check_for_constant_expression(var));
        } catch(...) {
            if(local_value) {
                local_value = false;

                if(contexts.rbegin() == contexts.rend())
                    throw std::runtime_error{"Can't make global variable local."};

                auto & ref = (*contexts.rbegin())[std::string{var}];

                ref.non_reference = util::make_var<types::nihil>();

                vars.emplace(&ref.non_reference);
                return;
            }

            auto it = contexts.rbegin();

            while(it != contexts.rend()) {
                auto & ref = (*it)[std::string{var}];
                if(ref.non_reference) {
                    vars.emplace(&ref.non_reference);
                    return;
                } else if(ref.reference) {
                    vars.emplace(ref.reference);
                    return;
                }
                ++it;
            }

            auto & ref = (*global_table)[std::string{var}];

            if(ref == nullptr)
                ref = util::make_var<types::nihil>();

            vars.emplace(&ref);
        }
    }
}
