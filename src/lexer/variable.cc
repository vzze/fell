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

        bool member_access = false;

        while(std::strchr(" +-%*/)", expr[i]) == 0 && i < expr.length()) {
            if(expr[i] == '[') {
                member_access = true;
                break;
            }
            ++i;
        }

        types::variable::var intermediary;

        const auto var = std::string_view{expr.data() + j, i - j};

        if(member_access) {
            const auto & ref = (*lang::global_table)[std::string{var}];

            if(ref == nullptr)
                throw std::runtime_error{"Undefined variable: " + std::string{var}};

            util::copy(intermediary, ref);

            while(expr[i] == '[') {
                ++i;

                std::size_t p = i;

                std::size_t paren_counter = 1;

                while(paren_counter) {
                    if(expr[i] == '[')
                        ++paren_counter;
                    if(expr[i] == ']')
                        --paren_counter;
                    ++i;
                }

                --i;

                const auto copy{i};

                --i;

                while(std::isspace(expr[i])) --i;

                if(p == i + 1)
                    throw std::runtime_error{"Expected expression."};

                i = copy + 1;

                const auto member_expr = std::string_view{expr.data() + p, i - p - 1};

                const auto res = solve_expression(member_expr);

                const auto member = std::move((*intermediary)[res]);

                if(member == nullptr)
                    throw std::runtime_error{"Undefined member: " + util::get_value<types::string::str>(res)};

                util::copy(intermediary, member);
            }

            --i;

            vars.push(std::move(intermediary));
        } else {
            --i;
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
}
