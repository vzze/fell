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

        while(std::strchr(" \",;=<>!&?+-%*/()[]{}", expr[i]) == 0 && i < expr.length()) {
            ++i;
        }

        const auto var = std::string_view{expr.data() + j, i - j};

        if(var == "local") {
            local_value = true;
            alternance = false;
            return;
        }

        /* std::cout << var << '\n'; */

        if(var == "Global") {
            return vars.push(inmemory{&global_table});
        }

        if(var == "ret") {
            if(contexts.rbegin() == contexts.rend())
                throw std::runtime_error{"Can't return from global context."};

            auto & ref = (*contexts.rbegin())[std::string{var}];

            ref.non_reference = util::make_var<types::nihil>();

            return vars.push(inmemory{&ref.non_reference});
        }

        --i;

        try {
            vars.push(inmemory{check_for_constant_expression(var)});
        } catch(...) {
            if(local_value) {
                local_value = false;

                if(contexts.rbegin() == contexts.rend())
                    throw std::runtime_error{"Can't make global variable local."};

                auto & ref = (*contexts.rbegin())[std::string{var}];

                ref.non_reference = util::make_var<types::nihil>();

                return vars.push(inmemory{&ref.non_reference});
            }

            auto it = contexts.rbegin();

            while(it != contexts.rend()) {
                auto & ref = (*it)[std::string{var}];
                if(ref.non_reference) {
                    return vars.push(inmemory{&ref.non_reference});
                } else if(ref.reference) {
                    return vars.push(inmemory{ref.reference});
                }
                ++it;
            }

            auto & ref = (*global_table)[std::string{var}];

            if(ref == nullptr)
                ref = util::make_var<types::nihil>();

            vars.push(inmemory{&ref});
        }
    }
}
